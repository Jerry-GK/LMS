#pragma once
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "Library.h"

//自定义主窗口
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    //构建主窗口界面
    MainWidget(Library* lib);
    ~MainWidget();

private slots://slot functions
    //goto other page
    void goto_log_in();
    void goto_main();
    void goto_add_book();
    void goto_query_book();
    void goto_card_option();
    void goto_borrow_book();
    void goto_return_book();
    void goto_add_card();
    void goto_delete_card();
    void goto_last();
    //do
    void do_log_in();
    void do_add_book();
    void do_query_book();
    void do_query_book_by_card_b();
    void do_query_book_by_card_r();
    void do_query_all_card();
    void do_borrow_book();
    void do_return_book();
    void do_add_card();
    void do_del_card();
    void set_order(int col);

private:
    //set layout for line edit
    void set_layout_log_in();
    void set_layout_query();
    void set_layout_borrow();
    void set_layout_return();
    void set_layout_add_card();
    void set_layout_del_card();
    void set_layout_add_book();
    //assisting functions
    void show_line_edit();
    void create_msg_box(string message);
    void set_order_str(string ord,int col);
    void hide_all();
    void clear_layout();

private:
    //universal items
    Library* plib;
    string query_order;
    int page_status;//0:log in, 1:main menu, 2:add book, 3:query_book, 4:card_option, 5:borrow book, 6:return book, 7:add_card, 8:delete card

    QTableWidget* table_book;
    QTableWidget* table_card;
    QPushButton* btn_return;
    QLabel* lab_msg;
    QFormLayout* layout_le;

    //0:log in
    QLineEdit* le_log_in_admID;
    QLineEdit* le_log_in_password;
    QPushButton* btn_log_in;
    QPushButton* btn_exit;

    //1:main menu
    QPushButton* btn_query_book;
    QPushButton* btn_add_book;
    QPushButton* btn_manage_card;
    QPushButton* btn_log_out;

    //2:add_book
    QLineEdit* le_add_book_bno;
    QLineEdit* le_add_book_category;
    QLineEdit* le_add_book_title;
    QLineEdit* le_add_book_press;
    QLineEdit* le_add_book_year;
    QLineEdit* le_add_book_author;
    QLineEdit* le_add_book_price;
    QLineEdit* le_add_book_num;
    QLineEdit* le_add_book_file;
    QPushButton* btn_do_add_book;

    //3:query book
    QLineEdit* le_query_bno;
    QLineEdit* le_query_category;
    QLineEdit* le_query_title;
    QLineEdit* le_query_press;
    QLineEdit* le_query_year_begin;
    QLineEdit* le_query_year_end;
    QLineEdit* le_query_author;
    QLineEdit* le_query_price_begin;
    QLineEdit* le_query_price_end;
    QPushButton* btn_do_query;

    //4:card option
    QPushButton* btn_borrow_book;
    QPushButton* btn_return_book;
    QPushButton* btn_add_card;
    QPushButton* btn_delete_card;

    //5:borrow book
    QLineEdit* le_borrow_cno;
    QLineEdit* le_borrow_bno;
    QPushButton* btn_do_borrow;

    //6:return book
    QLineEdit* le_return_cno;
    QLineEdit* le_return_bno;
    QPushButton* btn_do_return;

    //7:add card
    QLineEdit* le_add_card_cno;
    QLineEdit* le_add_card_name;
    QLineEdit* le_add_card_dep;
    QLineEdit* le_add_card_type;
    QPushButton* btn_do_add_card;

    //8:delete card
    QLineEdit* le_del_card_cno;
    QPushButton* btn_do_del_card;
};






