using namespace std;
class Ustring {
    public:
        Ustring(void);
        Ustring(const char* s);
        Ustring(const Ustring& us);
        Ustring(const Ustring* us);
        ~Ustring();

    private:
        char* _cstr;
};

