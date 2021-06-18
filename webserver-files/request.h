#ifndef __REQUEST_H__
#define __REQUEST_H__
#include <sys/time.h>

struct request_t{
    double arrival;
    double dispatch;
    int connfd;
};
typedef struct request_t request;

void requestHandle(request req);

#endif
