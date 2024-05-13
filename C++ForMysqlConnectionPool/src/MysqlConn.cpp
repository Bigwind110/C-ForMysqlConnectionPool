#include "MysqlConn.h"
MysqlConn::MysqlConn()
	:m_conn(nullptr)
{
	m_conn = mysql_init(nullptr);
	mysql_set_character_set(m_conn, "utf8");
}

MysqlConn::~MysqlConn()
{
	if (nullptr != m_conn)
	{
		mysql_close(m_conn);
	}
	freeResult();
}

bool MysqlConn::connect(std::string user, std::string passwd, std::string dbName, std::string ip, unsigned short port)
{

	if (nullptr == mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0))
	{
		return false;
	}
	return true;
}

bool MysqlConn::update(std::string sql)
{
	if (0 != mysql_query(m_conn, sql.c_str()))
	{
		return true;
	}
	return false;
}

bool MysqlConn::query(std::string sql)
{
	freeResult();
	if (0 != mysql_query(m_conn, sql.c_str()))
	{
		m_result = mysql_store_result(m_conn);
		return true;
	}
	return false;
}

bool MysqlConn::next()
{
	if (nullptr != m_result)
	{
		m_row = mysql_fetch_row(m_result);
		return true;
	}
	return false;
}

std::string MysqlConn::value(int index)
{
	int dbNum = mysql_num_fields(m_result);
	if (0 > index || index > dbNum)
	{
		return std::string();
	}
	char* val = m_row[index];
	unsigned int length = mysql_fetch_lengths(m_result)[index];

	return std::string(val, length);
}

bool MysqlConn::transacation()
{
	return mysql_autocommit(m_conn, false);
}

bool MysqlConn::commit()
{
	return mysql_commit(m_conn);
}

bool MysqlConn::roolback()
{
	return mysql_rollback(m_conn);
}

long long MysqlConn::getAliveTime()
{
	std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_aliveTime;
	std::chrono::milliseconds millsec = std::chrono::duration_cast<std::chrono::milliseconds>(res);
	return millsec.count();
}

void MysqlConn::refreshAliveTime()
{
	m_aliveTime = std::chrono::steady_clock::now();
}

void MysqlConn::freeResult()
{
	if (nullptr != m_result)
	{
		mysql_free_result(m_result);
		m_result = nullptr;
	}
}
