#pragma once
#include <iostream>
#include <memory>//shared_prt����ͷ�ļ�
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include<fstream>
#include<json/json.h>
#include"MysqlConn.h"
/*
* ���ݿ����ӳ�: ����ģʽ
* MySqlConn ��һ������MySQL���ݿ����
*/
class ConnectionPool
{
public:
	// �õ���������
	static ConnectionPool* getConnectionPool();
	// �����ӳ���ȡ��һ������
	std::shared_ptr<MysqlConn*> getConnection();
	// ɾ����������Ϳ�����ֵ��������غ���
	ConnectionPool(const ConnectionPool& other) = delete;
	ConnectionPool& operator=(const ConnectionPool& other) = delete;

private:
	// ���캯��˽�л�
	ConnectionPool();
	// ��������˽�л�
	~ConnectionPool();
	//����Json�����ļ�
	bool parseJsonFile();
	//������
	void produceConnection();
	//����
	void recycleConnection();
	//���Mysql����
	void addConnection();

	std::string m_ip;             // ���ݿ������ip��ַ
	std::string m_user;           // ���ݿ�������û���
	std::string m_dbName;         // ���ݿ�����������ݿ���
	std::string m_passwd;         // ���ݿ����������
	unsigned short m_port;   // ���ݿ�������󶨵Ķ˿�
	int m_minSize;           // ���ӳ�ά������С������
	int m_maxSize;           // ���ӳ�ά�������������
	int m_maxIdleTime;       // ���ӳ������ӵ�������ʱ��
	int m_timeout;           // ���ӳػ�ȡ���ӵĳ�ʱʱ��

	std::queue<MysqlConn*> m_connectionQ;
	std::mutex m_mutexQ;
	std::condition_variable m_cond;
};

