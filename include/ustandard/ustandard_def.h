#ifndef __USTANDARD_DEF_H__
#define __USTANDARD_DEF_H__
#include "ustandard_sys.h"
#include "ustandard/utypes.h"
#include "ustandard/ufunction.h"

#define LEN_LOG_STR 1024

#define __LOGOUT_DEBUG      stdout
#define __LOGOUT_INFO       stdout
#define __LOGOUT_ERROR      stderr




#define ulogdbg(fmt...)                                                         \
if(NULL != __LOGOUT_DEBUG) {                                                    \
    char s_prefix[LEN_LOG_STR];                                                 \
    snprintf(s_prefix, LEN_LOG_STR, "[%30s, %36s, %6d]%5s:",                    \
            __FILE__, __FUNCTION__, __LINE__, "debug");                         \
    char s_content[LEN_LOG_STR];                                                \
    snprintf(s_content, LEN_LOG_STR, fmt);                                      \
    if(NULL != __LOGOUT_DEBUG) {                                                \
        fprintf(__LOGOUT_DEBUG, "%s%s", s_prefix, s_content);                   \
    }                                                                           \
}


#define uloginf(fmt...)                                                         \
if(NULL != __LOGOUT_INFO) {                                                     \
    char s_prefix[LEN_LOG_STR];                                                 \
    snprintf(s_prefix, LEN_LOG_STR, "[%30s, %36s, %6d]%5s:",                    \
            __FILE__, __FUNCTION__, __LINE__, "info");                          \
    char s_content[LEN_LOG_STR];                                                \
    snprintf(s_content, LEN_LOG_STR, fmt);                                      \
    if(NULL != __LOGOUT_INFO) {                                                 \
        fprintf(__LOGOUT_INFO,  "%s%s", s_prefix, s_content);                   \
    }                                                                           \
}


#define ulogerr(fmt...)                                                         \
if(NULL != __LOGOUT_ERROR) {                                                    \
    char s_prefix[LEN_LOG_STR];                                                 \
    snprintf(s_prefix, LEN_LOG_STR, "[%30s, %36s, %6d]%5s:",                    \
            __FILE__, __FUNCTION__, __LINE__, "error");                         \
    char s_content[LEN_LOG_STR];                                                \
    snprintf(s_content, LEN_LOG_STR, fmt);                                      \
    if(NULL != __LOGOUT_ERROR) {                                                \
        fprintf(__LOGOUT_ERROR, "%s%s", s_prefix, s_content);                   \
    }                                                                           \
}

#endif /* ustandard_def.h */
