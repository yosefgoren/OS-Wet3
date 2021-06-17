#ifndef __REQUEST_H__
#define __REQUEST_H__
#include <sys/time.h>

struct request_t{
    struct timeval arrival;
    struct timeval dispatch;
    int connfd;
};
typedef struct request_t request;

void requestHandle(request req);

#endif
