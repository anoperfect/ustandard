#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
 
class String{
    friend ostream& operator<< (ostream&,String&);
public:
    String(const char* str=NULL);                //赋值构造兼默认构造函数(char)
    String(const String &other);                   //赋值构造函数(String)
    String& operator=(const String&other);         //operator=
    String operator+(const String &other)const;  //operator+
    String operator+(const char* str);
    bool operator==(const String&);              //operator==
    char& operator[](unsigned int);                 //operator[]
    size_t size(){return strlen(m_data);};
    ~String(void) {printf("destruct\n");delete[] m_data;}
private:
    char *m_data;
};
inline String::String(const char* str)
{
    printf("Construct by cstr\n");
    if (!str) m_data=0;
    else
    {
        m_data = new char[strlen(str)+1];
        strcpy(m_data,str);
    }
}
inline String::String(const String& other)
{
    printf("Construct by copy\n");
    if(!other.m_data) m_data=0;
    else
    {
        m_data=new char[strlen(other.m_data)+1];
        strcpy(m_data,other.m_data);
    }
}
inline String& String::operator=(const String& other)
{
    printf("operator=\n");
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
inline String String::operator+(const String &other)const
{
    printf("operator+\n");
    String newString;
    if(!other.m_data)
        newString = *this;
    else if(!m_data)
        newString = other;
    else
    {
        newString.m_data = new char[strlen(m_data)+strlen(other.m_data)+1];
        strcpy(newString.m_data,m_data);
        strcat(newString.m_data,other.m_data);
    }
    return newString;
}
inline String String::operator+(const char* str)
{
    printf("operator+cstr\n");
    const char* tmp = m_data;
    String newString;
        newString.m_data = new char[strlen(m_data)+strlen(str)+1];
        m_data = NULL;
        strcpy(m_data,tmp);
        strcat(m_data,str);
        delete[] tmp;
    return *this;
}
inline bool String::operator==(const String &s)    
{
    if ( strlen(s.m_data) != strlen(m_data) )
        return false;
    return strcmp(m_data,s.m_data)?false:true;
}
inline char& String::operator[](unsigned int e)
{
    if (e>=0&&e<=strlen(m_data))
        return m_data[e];
}
ostream& operator<<(ostream& os,String& str)
{
    os << str.m_data;
    return os;
}
int main()
{
    String str = "a"; //String("a") + String("123");
    str = "xyx";
    return 0;
}
