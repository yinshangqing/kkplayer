#ifndef MY_DATABASE_H
#define MY_DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class MyDatabase
{
public:
	MyDatabase();
	~MyDatabase();

private:
	QSqlDatabase	mDb;
	QSqlQuery		mSq;

public:
	bool Connect(QString dbName, QString dbUserName, QString dbPassword);
	void DisConnect();
	bool Open();
	void Close();
	void GetSq(QSqlQuery &sq){ sq = mSq; };
	bool ExecuteSql(QString tableString);
	bool CreateVideoSetTable();
	bool CreateVideoTable();
	bool CreateSettingTable();
};

#endif