#include "ustandard/ustandard.h"
#include "ustandard/udevelop.h"
#include "usvmesg/usvmesg_public.inc.c"
#include "ustandard/usvmesgc.h"
struct usvmesgc {
    int     msgid;
    struct  umsgbuf* mbuf_send;
    size_t  size;
    size_t  size_init;
};


static int _usvmesgc_init_by_name(struct usvmesgc* msg_send, const char* recver);
static int _usvmesgc_deinit(struct usvmesgc* msg_send);

int usvmesgc_send(const char* sender, const char* recver, 
        const void* p, size_t size)
{
    int ret = 0;
    struct usvmesgc msg_send;

    ret = _usvmesgc_init_by_name(&msg_send, recver);
    if(0 != ret) {
        usloge_perror("_usvmesgc_get_by_name");
        ret = -1;
        goto finished;
    }

    struct usvmesg_header header;
    header.mtype = USVMESG_TYPE_HEADER;
    header.size = size;
    um_strcpy(header.sender, USVMESG_LEN_NAME, sender);
    header.pid  = getpid();

    ret = msgsnd(msg_send.msgid, &header, sizeof(header)-sizeof(long), IPC_NOWAIT);
    if(0 != ret) {
        usloge_perror("send header");
        ret = -1;
        goto finished;
    }
    
    ret = _usvmesg_prepare_recv_buffer(&msg_send.mbuf_send, &msg_send.size, header.size);
    if(0 != ret) {
        usloge_perror("_usvmesgc_prepare_recv_buffer");
        ret = -1;
        goto finished;
    }

    msg_send.mbuf_send->mtype = header.pid;
    memcpy(msg_send.mbuf_send->mtext, p, size);
    ret = msgsnd(msg_send.msgid, msg_send.mbuf_send, size, IPC_NOWAIT);
    if(0 != ret) {
        ulogerr("msgid:%d, mtype:%ld, size:%u.\n", msg_send.msgid, msg_send.mbuf_send->mtype, size);
        usloge_perror("send content");
        ret = -1;
        goto finished;
    }

finished:     
    _usvmesgc_deinit(&msg_send);
    return ret;
}


int _usvmesgc_init_by_name(struct usvmesgc* msg_send, const char* recver)
{
    int ret = 0;

    msg_send->msgid         = -1;
    msg_send->mbuf_send     = NULL;
    msg_send->size          = 0;
    msg_send->size_init     = SIZE_INIT;

    ret = -1;
    char path[KLEN_PATH];
    do {
        msg_send->size = SIZE_INIT;
        msg_send->mbuf_send = um_malloc(sizeof(long)+msg_send->size);
        if(NULL == msg_send->mbuf_send) {
            usloge_malloc(sizeof(long)+msg_send->size);
            break;
        }

        snprintf(path, KLEN_PATH, "/tmp/.ipcpath/%s", recver);
        key_t key = ftok(path, 'x');
        if(-1 == key) {
            usloge_perror("ftok");
            break;
        }

        msg_send->msgid = msgget(key, 0600);
        if(-1 == msg_send->msgid) {
            usloge_perror("msgget");
            break;
        }

        ret = 0;
    }while(0);

    return ret;
}


int _usvmesgc_deinit(struct usvmesgc* msg_send)
{
    int ret = 0;

    if(NULL != msg_send->mbuf_send) {
        um_free(msg_send->mbuf_send);
        msg_send->mbuf_send = NULL;
    }

    if(-1 != msg_send->msgid) {

    }

    return ret;
}
