int ulog_init(int argc, char* argv[]);

int ulog_enable_level(const char* level, const char* output);
int ulog_disable_level(const char* level);

int ulog(const char* level, 
        const char* file, const char* function, unsigned long line, 
        const char* format, ...); 

#define ulogdbg(format...) ulog("debug", __FILE__, __FUNCTION__, __LINE__, format)
#define uloginf(format...) ulog("info",  __FILE__, __FUNCTION__, __LINE__, format)
#define ulogerr(format...) ulog("error", __FILE__, __FUNCTION__, __LINE__, format)
