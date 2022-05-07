#pragma once
#include<windows.h>
#include<iostream>
#include<fstream>
#include <assert.h>
#include<sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <vector>
#include <algorithm>
#include "DataBase.h"
#include "Book.h"
#include "Card.h"
#define BORROW_TIME 30//the maxium time interval someone can borrow a book
#define MAX_RETURN 50//max query return for books
#define BUFFER_SIZE 100

using namespace std;

class Library
{
private:
    DataBase db;//the database connected
public:
    string cur_adminID;//default to be "_NONE"

public:
    void Init();//Init the connecting database. Connect to the data source, use database lib, and create tables if not exists. Add admin if empty

    int adm_log_in(string adminID, string password);//admin log in,return 1:succeed, 2:ID not exists, 3:password error

    void adm_log_out();//admin log out

    int add_book(string bno, string category, string title, string press, int year, string author, float price, int num);//add book
    //return 1:succeed, 2:bno empty/num=0, 3:new book info not complete

    int add_books_by_file(string filename);//add books by the file
    //return 1:succeed, 2:can not open file, 3:book-adding error

	vector<Book> query_book(string bno, string category, string title, string press, int year_begin, int year_end, 
							string author, float price_begin, float price_end, string order);
	//query books, return the book vector
	//if a string parameter = "" or int/float parameter = -1, it means it's not a selection constrainst

    int add_card(string cno, string name, string department, string type);//add a card, return 1 if succeed, 2 if cno exists, 3 if info not complete

	int delete_card(string cno);//delete the card whose number is cno, return 1 if succeed, 2 if no such card

	vector<Book> query_book_by_card(string cno);//query the books that has been borrowed(not returned) 

    vector<Card> query_all_card();

    string borrow_book(string cno, string bno);//card cno borrow a book with number bno (only once per book allowed)
    //return "1":succeed, "2":No Card, "3":No Book, "4":Can not borrow twice, "<date format>"No stock and return the nearest return date

    int return_book(string bno, string cnp);//card cno return book bno
    //return 1:succeed, 2:No such borrow record, 3:Card cno has some book not returned yet

	void quit();
};
