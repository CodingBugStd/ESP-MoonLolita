#ifndef _LOLITA_APP_FRAME_H_
#define _LOLITA_APP_FRAME_H_

#include "stdio.h"

typedef enum{
    LLT_APP = 0,
    LLT_SERVICE = 1,
    LLT_MSG = 2,
    LLT_EVENT = 3,
    LLT_THREAD = 4,
    LLT_VARIABLE = 5,
    LLT_IO_CHANNEL = 6,
}llt_obj_type_t;

typedef struct{
    llt_obj_type_t  type;   //类型
    const char * name;      //名字
    uint8_t index;          //索引
    void * obj_real;
}llt_obj_t;

typedef void* (*llt_entrance_function_t)(void*);
typedef llt_entrance_function_t llt_app_entrance_t;
typedef llt_entrance_function_t llt_service_entrance_t;

typedef struct{
    llt_obj_t           father;
    llt_app_entrance_t  entrance;
    void *              entrance_args;
    void **             entrance_return;

}llt_app_t;

typedef enum{
    LLT_ERROR = 0,
    LLT_PASS = 1,
    LLT_TIMEOUT = 2,
    LLT_INVALID_OBJ = 3,
}llt_err_t;

typedef enum{
    LLT_FRAME_RUNNING_APP = 0,
    LLT_FRAME_RUNNING_CMD = 1,
    LLT_FRAME_ERROR = 2,
    LLT_FRAME_PAUSE = 3,
    LLT_FRAME_USER_IO = 4,
    LLT_FRAME_NONE = 5,
}llt_frame_state_t;

#endif  //_LOLITA_APP_FRAME_H

