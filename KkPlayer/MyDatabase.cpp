#include "stdafx.h"
#include "MyDatabase.h"
#include <QDebug>

MyDatabase::MyDatabase()
{
}


MyDatabase::~MyDatabase()
{
}

bool MyDatabase::Connect(QString dbName, QString dbUserName, QString dbPassword)
{
	bool bConnected = true;
	if (QSqlDatabase::contains("qt_sql_default_connection"))
	{
		mDb = QSqlDatabase::database("qt_sql_default_connection");
	}
	else
	{
		mDb = QSqlDatabase::addDatabase("QSQLITE");
		mDb.setDatabaseName(dbName + ".db");
		mDb.setUserName(dbUserName);
		mDb.setPassword(dbPassword);
		bConnected = mDb.open();
	}

	mSq = QSqlQuery::QSqlQuery(mDb);
	return bConnected;
}

void MyDatabase::DisConnect()
{

}

bool MyDatabase::Open()
{
	bool bRet = true;
	if (!mDb.open())
	{
		qDebug() << "Failed to open data base, error = " << mDb.lastError();
		bRet = false;
	}
	return bRet;
}

void MyDatabase::Close()
{
	mDb.close();
}

bool MyDatabase::ExecuteSql(QString sqlCmds)
{
	//����һ����Ϊstudent�ı�񣬱��������У���һ����id���ڶ��������֣�������������, "create table student (id int primary key, name varchar(30), age int)"
	//����һ������, "insert into student values (?, ?, ?)"
	//�޸�����, update student set name = :name where id = :id
	//��ѯ��������, select id, name from student
	//��ѯ��������, select * from student
	//��ѯ���id, select max(id) from student
	//ɾ��һ������, "QString delete_sql = "delete from student where id = ?""
	//��ձ��ɾ�����У�, "QString clear_sql = "delete from student""
	bool bRet = true;
	mSq.prepare(sqlCmds);
	if (!mSq.exec())
	{
		qDebug() << "Failed to create table, exe string = " << sqlCmds << ", last error = " << mSq.lastError();
		bRet = false;
	}
	else
	{
		qDebug() << "Table created success, exe string = " << sqlCmds;
	}
	return bRet;
}

bool MyDatabase::CreateVideoSetTable()
{
	bool bRet = false;
	QString create_sql;
	mSq.exec(QString("select count(*) from sqlite_master where type='table' and name='VideoSet'"));
	if (mSq.next())
	{
		if (mSq.value(0).toInt() == 0)
		{
			// ������
			create_sql = "CREATE TABLE IF NOT EXISTS VideoSet(id int primary key, setName varchar(256), setId int, videoCount int, setFileFolder varchar(256))";
			bRet = ExecuteSql(create_sql);
		}
		else
		{
			//�����
		}
	}
	
	return bRet;
}

bool MyDatabase::CreateVideoTable()
{
	bool bRet = false;
	QString create_sql;
	mSq.exec(QString("select count(*) from sqlite_master where type='table' and name='Video'"));
	if (mSq.next())
	{
		if (mSq.value(0).toInt() == 0)
		{
			// ������
			create_sql = "CREATE TABLE IF NOT EXISTS Video (id int primary key, videoId varchar(20), \
						 	videoName varchar(256), videoFilePath varchar(256), videoPlayTimeStamp int, setId int)";
			bRet = ExecuteSql(create_sql);
		}
		else
		{
			//�����
		}
	}
	return bRet;
}