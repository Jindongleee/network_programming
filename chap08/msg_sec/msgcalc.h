#ifndef MSGCALC_H

#define MSGCALC_H
#define REQUEST 0
#define RESPONSE 1
#define SUCCESS 0
#define FAIL 1

typedef struct _calcdata
{
    char type, flag, op;
    float op1, op2, result;
    /* data */
} calcdata_t;

typedef struct _msg{
    long type;
    calcdata_t data;
    struct sockaddr_in addr;
} msg_t;

#endif
