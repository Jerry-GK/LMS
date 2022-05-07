#pragma once
#include<string>
using namespace std;
class Card
{
public:
    string cno;
    string name;
    string dep;
    string type;

    Card() :cno(""), name(""),dep(""),type("") {}
    Card
    (string cno,
    string name,
    string dep,
    string type):
    cno(cno), name(name), dep(dep), type(type){}
};
