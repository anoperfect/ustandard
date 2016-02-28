#ifndef __UFILE_H__
#define __UFILE_H__
__BEGIN_DECLS

/*
    计算文件的长度.
    文件无法打开或者读取时, 返回0.
*/
long ufile_length(const char* filename);


/*
    文件全部内容读到ptr中. 最多读取长度size.
    返回实际读取的长度.
*/
size_t ufile_copy_to_buffer(const char* filename, void* ptr, size_t size);


/*
    在文件中插入内容. 插入的位置为index.
*/
int ufile_insert_content(const char* filename, long index, 
        void *ptr, size_t size, size_t nmemb);


/*
    向文件中写入buf内容,长度设置在size, 写方式按照mode指示的方式.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_write(const char* path, const char* mode, const void* buf, size_t size);


/*
    向文件中写入字符串str内容, 写方式按照mode指示的方式.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_write_string(const char* path, const char* mode, const char* str);


/*
    向文件中写入类printf变参内容, 写方式按照mode指示的方式.
    变参组合成的字符串总长度最大1024.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_writevs(const char* path, const char* mode, const char* fmt, ...);


/*
    文件不存在时,创建文件.
*/
int ufile_create(const char* path);


int ufile_global_write(const char* path, void* buf, size_t size);
int ufile_global_writevs(const char* path, const char* fmt, ...);
int ufile_global_close(void);


__END_DECLS
#endif//ufile.h


