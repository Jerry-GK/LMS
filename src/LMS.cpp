#include<windows.h>
#include<iostream>
#include <assert.h>
#include<sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include "DataBase.h"
#include "Library.h"
using namespace std;

int main() 
{
    cout << "System Initializing, please wait..." << endl;
    Library lib;
    lib.Init();
    string adminID = "001";
    string password = "1234";
    int login = lib.adm_log_in(adminID, password);

    if(login==1)
    {
        cout << "Admin " << adminID << " logged in successfully!" << endl;

        //lib.borrow_book("101","002");
        //lib.borrow_book("102", "001");
        //lib.borrow_book("102","003");

        vector<Book> vb = lib.query_book_by_card("102");         
        //vb = lib.query_book("","","","",-1,-1,"",-1,-1,"");
        for (vector<Book>::iterator it = vb.begin(); it != vb.end(); it++)
             cout << it->title << endl;
        //lib.add_one_book("001", "", "", "USA Press", 2021, "Trump", 12.34, 3);
        //lib.return_book("102","002");
        //lib.add_card("103","Li Hua","CS","T");
        //lib.borrow_book("101","001");
    }
    else
    {
        cout << "AdminID or password is not correct!" << endl;
    }
      
    //lib.add_one_book("001", "TextBook", "DataBase System", "Zhejiang University Press", 2022, "Wu Zhaohui", 32.5, 8, 8);
    lib.quit();
    cout << "System Quit!" << endl;
    system("pause");
}
