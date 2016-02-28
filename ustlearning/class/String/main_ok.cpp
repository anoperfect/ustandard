#include<iostream>
#include<string.h>
using namespace std;
 
class String{
public:
    String(const char* str=NULL);                //赋值构造兼默认构造函数(char)
    String(const String &other);                   //赋值构造函数(String)
    String& operator=(const String&other);         //operator= String operator+(const String &other)const;  //operator+
    bool operator==(const String&);              //operator==
    size_t size(){return strlen(m_data);};
    ~String(void) {delete[] m_data;}
private:
    char *m_data;
};
inline String::String(const char* str)
{
    if (!str) m_data=0;
    else
    {
        m_data = new char[strlen(str)+1];
        strcpy(m_data,str);
    }
}
inline String::String(const String& other)
{
    if(!other.m_data) m_data=0;
    else
    {
        m_data=new char[strlen(other.m_data)+1];
        strcpy(m_data,other.m_data);
    }
}
inline String& String::operator=(const String& other)
{
    if (this!=&other)
    {
        delete[] m_data;
        if(!other.m_data) m_data=0;
        else
        {
            m_data = new char[strlen(other.m_data)+1];
            strcpy(m_data,other.m_data);
        }
    }
    return *this;
}
inline bool String::operator==(const String &s)    
{
    if ( strlen(s.m_data) != strlen(m_data) )
        return false;
    return strcmp(m_data,s.m_data)?false:true;
}
int main()
{
    String s = "1";
    s = "456";
    return 0;
}
