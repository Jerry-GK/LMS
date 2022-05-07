#pragma once
#include<windows.h>
#include<iostream>
#include <assert.h>
#include<sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#define TO_SQLCHAR(s) ((SQLCHAR*)((s).c_str()))

using namespace std;

class DataBase
{
public:
	SQLHENV serverhenv;						//环境句柄
	SQLHDBC serverhdbc;						//连接句柄
	SQLHSTMT serverhstmt;					//语句句柄
	SQLRETURN ret;							//结果集

	DataBase();
	void get_connection(string dataSource, string userID, string password);//connect the data source
	void alloc_stmt_handle();//allocate the statement handle
	void execute_stmt(string stmt);//execute the SQL statement
	void free_stmt_handle();//free the statement handle
	bool stmt_fetch_data();//fetch the data from stmt handle and move to the next, return if valid data has been fetched
	void free_connection();//free the data source connection
};
