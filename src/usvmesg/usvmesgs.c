#include "ustandard/ustandard.h"
#include "ustandard/udevelop.h"
#include "ustandard/ufile.h"
#include "usvmesg/usvmesg_public.inc.c"
#include "ustandard/usvmesgs.h"
#define USVMESGS_USLEEP             (5*1000)
#define MAX_SECOND  (1)
#define   PERM S_IRUSR|S_IWUSR
struct usvmesgs {
    int     msgid;
    struct  umsgbuf* mbuf_recv;
    size_t  size;
    size_t  size_init;
    char    sender[USVMESG_LEN_NAME];
};


static int _usvmesgs_clear(struct usvmesgs* msg_t);
static int _usvmesgs_cleanup(struct usvmesgs* msg_t);


__ptr_t usvmesgs_register(const char* name, size_t size)
{
    __ptr_t retu = 0;
    struct usvmesgs* msg_t = NULL;

    int ret = 0;
    char path[KLEN_PATH];
    do {
        msg_t = um_malloc(sizeof(*msg_t));
        if(NULL == msg_t) {
            usloge_malloc(sizeof(*msg_t));
            break;
        }

        _usvmesgs_clear(msg_t);

        msg_t->size = SIZE_INIT;
        msg_t->mbuf_recv = um_malloc(sizeof(long)+msg_t->size);
        if(NULL == msg_t->mbuf_recv) {
            usloge_malloc(sizeof(long)+msg_t->size);
            break;
        }
        msg_t->mbuf_recv->mtype = 0x123456;

        snprintf(path, KLEN_PATH, "/tmp/.ipcpath/%s", name);

        if(0 != access(path, F_OK)) {
            ret = ufile_create(path);
            if(0 != ret) {
                usloge_perror("ufile_create");
                break;
            }
        }

        int fd = open(path, O_RDWR , 0600);
        if(fd <= 0) {
            usloge_perror("open");
            break;
        }

        int lock_result = lockf(fd, F_TEST, 0);
        if(lock_result < 0) {
            usloge_perror("lockf");
            break;
        }

        lock_result = lockf(fd, F_LOCK, 0);
        if(lock_result < 0) {
            usloge_perror("lockf");
            break;
        }

        key_t key = ftok(path, 'x');
        if(-1 == key) {
            usloge_perror("ftok");
            break;
        }

        msg_t->msgid = msgget(key, PERM|IPC_CREAT);
        if(-1 == msg_t->msgid) {
            usloge_perror("msgget");
            break;
        }

        retu = (__ptr_t)msg_t;
    }while(0);

    if((__ptr_t)0 == retu) {
        _usvmesgs_cleanup(msg_t);
        msg_t = NULL;
    }

    return retu;
}


int usvmesgs_withdraw(__ptr_t id)
{
    int ret = 0;

    struct usvmesgs* msg_t = (struct usvmesgs*)id;
    _usvmesgs_cleanup(msg_t);
    msg_t = NULL;
    
    return ret;
}


int usvmesgs_recv(__ptr_t id, struct umesg_recv_data* data, struct timeval* to)
{
    int ret = 0;

    struct usvmesgs* msg_t = (struct usvmesgs*)id;
    long usec = 0;
    if(to!=NULL){
        usec = to->tv_sec*1000*1000+to->tv_usec;
    }
    else {
        usec = MAX_SECOND*1000*1000;
    }

    struct usvmesg_header header;
    while(1) {
        ssize_t size1 = msgrcv(msg_t->msgid, 
                &header, sizeof(struct usvmesg_header)-sizeof(long), 
                USVMESG_TYPE_HEADER, IPC_NOWAIT);
        if(-1 == size1) {
            int eno = errno;
            if(EAGAIN == eno || EINTR == eno) {
                ulogdbg("currently no data.\n"); 
                usleep(USVMESGS_USLEEP);     
                usec -= USVMESGS_USLEEP;

                if(usec < 0) {
                    ret = -1;
                    errno = EAGAIN;
                    break;
                }
                else {
                    continue;
                }
            }
            else {
                ret = -1;
                break;
            }
        }
        else if(sizeof(struct usvmesg_header)-sizeof(long) != size1) {
            ulogerr("read header error.\n");
            ret = -1;
            break;
        }

        ret = _usvmesg_prepare_recv_buffer(
                &msg_t->mbuf_recv, &msg_t->size, 
                header.size);
        if(0 != ret) {
            usloge_perror("_usvmesgs_prepare_recv_buffer");
            break;
        }

        ssize_t size2 = msgrcv(msg_t->msgid, 
                msg_t->mbuf_recv, header.size, header.pid, IPC_NOWAIT);
        if(size2 != header.size) {
            ulogerr("read content error.(ssize=%zd)\n", size2);
            ret = -1;
            break;
        }

        um_strcpy(msg_t->sender, USVMESG_LEN_NAME, header.sender);
        data->sender = msg_t->sender;
        data->ptr = msg_t->mbuf_recv->mtext;
        data->size = header.size;
        ulogdbg("usvmesgv recv : name = %s , length = %zd.\n", 
                data->sender, data->size);

        break;
    }
    
    return ret;
}


int _usvmesgs_clear(struct usvmesgs* msg_t)
{
    int ret = 0;

    if(NULL != msg_t) {
        msg_t->msgid        = -1;
        msg_t->mbuf_recv    = NULL;
        msg_t->size         = 0;
        msg_t->size_init    = SIZE_INIT;
    }

    return ret;
}


int _usvmesgs_cleanup(struct usvmesgs* msg_t)
{
    int ret = 0;

    if(NULL != msg_t) {
        if(NULL != msg_t->mbuf_recv) {
            um_free(msg_t->mbuf_recv);
            msg_t->mbuf_recv = NULL;
        }

        if(-1 != msg_t->msgid) {
            msgctl(msg_t->msgid, IPC_RMID, NULL);
            msg_t->msgid = -1;
        }

        _usvmesgs_clear(msg_t);

        um_free(msg_t);
        msg_t = NULL;
    }

    return ret;
}
