#pragma once
#include<string>
using namespace std;
class Book
{
public:
	string bno;
	string category;
	string title;
	string press;
	int year;
	string author;
	float price;
	int total;
	int stock;

    Book() :bno(""), category(""),title(""),press(""),year(-1),author(""),price(-1),total(-1),stock(-1) {}
	Book
	(string bno,
	string category,
	string title,
	string press,
	int year,
	string author,
	float price,
	int total,
	int stock) :
    bno(bno), category(category), title(title), press(press), year(year), author(author), price(price), total(total), stock(stock) {}
};
