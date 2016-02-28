#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

class Ustring {
    public:
        Ustring(const char* s);
        Ustring(const Ustring& us);
        ~Ustring(void) {printf("delete %p\n", m_data); delete[] m_data;m_data=NULL;}

    //private:
        char* m_data;
};


Ustring :: Ustring(const char* s)
{
    cout << "construct 2" << endl;
    if(NULL == s) {
        s = "";
    }
    m_data = strdup(s);
    printf("m_data = %p\n", m_data);
}


Ustring :: Ustring(const Ustring& us)
{
    cout << "construct copy" << endl;
    m_data = strdup(us.m_data);
    printf("m_data = %p\n", m_data);
}


#if 0
Ustring :: ~Ustring()
{
    cout << "destruct" << endl;

    delete[] m_data;
}
#endif


void test01(void)
{
    Ustring us = "1";
    printf("us.m_data=%p\n", us.m_data);
    us = Ustring("123");
    printf("us.m_data=%p\n", us.m_data);
}




void test02(void)
{
    //Ustring* us1 = new Ustring();
    /* construct 1. */
}




int main()
{

    test01();
    //string s1 = "1";
    //s1 = "456";

    //Ustring* us2 = new Ustring("abc");

    //Ustring us_cp1 = *us1;
    //Ustring us_cp2 = *us2;

    //Ustring us_e1 = new Ustring("123");
    //Ustring us_e2 = new Ustring("123");
    //Ustring us_e2 = Ustring("123");
    //Ustring* tmp = new Ustring("123");
    //Ustring us_e2 = tmp;

    //Ustring tmp = Ustring("123");
    //Ustring us_e1 = tmp;
    //Ustring us1 = Ustring(new Ustring(Ustring("123")));

    return 0;
}
