#define USVMESG_LEN_NAME            (128)
#define USVMESG_TYPE_HEADER         (0x1)
#define SIZE_INIT   (1024)
#define SIZE_ADDI   (1024)

struct umsgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */
};


struct usvmesg_header {
    long    mtype;

    size_t  size;
    char    sender[USVMESG_LEN_NAME];
    pid_t   pid;
};


static int _usvmesg_prepare_recv_buffer(struct umsgbuf** mbuf, size_t* size_mbuf, 
        size_t size)
{
    int ret = 0;

    if(size > *size_mbuf) {
        size_t size_realloc = sizeof(long) + size + SIZE_ADDI;
        void* tmp = um_realloc(*mbuf, size_realloc);
        if(NULL == tmp) {
            usloge_realloc(size_realloc);
            ret = -1;
        }
        else {
            *mbuf = tmp;
            *size_mbuf = size_realloc - sizeof(long);
        }
    }

    return ret;
}
