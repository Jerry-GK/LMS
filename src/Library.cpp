#pragma once
#include "Library.h"
#include "DataBase.h"
using namespace std;

void Library::Init()
{
    this->cur_adminID = "_NONE";//not log in yet
    string datasource,user,pw;
    cout<<"\nLIB MSG: CONNECT TO THE DATABASE FIRST!"<<endl;
    cout<<"\nPlease enter datasource name(recommonded to be \"LibSource\"):"<<endl;
    cin>>datasource;
    cout<<"Please enter user name(recommonded to be \"root\"):"<<endl;
    cin>>user;
    cout<<"Please enter user's password:"<<endl;
    cin>>pw;
    db.get_connection(datasource, user, pw);

    string stmt="create database if not exists lib;";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
    db.free_stmt_handle();

    stmt ="use lib;";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
	db.free_stmt_handle();

    stmt=" create table if not exists book(\
            bno varchar(100) primary key,\
            category varchar(100),\
            title varchar(100),\
            press varchar(100),\
            year int,\
            author varchar(100),\
            price float,\
            total int,\
            stock int\
        );";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
    db.free_stmt_handle();

    stmt="  create table if not exists card(\
            cno varchar(100) primary key,\
            name varchar(100),\
            department varchar(100),\
            type char(1)\
        );";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
    db.free_stmt_handle();

    stmt="  create table if not exists admins(\
            admID varchar(100) primary key,\
            pw varchar(100),\
            name varchar(100),\
            phone varchar(100)\
            );";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
    db.free_stmt_handle();

    stmt="  create table if not exists borrow(\
            bno varchar(100),\
            cno varchar(100),\
            borrow_date date,\
            return_date date,\
            handlerID varchar(100),\
            foreign key(bno) references book(bno),\
            foreign key(cno) references card(cno),\
            foreign key(handlerID) references admins(admID)\
        );";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //no read
    db.free_stmt_handle();

    bool no_admin=true;
    stmt="select * from admins";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //read
    if(db.stmt_fetch_data())
        no_admin=false;
    db.free_stmt_handle();

    if(no_admin)
    {
        stmt="insert into admins values('001','1234','admin','13588888888')";
        db.alloc_stmt_handle();
        db.execute_stmt(stmt);
        //no read
        db.free_stmt_handle();
    }
    return;
}

int Library::adm_log_in(string adminID, string password)
{
    int ret=2;
	string stmt = "select pw from admins where admID = '" + adminID + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
    if (db.stmt_fetch_data())
	{
		SQLCHAR correct_pw[BUFFER_SIZE] = { 0 };
		SQLINTEGER len;
        SQLGetData(db.serverhstmt, 1, SQL_C_CHAR, (void*)correct_pw, sizeof(correct_pw), &len);
		string cpw((const char*)correct_pw);
		if (password == cpw)
		{
			this->cur_adminID = adminID;
            ret = 1;//登陆成功
            cout<<"\nLIB MSG: ADMIN "+cur_adminID+" LOG IN SUCCESSFULLY!"<<endl;
		}
		else
            ret = 3;//密码错误
	}
	else
        ret = 2;//ID不存在
	db.free_stmt_handle();
    return ret;
}

void Library::adm_log_out()
{
    cout<<"\nLIB MSG: ADMIN "+cur_adminID+" LOG OUT SUCCESSFULLY!"<<endl;
    this->cur_adminID="_NONE";
}

int Library::add_book(string bno, string category, string title, string press, int year, string author, float price, int num)
{
    if(bno.empty()||num<=0)
        return 2;//info not complete

	//check if the book exists
	bool exists = false;
	string stmt = "select * from book where bno = '" + bno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
		exists = true;
	}
	db.free_stmt_handle();

	if (exists)//the added book exists, change the total and stock
	{
		//update total
		string stmt_total = "update book set total = total + " + to_string(num) + " where bno = " + bno + "; ";
		db.alloc_stmt_handle();
		db.execute_stmt(stmt_total);
		//no read
		db.free_stmt_handle();

		//update stock
		string stmt_stock = "update book set stock = stock + " + to_string(num) + " where bno = " + bno + "; ";
		db.alloc_stmt_handle();
		db.execute_stmt(stmt_stock);
		//no read
		db.free_stmt_handle();

        return 1;
	}

	//add new book
    if(bno.empty()||category.empty()||title.empty()||press.empty()||year==0||author.empty()||price==0||num==0)
        return 3;//info not complete
	stmt = "insert into book values('"+bno+"', '"+category+"', '"+title+"', '"+press+"', "+to_string(year)+", '"+author+"', "+to_string(price)+", "
					+to_string(num)+", "+to_string(num)+");";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();

    return 1;
}

int Library::add_books_by_file(string filename)
{
    string final_filename="..\\doc\\"+filename;
    ifstream ifs(final_filename,ios::in);

    if(!ifs.is_open())
    {
        return 2;
    }
    string bno,category,title,press,author;
    int year,num;
    float price;
    while(ifs)
    {
        ifs>>bno>>category>>title>>press>>year>>author>>price>>num;
        replace(bno.begin(),bno.end(),'_',' ');
        replace(category.begin(),category.end(),'_',' ');
        replace(title.begin(),title.end(),'_',' ');
        replace(press.begin(),press.end(),'_',' ');
        replace(author.begin(),author.end(),'_',' ');

        int ret=add_book(bno,category,title,press,year,author,price,num);
        if(ret!=1)
            return 3;
    }
    ifs.close();
    return 1;
}

vector<Book> Library::query_book(string bno, string category, string title, string press, int year_begin, int year_end, 
									string author, float price_begin, float price_end, string order)
{
	string stmt = "select * from book where ";
	vector<Book> ret;
	int flag = 0;
	if (bno != "")
	{
		stmt.append((string)(flag==1?" and ":"") + "bno = '" + bno + "'");
		flag = 1;
	}
	if(category != "")
	{
		stmt.append((string)(flag == 1 ? " and " : "") + "category = '" + category + "'");
		flag = 1;
	}
	if(title != "")
	{
		stmt.append((string)(flag == 1 ? " and " : "") + "title = '" + title + "'");
		flag = 1;
	}
	if(press != "")
	{
		stmt.append((string)(flag == 1 ? " and " : "") + "press = '" + press + "'");
		flag = 1;
	}
	if(year_begin != -1)
	{
        stmt.append((string)(flag == 1 ? " and " : "") + "year >= " + to_string(year_begin));
		flag = 1;
	}
    if(year_end != -1)
    {
        stmt.append((string)(flag == 1 ? " and " : "") + "year <= " + to_string(year_end));
        flag = 1;
    }
	if(author != "")
	{
		stmt.append((string)(flag == 1 ? " and " : "") + "author = '" + author + "'");
		flag = 1;
	}
	if (price_begin != -1)
	{
        stmt.append((string)(flag == 1 ? " and " : "") + "price >= " + to_string(price_begin));
		flag = 1;
	}
    if (price_end != -1)
    {
        stmt.append((string)(flag == 1 ? " and " : "") + "price <= " + to_string(price_end));
        flag = 1;
    }
	stmt.append(" order by " + (order == "" ? "title" : order));
	stmt.append(" limit " + to_string(MAX_RETURN) + ";");

	if (flag == 0)//unconditionally select
		stmt = "select * from book order by " + (order == "" ? "title" : order) + " limit " + to_string(MAX_RETURN);

	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	if (db.ret == SQL_SUCCESS || db.ret == SQL_SUCCESS_WITH_INFO)
	{
		//copy the info of book into buffer
		SQLCHAR bno[BUFFER_SIZE],category[BUFFER_SIZE],title[BUFFER_SIZE],press[BUFFER_SIZE],author[BUFFER_SIZE];
		SQLINTEGER year, total, stock;
		SQLREAL price;
		SQLINTEGER len;
		SQLBindCol(db.serverhstmt, 1, SQL_C_CHAR, (void*)bno, sizeof(bno), &len);
		SQLBindCol(db.serverhstmt, 2, SQL_C_CHAR, (void*)category, sizeof(category), &len);
		SQLBindCol(db.serverhstmt, 3, SQL_C_CHAR, (void*)title, sizeof(title), &len);
        SQLBindCol(db.serverhstmt, 4, SQL_C_CHAR, (void*)press, sizeof(press), &len);
        SQLBindCol(db.serverhstmt, 5, SQL_C_SLONG, (void*)&year, sizeof(year), &len);

        SQLBindCol(db.serverhstmt, 6, SQL_C_CHAR, (void*)author, sizeof(author), &len);
        SQLBindCol(db.serverhstmt, 7, SQL_C_FLOAT, (void*)&price, sizeof(price), &len);
        SQLBindCol(db.serverhstmt, 8, SQL_C_SLONG, (void*)&total, sizeof(total), &len);
		SQLBindCol(db.serverhstmt, 9, SQL_C_SLONG, (void*)&stock, sizeof(stock), &len);

		while (db.stmt_fetch_data())
		{
			Book new_book;		
			new_book.bno.assign((const char *)bno);
			new_book.category.assign((const char*)category);
			new_book.title.assign((const char*)title);
			new_book.press.assign((const char*)press);
			new_book.year = year;

			new_book.author.assign((const char*)author);
			new_book.price = price;
			new_book.total = total;
			new_book.stock = stock;

			ret.push_back(new_book);
			if (ret.size() > 50)
			{
                cout << "\nLIB ERROR: MORE THAN " << MAX_RETURN << " RETURNS!" << endl;
			}
		}
	}

	db.free_stmt_handle();
	return ret;
}

int Library::add_card(string cno, string name, string department, string type)
{
    if(cno.empty()||name.empty()||department.empty()||type.empty())
        return 3;//information not complete
    bool cno_exists = false;
    string stmt = "select * from card where cno = '" + cno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
		cno_exists = true;
	}
	db.free_stmt_handle();
    if(cno_exists)
    {
        return 2;
    }
    
	stmt = "insert into card values('" + cno + "', '" + name + "', '" + department + "', '" + type + "');";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();
    return 1;
}

int Library::delete_card(string cno)
{
    bool cno_exists = false;
    bool have_book = false;
    string stmt = "select * from card where cno = '" + cno + "';";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //read
    if (db.stmt_fetch_data())
    {
        cno_exists = true;
    }
    db.free_stmt_handle();
    if(!cno_exists)
    {
        return 2;
    }

    stmt = "select * from borrow where cno = '" + cno + "';";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //read
    if (db.stmt_fetch_data())
    {
        have_book = true;
    }
    db.free_stmt_handle();
    if(have_book)
    {
        return 3;
    }

    stmt = "delete from card where cno = '" + cno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();
    return 1;
}

vector<Book> Library::query_book_by_card(string cno)
{
	string stmt = "select bno from borrow where cno = '" + cno + "';";
	string borrowed_bno = "_NOT_A_BOOK";
	vector<Book> ret;
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.ret == SQL_SUCCESS || db.ret == SQL_SUCCESS_WITH_INFO)
	{
		//copy the info of book into buffer
		SQLCHAR bno[BUFFER_SIZE];
		SQLINTEGER len;
		SQLBindCol(db.serverhstmt, 1, SQL_C_CHAR, (void*)bno, sizeof(bno), &len);
		while (db.stmt_fetch_data())
		{
			borrowed_bno.assign((const char*)bno);
			SQLHSTMT stmt_temp = db.serverhstmt;
			ret.push_back(query_book(borrowed_bno, "", "", "", -1, -1, "", -1, -1, "").front());
			db.serverhstmt = stmt_temp;
		}
	}
	db.free_stmt_handle();
	return ret;//query book only according to bno
}

vector<Card> Library::query_all_card()
{
    string stmt = "select * from card order by cno;";
    vector<Card> ret;
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //read
    if (db.ret == SQL_SUCCESS || db.ret == SQL_SUCCESS_WITH_INFO)
    {
        //copy the info of book into buffer
        SQLCHAR cno[BUFFER_SIZE], name[BUFFER_SIZE], dep[BUFFER_SIZE], type[BUFFER_SIZE];
        SQLINTEGER len;
        SQLBindCol(db.serverhstmt, 1, SQL_C_CHAR, (void*)cno, sizeof(cno), &len);
        SQLBindCol(db.serverhstmt, 2, SQL_C_CHAR, (void*)name, sizeof(name), &len);
        SQLBindCol(db.serverhstmt, 3, SQL_C_CHAR, (void*)dep, sizeof(dep), &len);
        SQLBindCol(db.serverhstmt, 4, SQL_C_CHAR, (void*)type, sizeof(type), &len);
        while (db.stmt_fetch_data())
        {
            Card new_card;
            new_card.cno.assign((const char *)cno);
            new_card.name.assign((const char*)name);
            new_card.dep.assign((const char*)dep);
            new_card.type.assign((const char*)type);
            ret.push_back(new_card);
        }
    }
    db.free_stmt_handle();
    return ret;
}

string Library::borrow_book(string cno, string bno)
{
	bool cno_exists = false;
    bool bno_exists = false;
    bool bno_have_stock=false;
	bool cno_bno_exists = false;

	//check if the cno exists--------------------------------------------------------
	string stmt = "select * from card where cno = '" + cno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
		cno_exists = true;
	}
	db.free_stmt_handle();
	if (!cno_exists)
	{
        return "2";
	}

    //check if the bno exists --------------------------------------------------------
    stmt = "select * from book where bno = '" + bno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
        bno_exists = true;
	}
	db.free_stmt_handle();
    if (!bno_exists)
	{
        return "3";
	}

	//check if the cno has borrowed bno (the same card is not allowed to borrow the same book more than once)--------------------
	stmt = "select * from borrow where cno = '" + cno + "' and bno = '" + bno + "';";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
		cno_bno_exists = true;
	}
	db.free_stmt_handle();

	if (cno_bno_exists)
	{
        return "4";
	}

    //check if the bno exists and stock is positive-------------------------------------
    stmt = "select * from book where bno = '" + bno + "' and stock > 0;";
    db.alloc_stmt_handle();
    db.execute_stmt(stmt);
    //read
    if (db.stmt_fetch_data())
    {
        bno_have_stock = true;
    }
    db.free_stmt_handle();
    if (!bno_have_stock)
    {
        stmt="select min(return_date) from borrow where bno = '" + bno + "';";
        db.alloc_stmt_handle();
        db.execute_stmt(stmt);
        //read
        SQLCHAR return_date[BUFFER_SIZE] = { 0 };
        SQLINTEGER len;
        if (db.stmt_fetch_data())
        {
            SQLGetData(db.serverhstmt, 1, SQL_C_CHAR, (void*)return_date, sizeof(return_date), &len);
        }
        db.free_stmt_handle();
        string ret_date_str((const char*)return_date);
        return ret_date_str;
    }

	//legal borrow--------------------------------------------------------
	//add tuple into borrow
    stmt = "insert into borrow values('" + bno + "', '" + cno + "', " + "DATE_SUB(curdate(),INTERVAL 0 DAY)" + ", " +
            "DATE_SUB(curdate(),INTERVAL "+to_string(BORROW_TIME)+ " DAY)" + ", '" + this->cur_adminID + "');";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();

	//update stock
	stmt = "update book set stock = stock - 1 where bno = " + bno + ";";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();
    return "1";
}

int Library::return_book(string cno, string bno)
{
	bool in_borrow = false;
	string stmt = "select * from borrow where cno = '" + cno + "' and bno = '" + bno + "'; ";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//read
	if (db.stmt_fetch_data())
	{
		in_borrow = true;
	}
	db.free_stmt_handle();

	if (!in_borrow)
	{
        return 2;
	}

	//delete borrow record
	stmt="delete from borrow where cno = '" + cno + "' and bno = '" + bno + "'; ";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();

	//update stock
	stmt = "update book set stock = stock + 1 where bno = " + bno + ";";
	db.alloc_stmt_handle();
	db.execute_stmt(stmt);
	//no read
	db.free_stmt_handle();
    return 1;
}

void Library::quit()
{
	db.free_connection();
}
