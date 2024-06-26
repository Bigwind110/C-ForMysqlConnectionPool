#pragma once
#include<mysql.h>
#include<iostream>
#include<chrono>

class MysqlConn
{
public:
	//初始化数据库连接
	MysqlConn();
	//释放数据库连接
	~MysqlConn();
	//连接数据库
	bool connect(std::string user, std::string passwd, std::string dbName, std::string ip = "localhost", unsigned short port = 3306);
	//更新数据库: insert, update, delete
	bool update(std::string sql);
	//查询数据库
	bool query(std::string sql);
	//遍历查询得到的结果集
	bool next();
	//得到结果集中的字段值
	std::string value(int index);
	//事务操作
	bool transacation();
	//提交事务
	bool commit();
	//事务回滚
	bool roolback();
	//计算存活时间
	long long getAliveTime();
	//刷新存活起始时间
	void refreshAliveTime();
	//TODO
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
	std::chrono::steady_clock::time_point m_aliveTime;
};

