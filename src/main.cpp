#include<windows.h>
#include<iostream>
#include <vector>
#include <string>
#include <assert.h>
#include<sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <QApplication>

#include "DataBase.h"
#include "Library.h"
#include "MainWidget.h"
#include <QPushButton>

using namespace std;

int main(int argc, char *argv[])
{
    cout<<"This is a log for the Library Management System!"<<endl;
    cout << "\nSYS MSG: SYSTEM INITIALIZING..." << endl;
    QApplication app(argc, argv);
    Library lib;
    lib.Init();

    MainWidget mWidget(&lib);
    mWidget.show();

    int result=app.exec();
    lib.quit();
    cout << "\nSYS MSG: SYSTEM QUIT!" << endl;
    return result;
}
