#include "DataBase.h"
using namespace std;

DataBase::DataBase()
{
    cout << "\nDB MSG: DATABSE INITIALIZING..." << endl;
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &serverhenv);

    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: ENVIRONMENT HANDLE CAN NOT BE ALLOCATED!"<< endl;
        system("pause");
        exit(-1);
    }
    //设置环境属性
    ret = SQLSetEnvAttr(serverhenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: ENVIRONMENT HANDLE ATTRIBUTES CAN NOT BE SET!" << endl;
        system("pause");
        exit(-1);
    }
    //分配连接句柄
    ret = SQLAllocHandle(SQL_HANDLE_DBC, serverhenv, &serverhdbc);
    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: CONNECTION HANDLE CAN NOT BE ALLOCATED!" << endl;
        system("pause");
        exit(-1);
    }
    serverhstmt = 0;
}

void DataBase::get_connection(string dataSource, string userID, string password)
{
    //数据库连接。第二个参数是之前配置的数据源，后面是数据库用户名和密码
    ret = SQLConnectA(serverhdbc, (SQLCHAR*)dataSource.c_str(), SQL_NTS, (SQLCHAR*)userID.c_str(), SQL_NTS,(SQLCHAR*)password.c_str(), SQL_NTS);

    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: DATABASE CONNECTION FAILED!" << endl;
        cout<<"\nHINT: PLEASE CHECK YOUR ENVIRONMENT FOR ODBC32 DATASOURCE, AND THE NAME OF LIBSOURSE, USER AND THE PASSWORD!"<<endl;
        system("pause");
        exit(-1);
    }
    cout << "\nDB MSG: DATASOURCE<"+dataSource+">, USER<"+userID + ">CONNECT SUCCESSFULLY!" << endl;
}

void DataBase::alloc_stmt_handle()
{
    this->ret = SQLAllocHandle(SQL_HANDLE_STMT, this->serverhdbc, &this->serverhstmt);
    if (!SQL_SUCCEEDED(this->ret))
    {
        cout << "\nDB ERROR: STATEMENT HANDLE CAN NOT BE ALLOCATED!" << endl;
        system("pause");
        exit(-1);
    }
}

void DataBase::execute_stmt(string stmt)
{
    this->ret = SQLExecDirectA(this->serverhstmt, TO_SQLCHAR(stmt), SQL_NTS);
    if (!SQL_SUCCEEDED(this->ret))
    {
        cout << "\nDB ERROR: SQL STATEMENT:\n\"" << stmt << "\"\nIS NOT VALID!" << endl;
        cout<<"\nHINT: DO NOT INPUT CHINESE CHARACTERS, \' , OR \" (TO AVOID SQL INJECTION)!"<<endl;
        system("pause");
        exit(-1);
    }
    else
       cout << "\nDB MSG: SQL STATEMENT:\n\"" << stmt << "\"\nEXECUTED SUCCESSFULLY!" << endl;
}

void DataBase::free_stmt_handle()
{
    this->ret = SQLFreeHandle(SQL_HANDLE_STMT, this->serverhstmt);
    if (!SQL_SUCCEEDED(this->ret))
    {
        cout << "\nDB ERROR: STATEMENT HANDLE CAN NOT BE FREED!" << endl;
        system("pause");
        exit(-1);
    }
}

bool DataBase::stmt_fetch_data()
{
    return SQL_NO_DATA != SQLFetch(this->serverhstmt);
}

void DataBase::free_connection()
{
    ret = SQLEndTran(SQL_HANDLE_DBC, serverhdbc, SQL_COMMIT); ;
    ret = SQLDisconnect(serverhdbc);
    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: DISCONNECTION FAILED!" << endl;
        system("pause");
        exit(-1);
    }
    ret = SQLFreeHandle(SQL_HANDLE_DBC, serverhdbc);
    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: CONNECTION HANDLE CAN NOT BE FREED!" << endl;
        system("pause");
        exit(-1);
    }
    ret = SQLFreeHandle(SQL_HANDLE_ENV, serverhenv);
    if (!SQL_SUCCEEDED(ret))
    {
        cout << "\nDB ERROR: ENVIRONMENT HANDLE CAN NOT BE FREED!" << endl;
        system("pause");
        exit(-1);
    }
}
