#pragma once
#include<mysql.h>
#include<iostream>
#include<chrono>

class MysqlConn
{
public:
	//��ʼ�����ݿ�����
	MysqlConn();
	//�ͷ����ݿ�����
	~MysqlConn();
	//�������ݿ�
	bool connect(std::string user, std::string passwd, std::string dbName, std::string ip = "localhost", unsigned short port = 3306);
	//�������ݿ�: insert, update, delete
	bool update(std::string sql);
	//��ѯ���ݿ�
	bool query(std::string sql);
	//������ѯ�õ��Ľ����
	bool next();
	//�õ�������е��ֶ�ֵ
	std::string value(int index);
	//�������
	bool transacation();
	//�ύ����
	bool commit();
	//����ع�
	bool roolback();
	//������ʱ��
	long long getAliveTime();
	//ˢ�´����ʼʱ��
	void refreshAliveTime();
	//TODO
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
	std::chrono::steady_clock::time_point m_aliveTime;
};

