#include <iostream>
#include <string.h>
#include "ustring.h"
#if 0
Ustring :: Ustring(void)
{
    cout << "construct 1" << endl;

    _cstr = new char[8];
    _cstr[0] = '\0';
}
#endif


Ustring :: Ustring(const char* s)
{
    cout << "construct 2" << endl;

    if(NULL == s) {
        s = "";
    }

    _cstr = strdup(s);
}


Ustring :: Ustring(const Ustring& us)
{
    cout << "construct copy" << endl;

    _cstr = strdup(us._cstr);
}


Ustring :: Ustring(const Ustring* us)
{
    cout << "construct pointer copy" << endl;

    _cstr = strdup(us->_cstr);
}


Ustring :: ~Ustring()
{
    cout << "destruct" << endl;

    delete[] _cstr;
}
