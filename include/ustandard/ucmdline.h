#ifndef __UCMDLINE_H__
#define __UCMDLINE_H__
__BEGIN_DECLS
int ucmdline_set(int argc, const char* argv[]);
const char* ucmdline_get_name(void);
int ucmdline_get_argc(void);
const char* ucmdline_get_argv(int idx);
__END_DECLS
#endif /* ucmdline.h */
