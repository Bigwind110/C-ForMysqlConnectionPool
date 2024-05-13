#pragma once
#include <iostream>
#include <memory>//shared_prt所需头文件
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
* 数据库连接池: 单例模式
* MySqlConn 是一个连接MySQL数据库的类
*/
class ConnectionPool
{
public:
	// 得到单例对象
	static ConnectionPool* getConnectionPool();
	// 从连接池中取出一个连接
	std::shared_ptr<MysqlConn*> getConnection();
	// 删除拷贝构造和拷贝赋值运算符重载函数
	ConnectionPool(const ConnectionPool& other) = delete;
	ConnectionPool& operator=(const ConnectionPool& other) = delete;

private:
	// 构造函数私有化
	ConnectionPool();
	// 析构函数私有化
	~ConnectionPool();
	//加载Json配置文件
	bool parseJsonFile();
	//生产者
	void produceConnection();
	//回收
	void recycleConnection();
	//添加Mysql连接
	void addConnection();

	std::string m_ip;             // 数据库服务器ip地址
	std::string m_user;           // 数据库服务器用户名
	std::string m_dbName;         // 数据库服务器的数据库名
	std::string m_passwd;         // 数据库服务器密码
	unsigned short m_port;   // 数据库服务器绑定的端口
	int m_minSize;           // 连接池维护的最小连接数
	int m_maxSize;           // 连接池维护的最大连接数
	int m_maxIdleTime;       // 连接池中连接的最大空闲时长
	int m_timeout;           // 连接池获取连接的超时时长

	std::queue<MysqlConn*> m_connectionQ;
	std::mutex m_mutexQ;
	std::condition_variable m_cond;
};

