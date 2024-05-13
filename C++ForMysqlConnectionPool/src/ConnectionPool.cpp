#include "ConnectionPool.h"
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool;
	return &pool;
}

std::shared_ptr<MysqlConn*> ConnectionPool::getConnection()
{
	std::unique_lock<std::mutex> locker(m_mutexQ);
	while (m_connectionQ.empty())
	{
		if (std::cv_status::timeout == m_cond.wait_for(locker, std::chrono::milliseconds(m_timeout)))
		{
			if (m_connectionQ.empty())
			{
				continue;
			}
		}
	}
	std::shared_ptr<MysqlConn> connPrt(m_connectionQ.front(), [this](MysqlConn* conn) {
		m_mutexQ.lock();
		conn->refreshAliveTime();
		m_connectionQ.emplace(conn);
		m_mutexQ.unlock();
		});
	m_connectionQ.pop();
	m_cond.notify_all();
	return std::shared_ptr<MysqlConn*>();
}

ConnectionPool::ConnectionPool()
{
	if (!parseJsonFile())
	{
		return;
	}
	for (int i = 0; i < m_minSize; i++)
	{
		addConnection();
	}
	std::thread producer(&ConnectionPool::produceConnection, this);
	std::thread recycler(&ConnectionPool::recycleConnection, this);
	producer.detach();
	recycler.detach();
}

ConnectionPool::~ConnectionPool()
{
	while (!m_connectionQ.empty())
	{
		MysqlConn* conn = m_connectionQ.front();
		m_connectionQ.pop();
		delete conn;
	}
}

bool ConnectionPool::parseJsonFile()
{
	using namespace Json;
	std::ifstream ifs("dbconf.json");
	Reader rd;
	Value root;
	rd.parse(ifs, root);
	if (root.isObject())
	{
		m_ip = root["ip"].asString();
		m_user = root["user"].asString();
		m_dbName = root["dbName"].asString();
		m_passwd = root["passwd"].asString();
		m_port = root["port"].asInt();
		m_minSize = root["minSize"].asInt();
		m_maxSize = root["maxSize"].asInt();
		m_maxIdleTime = root["maxIdleTime"].asInt();
		m_timeout = root["timeout"].asInt();
		return true;
	}
	return false;
}

void ConnectionPool::produceConnection()
{
	while (true)
	{
		std::unique_lock<std::mutex> locker(m_mutexQ);
		while (m_connectionQ.size() >= m_minSize)
		{
			m_cond.wait(locker);
		}
		addConnection();
		m_cond.notify_all();
	}
}

void ConnectionPool::recycleConnection()
{
	while (true)
	{

		while (m_connectionQ.size() > m_maxSize)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			m_mutexQ.lock();
			/*std::unique_lock<std::mutex> locker(m_mutexQ);
			while (m_connectionQ.size() < m_minSize)
			{
				m_cond.wait(locker);
			}*/
			MysqlConn* lastConn = m_connectionQ.front();
			if (lastConn->getAliveTime() >= m_maxIdleTime)
			{
				m_connectionQ.pop();
				delete lastConn;
			}
			else
			{
				break;
			}
			m_mutexQ.unlock();
		}
	}
}

void ConnectionPool::addConnection()
{

	MysqlConn* connection = new MysqlConn;
	connection->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
	connection->refreshAliveTime();
	m_connectionQ.emplace(connection);
}
