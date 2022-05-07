#include "MainWidget.h"
#include<QDebug>
using namespace std;

MainWidget::MainWidget(Library* plib)
{ 
    cout<<"\nGUI MSG: WIDGET GENERATE!"<<endl;
    //items in universal pages
    this->page_status=0;
    this->plib=plib;
    query_order="title";
    setWindowTitle("Library Management System");
    this->setGeometry(300,150,1350,700);    
    lab_msg=new QLabel(this);
    lab_msg->setGeometry(12,75,1000,400);
    lab_msg->setStyleSheet("QLabel{font:16px;color:blue;background-color:rgb(f9,f9,f9);}");
    lab_msg->show();
    layout_le = new QFormLayout;
    layout_le->setRowWrapPolicy(QFormLayout::WrapLongRows);
    this->setLayout(layout_le);
    btn_return=new QPushButton("返回",this);
    btn_return->setGeometry(1200,600,120,70);
    QObject::connect(btn_return,&QPushButton::clicked,this,&MainWidget::goto_last);

    table_book = new QTableWidget(this);
    table_book->setGeometry(13,290,1150,380);
    table_book->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_book->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_book->setColumnCount(9);
    table_book->setHorizontalHeaderLabels(QStringList() <<"书号"<< "类别"<<"书名"<<"出版社"<<"年份"<<"作者"<<"价格"<<"总藏书量"<<"库存");
    table_book->setRowCount(0);
    table_book->horizontalHeaderItem(2)->setText("书名(正序)");
    connect(table_book->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(set_order(int)));

    table_card = new QTableWidget(this);
    table_card->setGeometry(13,290,1150,380);
    table_card->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_card->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_card->setColumnCount(4);
    table_card->setHorizontalHeaderLabels(QStringList() <<"卡号"<< "姓名"<<"单位"<<"类别");
    table_card->setRowCount(0);

    //0:log in
    le_log_in_admID=new QLineEdit;
    le_log_in_password=new QLineEdit;
    le_log_in_admID->setPlaceholderText("请输入管理员ID");
    le_log_in_password->setPlaceholderText("请输入管理员密码");
    btn_log_in=new QPushButton("登录",this);
    btn_exit=new QPushButton("退出系统",this);
    btn_log_in->setGeometry(550,350,200,120);
    btn_exit->setGeometry(550,500,200,120);
    QObject::connect(btn_log_in,&QPushButton::clicked,this,&MainWidget::do_log_in);
    QObject::connect(btn_exit,&QPushButton::clicked,this,&MainWidget::close);

    //1:main menu
    btn_add_book=new QPushButton("图书入库",this);
    btn_add_book->setGeometry(550,50,200,120);
    btn_query_book=new QPushButton("图书查询",this);
    btn_query_book->setGeometry(550,200,200,120);
    btn_manage_card=new QPushButton("管理借书卡",this);
    btn_manage_card->setGeometry(550,350,200,120);
    btn_log_out=new QPushButton("退出登录",this);
    btn_log_out->setGeometry(550,500,200,120);
    QObject::connect(btn_log_out,&QPushButton::clicked,this,&MainWidget::goto_log_in);
    QObject::connect(btn_query_book,&QPushButton::clicked,this,&MainWidget::goto_query_book);
    QObject::connect(btn_add_book,&QPushButton::clicked,this,&MainWidget::goto_add_book);
    QObject::connect(btn_manage_card,&QPushButton::clicked,this,&MainWidget::goto_card_option);

    //2:add book
    le_add_book_bno=new QLineEdit;
    le_add_book_category=new QLineEdit;
    le_add_book_title=new QLineEdit;
    le_add_book_press=new QLineEdit;
    le_add_book_year=new QLineEdit;
    le_add_book_author=new QLineEdit;
    le_add_book_price=new QLineEdit;
    le_add_book_num=new QLineEdit;
    le_add_book_file=new QLineEdit;
    le_add_book_bno->setPlaceholderText("请输入待添加书籍的书号（若该书已存在，则以下信息只需填写新增<数量>即可，将自动忽略其他信息）");
    le_add_book_category->setPlaceholderText("请输入待添加书籍的类别");
    le_add_book_title->setPlaceholderText("请输入待添加书籍的书名");
    le_add_book_press->setPlaceholderText("请输入待添加书籍的出版社");
    le_add_book_year->setPlaceholderText("请输入待添加书籍的年份");
    le_add_book_author->setPlaceholderText("请输入待添加书籍的作者名");
    le_add_book_price->setPlaceholderText("请输入待添加书籍的价格");
    le_add_book_num->setPlaceholderText("请输入待添加书籍的添加数量");
    le_add_book_file->setPlaceholderText("如果要通过文件添加书籍，请在此处填入文件名(在程序文件夹中，要包含后缀.txt)。填入此项后将忽略以上所有非文件方式添加的信息！");
    btn_do_add_book=new QPushButton("添加书籍",this);
    btn_do_add_book->setGeometry(1200,500,120,70);
    QObject::connect(btn_do_add_book,&QPushButton::clicked,this,&MainWidget::do_add_book);

    //3:query_book
    string hint_message="无输入将不作为筛选条件";
    le_query_bno = new QLineEdit;
    le_query_category = new QLineEdit;
    le_query_title = new QLineEdit;
    le_query_press = new QLineEdit;
    le_query_year_begin = new QLineEdit;
    le_query_year_end = new QLineEdit;
    le_query_author = new QLineEdit;
    le_query_price_begin = new QLineEdit;
    le_query_price_end = new QLineEdit;
    le_query_bno->setPlaceholderText(hint_message.c_str());
    le_query_category->setPlaceholderText(hint_message.c_str());
    le_query_title->setPlaceholderText(hint_message.c_str());
    le_query_press->setPlaceholderText(hint_message.c_str());
    le_query_year_begin->setPlaceholderText(hint_message.c_str());
    le_query_year_end->setPlaceholderText(hint_message.c_str());
    le_query_author->setPlaceholderText(hint_message.c_str());
    le_query_price_begin->setPlaceholderText(hint_message.c_str());
    le_query_price_end->setPlaceholderText(hint_message.c_str());
    QObject::connect(le_query_bno,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_category,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_title,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_press,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_year_begin,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_year_end,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_author,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_price_begin,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    QObject::connect(le_query_price_end,&QLineEdit::textEdited,this,&MainWidget::do_query_book);
    btn_do_query=new QPushButton("查询",this);
    btn_do_query->setGeometry(1200,500,120,70);
    QObject::connect(btn_do_query,&QPushButton::clicked,this,&MainWidget::do_query_book);

    //4:card_option
    btn_borrow_book=new QPushButton("借书",this);
    btn_borrow_book->setGeometry(550,50,200,120);
    btn_return_book=new QPushButton("还书",this);
    btn_return_book->setGeometry(550,200,200,120);
    btn_add_card=new QPushButton("增加借书卡",this);
    btn_add_card->setGeometry(550,350,200,120);
    btn_delete_card=new QPushButton("删除借书卡",this);
    btn_delete_card->setGeometry(550,500,200,120);
    QObject::connect(btn_borrow_book,&QPushButton::clicked,this,&MainWidget::goto_borrow_book);
    QObject::connect(btn_return_book,&QPushButton::clicked,this,&MainWidget::goto_return_book);
    QObject::connect(btn_add_card,&QPushButton::clicked,this,&MainWidget::goto_add_card);
    QObject::connect(btn_delete_card,&QPushButton::clicked,this,&MainWidget::goto_delete_card);

    //5:borrow book
    le_borrow_cno=new QLineEdit;
    le_borrow_bno=new QLineEdit;
    le_borrow_cno->setPlaceholderText("请先输入卡号，将自动查找该卡的已借书籍");
    le_borrow_bno->setPlaceholderText("再输入想要借阅书籍的书号（不可重复借阅相同书籍）");
    btn_do_borrow=new QPushButton("借书",this);
    btn_do_borrow->setGeometry(1200,500,120,70);
    QObject::connect(le_borrow_cno,&QLineEdit::textEdited,this,&MainWidget::do_query_book_by_card_b);
    QObject::connect(btn_do_borrow,&QPushButton::clicked,this,&MainWidget::do_borrow_book);

    //6:return book
    le_return_cno=new QLineEdit;
    le_return_bno=new QLineEdit;
    le_return_cno->setPlaceholderText("请先输入卡号，将自动查找该卡的已借书籍");
    le_return_bno->setPlaceholderText("再输入想要归还书籍的书号（只能归还已借阅的书籍）");
    btn_do_return=new QPushButton("还书",this);
    btn_do_return->setGeometry(1200,500,120,70);
    QObject::connect(le_return_cno,&QLineEdit::textEdited,this,&MainWidget::do_query_book_by_card_r);
    QObject::connect(btn_do_return,&QPushButton::clicked,this,&MainWidget::do_return_book);

    //7:add_card
    le_add_card_cno=new QLineEdit;
    le_add_card_name=new QLineEdit;
    le_add_card_dep=new QLineEdit;
    le_add_card_type=new QLineEdit;
    le_add_card_cno->setPlaceholderText("请输入新借书卡的卡号（不可重复）");
    le_add_card_name->setPlaceholderText("请输入新借书卡的卡主姓名");
    le_add_card_dep->setPlaceholderText("请输入新借书卡的卡主单位");
    le_add_card_type->setPlaceholderText("请输入新借书卡的卡主类别（S/T）");
    btn_do_add_card=new QPushButton("添加借书卡",this);
    btn_do_add_card->setGeometry(1200,500,120,70);
    QObject::connect(btn_do_add_card,&QPushButton::clicked,this,&MainWidget::do_add_card);

    //8:delete card
    le_del_card_cno=new QLineEdit;
    le_del_card_cno->setPlaceholderText("请输入待删除借书卡的卡号（必须存在）");
    btn_do_del_card=new QPushButton("删除借书卡",this);
    btn_do_del_card->setGeometry(1200,500,120,70);
    QObject::connect(btn_do_del_card,&QPushButton::clicked,this,&MainWidget::do_del_card);

    this->goto_log_in();
}

//goto
void MainWidget::goto_query_book()
{
    page_status=3;
    hide_all();
    lab_msg->setText("书籍查询结果（可点击表头改变排序方式，最多显示50条）：");
    lab_msg->show();
    set_layout_query();
    btn_return->show();
    btn_do_query->show();
    table_book->show();
    this->do_query_book();
}

void MainWidget::goto_add_book()
{
    page_status=2;
    hide_all();
    btn_return->show();
    btn_do_add_book->show();
    set_layout_add_book();
}

void MainWidget::goto_card_option()
{
    page_status=4;
    hide_all();
    btn_return->show();
    btn_borrow_book->show();
    btn_return_book->show();
    btn_add_card->show();
    btn_delete_card->show();
}

void MainWidget::goto_log_in()
{
    page_status=0;
    hide_all();
    btn_log_in->show();
    btn_exit->show();
    set_layout_log_in();
    if(plib->cur_adminID!="_NONE")
        plib->adm_log_out();
}

void MainWidget::goto_main()
{
    page_status=1;
    hide_all();
    btn_add_book->show();
    btn_query_book->show();
    btn_manage_card->show();
    btn_log_out->show();
}

void MainWidget::goto_borrow_book()
{
    page_status=5;
    hide_all();
    lab_msg->setText("该借书卡所借的书籍：");
    lab_msg->show();
    set_layout_borrow();
    table_book->show();
    btn_return->show();
    btn_do_borrow->show();
}

void MainWidget::goto_return_book()
{
    page_status=6;
    hide_all();
    lab_msg->setText("该借书卡所借的书籍：");
    lab_msg->show();
    set_layout_return();
    table_book->show();
    btn_return->show();
    btn_do_return->show();
}

void MainWidget::goto_add_card()
{
    page_status=7;
    hide_all();
    lab_msg->setText("所有借书卡信息：");
    lab_msg->show();
    set_layout_add_card();
    table_card->show();
    btn_return->show();
    btn_do_add_card->show();
    do_query_all_card();
}

void MainWidget::goto_last()
{
    switch(page_status)
    {
        case 2:
        case 3:
        case 4:
            goto_main();break;
        case 5:
        case 6:
        case 7:
        case 8:
            goto_card_option();break;
        default:break;
    }
}

void MainWidget::goto_delete_card()
{
    page_status=8;
    hide_all();
    lab_msg->setText("所有借书卡信息：");
    lab_msg->show();
    set_layout_del_card();
    table_card->show();
    btn_return->show();
    btn_do_del_card->show();
    do_query_all_card();
}

//do
void MainWidget::do_query_book()
{
    table_book->setRowCount(0);
    string bno=le_query_bno->text().toStdString();
    string category=le_query_category->text().toStdString();
    string title=le_query_title->text().toStdString();
    string press=le_query_press->text().toStdString();
    int year_begin=atoi(le_query_year_begin->text().toStdString().c_str());
    int year_end=atoi(le_query_year_end->text().toStdString().c_str());
    string author=le_query_author->text().toStdString();
    float price_begin=atof(le_query_price_begin->text().toStdString().c_str());
    float price_end=atof(le_query_price_end->text().toStdString().c_str());

    if(year_begin==0) year_begin=-1;
    if(year_end==0) year_end=-1;
    if(price_begin==0) price_begin=-1;
    if(price_end==0) price_end=-1;

    vector<Book> books;
    books=plib->query_book(bno,category,title,press,year_begin,year_end,author,price_begin,price_end,this->query_order);

    for(vector<Book>::iterator it=books.begin();it!=books.end();it++)
    {
        table_book->setRowCount(table_book->rowCount()+1);
        table_book->setItem(table_book->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(it->bno)));
        table_book->setItem(table_book->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(it->category)));
        table_book->setItem(table_book->rowCount()-1,2,new QTableWidgetItem(QString::fromStdString(it->title)));
        table_book->setItem(table_book->rowCount()-1,3,new QTableWidgetItem(QString::fromStdString(it->press)));
        table_book->setItem(table_book->rowCount()-1,4,new QTableWidgetItem(QString::fromStdString(to_string(it->year))));
        table_book->setItem(table_book->rowCount()-1,5,new QTableWidgetItem(QString::fromStdString(it->author)));
        table_book->setItem(table_book->rowCount()-1,6,new QTableWidgetItem(QString::fromStdString(to_string(it->price))));
        table_book->setItem(table_book->rowCount()-1,7,new QTableWidgetItem(QString::fromStdString(to_string(it->total))));
        table_book->setItem(table_book->rowCount()-1,8,new QTableWidgetItem(QString::fromStdString(to_string(it->stock))));
    }  
}

void MainWidget::do_log_in()
{
    int ret=plib->adm_log_in(le_log_in_admID->text().toStdString(),le_log_in_password->text().toStdString());
    if(ret==1)
    {
        goto_main();
    }
    else if(ret==2)
        create_msg_box("管理员ID不存在！");
    else if(ret==3)
        create_msg_box("密码错误！");
}

void MainWidget::do_query_book_by_card_b()
{
    table_book->setRowCount(0);
    string cno=le_borrow_cno->text().toStdString();

    vector<Book> books;
    books=plib->query_book_by_card(cno);

    for(vector<Book>::iterator it=books.begin();it!=books.end();it++)
    {
        table_book->setRowCount(table_book->rowCount()+1);
        table_book->setItem(table_book->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(it->bno)));
        table_book->setItem(table_book->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(it->category)));
        table_book->setItem(table_book->rowCount()-1,2,new QTableWidgetItem(QString::fromStdString(it->title)));
        table_book->setItem(table_book->rowCount()-1,3,new QTableWidgetItem(QString::fromStdString(it->press)));
        table_book->setItem(table_book->rowCount()-1,4,new QTableWidgetItem(QString::fromStdString(to_string(it->year))));
        table_book->setItem(table_book->rowCount()-1,5,new QTableWidgetItem(QString::fromStdString(it->author)));
        table_book->setItem(table_book->rowCount()-1,6,new QTableWidgetItem(QString::fromStdString(to_string(it->price))));
        table_book->setItem(table_book->rowCount()-1,7,new QTableWidgetItem(QString::fromStdString(to_string(it->total))));
        table_book->setItem(table_book->rowCount()-1,8,new QTableWidgetItem(QString::fromStdString(to_string(it->stock))));
    }
}

void MainWidget::do_query_book_by_card_r()
{
    table_book->setRowCount(0);
    string cno=le_return_cno->text().toStdString();

    vector<Book> books;
    books=plib->query_book_by_card(cno);

    for(vector<Book>::iterator it=books.begin();it!=books.end();it++)
    {
        table_book->setRowCount(table_book->rowCount()+1);
        table_book->setItem(table_book->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(it->bno)));
        table_book->setItem(table_book->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(it->category)));
        table_book->setItem(table_book->rowCount()-1,2,new QTableWidgetItem(QString::fromStdString(it->title)));
        table_book->setItem(table_book->rowCount()-1,3,new QTableWidgetItem(QString::fromStdString(it->press)));
        table_book->setItem(table_book->rowCount()-1,4,new QTableWidgetItem(QString::fromStdString(to_string(it->year))));
        table_book->setItem(table_book->rowCount()-1,5,new QTableWidgetItem(QString::fromStdString(it->author)));
        table_book->setItem(table_book->rowCount()-1,6,new QTableWidgetItem(QString::fromStdString(to_string(it->price))));
        table_book->setItem(table_book->rowCount()-1,7,new QTableWidgetItem(QString::fromStdString(to_string(it->total))));
        table_book->setItem(table_book->rowCount()-1,8,new QTableWidgetItem(QString::fromStdString(to_string(it->stock))));
    }
}

void MainWidget::do_add_book()
{
    string filename=le_add_book_file->text().toStdString();
    if(filename!="")//add by file
    {
        int ret=plib->add_books_by_file(filename);
        if(ret==1)
            create_msg_box("通过文件添加书籍成功！");
        else if(ret==2)
            create_msg_box("输入的文件无法打开，请检查是否有后缀名txt以及doc文件夹下是否有该文件！");
        else if(ret==3)
            create_msg_box("文件内部格式有误，添加中途退出！");
        return;
    }

    //add by input
    int ret = plib->add_book(
                        le_add_book_bno->text().toStdString(),
                        le_add_book_category->text().toStdString(),
                        le_add_book_title->text().toStdString(),
                        le_add_book_press->text().toStdString(),
                        atoi(le_add_book_year->text().toStdString().c_str()),
                        le_add_book_author->text().toStdString(),
                        atof(le_add_book_price->text().toStdString().c_str()),
                        atoi(le_add_book_num->text().toStdString().c_str())
                    );
    if(ret==1)
        create_msg_box("通过输入添加书籍成功！");
    else if(ret==2)
        create_msg_box("书号不能为空，数量不能不填或为非正数！");
    else if(ret==3)
        create_msg_box("该书为新书，您输入的书籍信息不全！");
}

void MainWidget::do_borrow_book()
{
    string ret=plib->borrow_book(le_borrow_cno->text().toStdString(),le_borrow_bno->text().toStdString());
    if(ret=="1")
        create_msg_box("借书成功！");
    else if(ret=="2")
        create_msg_box("不存在该借书卡！");
    else if(ret=="3")
        create_msg_box("不存在该书！");
    else if(ret=="4")
        create_msg_box("不可多次借阅同一本书！");
    else
        create_msg_box("该书库存为零，预计最快归还日期：" + ret);
    do_query_book_by_card_b();
}

void MainWidget::do_return_book()
{
    int ret=plib->return_book(le_return_cno->text().toStdString(),le_return_bno->text().toStdString());
    if(ret==1)
        create_msg_box("还书成功！");
    else if(ret==2)
        create_msg_box("没有符合条件的借阅记录！");
    do_query_book_by_card_r();
}

void MainWidget::do_add_card()
{
    int ret=plib->add_card(le_add_card_cno->text().toStdString(),le_add_card_name->text().toStdString(),
                           le_add_card_dep->text().toStdString(),le_add_card_type->text().toStdString());
    if(ret==1)
        create_msg_box("添加借书卡成功！");
    else if(ret==2)
        create_msg_box("添加借书卡失败，可能已有该卡号！");
    else if(ret==3)
        create_msg_box("输入的借书卡信息不全！");
    do_query_all_card();
}

void MainWidget::do_del_card()
{
    int ret=plib->delete_card(le_del_card_cno->text().toStdString());
    if(ret==1)
        create_msg_box("删除借书卡成功！");
    else if(ret==2)
        create_msg_box("删除借书卡失败，可能不存在该卡号！");
    else if(ret==3)
        create_msg_box("删除借书卡失败，可能该卡还有未归还的书！");
    do_query_all_card();
}

void MainWidget::do_query_all_card()
{
    table_card->setRowCount(0);
    vector<Card> cards;
    cards=plib->query_all_card();
    for(vector<Card>::iterator it=cards.begin();it!=cards.end();it++)
    {
        table_card->setRowCount(table_card->rowCount()+1);
        table_card->setItem(table_card->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(it->cno)));
        table_card->setItem(table_card->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(it->name)));
        table_card->setItem(table_card->rowCount()-1,2,new QTableWidgetItem(QString::fromStdString(it->dep)));
        table_card->setItem(table_card->rowCount()-1,3,new QTableWidgetItem(QString::fromStdString(it->type)));
    }
}

//set layout for line edit
void MainWidget::set_layout_log_in()
{
    clear_layout();
    layout_le->addRow("管理员ID：",le_log_in_admID);
    layout_le->addRow("密码：",le_log_in_password);
    layout_le->setSpacing(80);
    show_line_edit();
}

void MainWidget::set_layout_query()
{
    clear_layout();
    layout_le->addRow("书号：",le_query_bno);
    layout_le->addRow("类别：",le_query_category);
    layout_le->addRow("书名：",le_query_title);
    layout_le->addRow("出版社：",le_query_press);
    layout_le->addRow("年份起点：",le_query_year_begin);
    layout_le->addRow("年份终点：",le_query_year_end);
    layout_le->addRow("作者：",le_query_author);
    layout_le->addRow("价格起点：",le_query_price_begin);
    layout_le->addRow("价格终点：",le_query_price_end);
    layout_le->setSpacing(7);
    show_line_edit();
}

void MainWidget::set_layout_borrow()
{
    clear_layout();
    layout_le->addRow("卡号：",le_borrow_cno);
    layout_le->addRow("书号：",le_borrow_bno);
    layout_le->setSpacing(30);
    show_line_edit();
}

void MainWidget::set_layout_return()
{
    clear_layout();
    layout_le->addRow("卡号：",le_return_cno);
    layout_le->addRow("书号：",le_return_bno);
    layout_le->setSpacing(30);
    show_line_edit();
}

void MainWidget::set_layout_add_card()
{
    clear_layout();
    layout_le->addRow("卡号：",le_add_card_cno);
    layout_le->addRow("姓名：",le_add_card_name);
    layout_le->addRow("单位：",le_add_card_dep);
    layout_le->addRow("类别：",le_add_card_type);
    layout_le->setSpacing(20);
    show_line_edit();
}

void MainWidget::set_layout_del_card()
{
    clear_layout();
    layout_le->addRow("待删除的卡号：",le_del_card_cno);
    layout_le->setSpacing(20);
    show_line_edit();
}

void MainWidget::set_layout_add_book()
{
    clear_layout();

    layout_le->addRow("书号：",le_add_book_bno);
    layout_le->addRow("类别：",le_add_book_category);
    layout_le->addRow("书名：",le_add_book_title);
    layout_le->addRow("出版社：",le_add_book_press);
    layout_le->addRow("年份：",le_add_book_year);
    layout_le->addRow("作者：",le_add_book_author);
    layout_le->addRow("价格：",le_add_book_price);
    layout_le->addRow("数量：",le_add_book_num);
    layout_le->addRow("<文件方式>文件名：",le_add_book_file);
    layout_le->setSpacing(10);
    show_line_edit();
}

//assisting functuons
void MainWidget::show_line_edit()
{
    for (int i = 0; i < layout_le->count(); ++i) {
          QWidget* w =  layout_le->itemAt(i)->widget();
          if (w != NULL)
              w->show();
      }
}

void MainWidget::create_msg_box(string message)
{
    QMessageBox MBox;
    MBox.setWindowTitle("系统提示信息");
    MBox.setText(QString::fromStdString(message));
    MBox.setFixedSize(100,100);
    MBox.exec();
}

void MainWidget::set_order_str(string ord,int col)
{
    if(query_order==ord)
    {
        query_order+=" desc";
        table_book->horizontalHeaderItem(col)->setText(table_book->horizontalHeaderItem(col)->text()+"(倒序)");
    }
    else
    {
        query_order=ord;
        table_book->horizontalHeaderItem(col)->setText(table_book->horizontalHeaderItem(col)->text()+"(正序)");
    }
}

void MainWidget::set_order(int col)
{
    table_book->setHorizontalHeaderLabels(QStringList() <<"书号"<< "类别"<<"书名"<<"出版社"<<"年份"<<"作者"<<"价格"<<"总藏书量"<<"库存");

    switch(col)
    {
        case 0:set_order_str("bno",col);break;
        case 1:set_order_str("category",col);break;
        case 2:set_order_str("title",col);break;
        case 3:set_order_str("press",col);break;
        case 4:set_order_str("year",col);break;
        case 5:set_order_str("author",col);break;
        case 6:set_order_str("price",col);break;
        case 7:set_order_str("total",col);break;
        case 8:set_order_str("stock",col);break;
        default:set_order_str("title",col);break;
    }
    this->do_query_book();
}

void MainWidget::hide_all()
{
    lab_msg->hide();
    btn_log_in->hide();
    btn_exit->hide();

    btn_add_book->hide();
    btn_do_add_book->hide();
    btn_query_book->hide();
    btn_manage_card->hide();
    btn_log_out->hide();
    btn_return->hide();
    btn_do_query->hide();
    table_book->hide();
    table_book->setRowCount(0);
    table_card->hide();
    table_card->setRowCount(0);
    btn_borrow_book->hide();
    btn_return_book->hide();
    btn_add_card->hide();
    btn_delete_card->hide();

    btn_do_borrow->hide();
    btn_do_return->hide();
    btn_do_add_card->hide();
    btn_do_del_card->hide();
    for (int i = 0; i < layout_le->count(); ++i) {
          QWidget* w =  layout_le->itemAt(i)->widget();
          w->hide();
      }
}

void MainWidget::clear_layout()
{
    delete layout_le;
    layout_le = new QFormLayout;
    layout_le->setRowWrapPolicy(QFormLayout::WrapLongRows);
    this->setLayout(layout_le);
}

MainWidget::~MainWidget()
{
    //free space
    delete table_book;
    delete table_card;
    delete btn_return;
    delete lab_msg;
    delete layout_le;
    delete le_log_in_admID;
    delete le_log_in_password;
    delete btn_log_in;
    delete btn_exit;
    delete btn_query_book;
    delete btn_add_book;
    delete btn_manage_card;
    delete btn_log_out;
    delete le_add_book_bno;
    delete le_add_book_category;
    delete le_add_book_title;
    delete le_add_book_press;
    delete le_add_book_year;
    delete le_add_book_author;
    delete le_add_book_price;
    delete le_add_book_num;
    delete le_add_book_file;
    delete btn_do_add_book;
    delete le_query_bno;
    delete le_query_category;
    delete le_query_title;
    delete le_query_press;
    delete le_query_year_begin;
    delete le_query_year_end;
    delete le_query_author;
    delete le_query_price_begin;
    delete le_query_price_end;
    delete btn_do_query;
    delete btn_borrow_book;
    delete btn_return_book;
    delete btn_add_card;
    delete btn_delete_card;
    delete le_borrow_cno;
    delete le_borrow_bno;
    delete btn_do_borrow;
    delete le_return_cno;
    delete le_return_bno;
    delete btn_do_return;
    delete le_add_card_cno;
    delete le_add_card_name;
    delete le_add_card_dep;
    delete le_add_card_type;
    delete btn_do_add_card;
    delete le_del_card_cno;
    delete btn_do_del_card;

    cout<<"\nGUI MSG: WIDGET EXIT!"<<endl;
}
