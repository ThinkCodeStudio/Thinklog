#ifndef __THINKLOG_H__
#define __THINKLOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

// #define RELEASE


//显示方式:0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
//前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）
//背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）

//\033[0m 默认
//\033[1;32;40m 绿色
//033[1;31;40m 红色
//printf( "\033[1;31;40m 输出红色字符 \033[0m" );


typedef enum
{
    THINKLOG_CODE_FAIL = -100,
    THINKLOG_CODE_SUCCESS = 0,
} THINKLOG_CODE;

#ifdef RELEASE

#define THINKLOG int
#define THINKLOG_PRINT(log, ...)
#define THINKLOG_LOG(function) rc = function
#define THINKLOG_LOG_CODE(name,arg) name arg
#define THINKLOG_SUCCESS(function) rc = function
#define THINKLOG_SUCCESS_EXIT(function) rc = function
#define THINKLOG_ERROR(function) rc = function
#define THINKLOG_ERROR_EXIT(function) rc = function
#define THINKLOG_GETCODE rc
#define THINKLOG_SETRESULT(v, msg, ...) rc = v
#define THINKLOG_INIT THINKLOG rc
#define THINKLOG_RETURN return rc
#define THINKLOG_RETURN_FUNC(func) return func
#define PRINTF(format, ...)
#define THINKLOG_STRING(log)
#define THINKLOG_EXIT
#else

#include <string.h>
#include <stdio.h>

#if (defined __LINUX__) || (defined __HISI__)
#define PRINTF(format, ...) printf(format, ##__VA_ARGS__)
#endif
#ifdef __OPENLUAT__
#include "iot_debug.h"
#define PRINTF(format, ...) iot_debug_print(format, ##__VA_ARGS__)
#endif

#define FLAG "think"

typedef struct
{
    const char *name;
    const char *file;
    unsigned int line;
    int code;
    char message[64];
} Thinklog;

#define THINKLOG_CREATE                             \
    {                                              \
        __FUNCTION__, __FILE__, 0, 0, { 0 } \
    }

#define THINKLOG_NULL          \
    {                           \
        NULL, NULL, 0, 0, { 0 } \
    }

#define THINKLOG Thinklog
//创建LOG对象
#define THINKLOG_INIT THINKLOG rc = THINKLOG_CREATE, results = THINKLOG_NULL
//返回数据
#define THINKLOG_RETURN return rc
//将函数的返回值返回
#define THINKLOG_RETURN_FUNC(function) return function
//获得返回码，if判断使用
#define THINKLOG_GETCODE results.code
//设置返回
#define THINKLOG_SETRESULT(v, msg, ...) rc.line = __LINE__; sprintf(rc.message, msg, ##__VA_ARGS__); rc.code = v
//设置结果值,并且跳转到THINKLOG_EXIT
#define THINKLOG_SETRESULT_EXIT(v, msg, ...) rc.line = __LINE__; sprintf(rc.message, msg, ##__VA_ARGS__); rc.code = v; goto exit
//打印只输出文本
#define THINKLOG_STRING(str) \
    PRINTF(FLAG ">P:[%s]#%d:%s\r\n", __FUNCTION__, __LINE__, #str)
//打印输出消息
#define THINKLOG_PRINT(log, ...) \
    PRINTF(FLAG ">P:[%s]#%d:" log "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
//解析返回数据并打印,正常输出
#define THINKLOG_LOG(function) \
    results = function;         \
    PRINTF(FLAG ">L:[%s]#%d<-[%s]#%d:%s. code=%d\r\n", __FUNCTION__, __LINE__, results.name, results.line, results.message, results.code)
//适配返回int值的函数
#define THINKLOG_LOG_CODE(name,arg) \
    PRINTF(FLAG ">L:[%s]#%d:code=%d\r\n", __FUNCTION__, __LINE__, #name, name arg)
//解析返回数据并打印,返回成功才打印
#define THINKLOG_SUCCESS(function)        \
    results = function;                    \
    if (results.code >= THINKLOG_SUCCESS) \
    PRINTF(FLAG ">S:[%s]#%d<-[%s]#%d:%s. code=%d\r\n", __FUNCTION__, __LINE__, results.name, results.line, results.message, results.code)
//解析返回数据并打印,返回成功才打印，并跳转到退出代码
#define THINKLOG_SUCCESS_EXIT(function)                                                                                                       \
    results = function;                                                                                                                        \
    if (results.code >= THINKLOG_SUCCESS)                                                                                                     \
    {                                                                                                                                          \
        PRINTF(FLAG ">S:[%s]#%d<-[%s]#%d:%s. code=%d\r\n", __FUNCTION__, __LINE__, results.name, results.line, results.message, results.code); \
        goto exit;                                                                                                                             \
    }
//解析返回数据并打印,返回失败才打印
#define THINKLOG_ERROR(function)         \
    results = function;                   \
    if (results.code < THINKLOG_SUCCESS) \
    PRINTF(FLAG ">E:[%s]#%d<-[%s]#%d:%s. code=%d. in:%s\r\n", __FUNCTION__, __LINE__, results.name, results.line, results.message, results.code, results.file)
//解析返回数据并打印,返回失败才打印,并跳转到退出代码
#define THINKLOG_ERROR_EXIT(function)                                                                                                                              \
    results = function;                                                                                                                                             \
    if (results.code < THINKLOG_SUCCESS)                                                                                                                           \
    {                                                                                                                                                               \
        PRINTF(FLAG ">E:[%s]#%d<-[%s]#%d:%s. code=%d. in:%s\r\n", __FUNCTION__, __LINE__, results.name, results.line, results.message, results.code, results.file); \
        goto exit;                                                                                                                                                  \
    }
//跳转退出
#define THINKLOG_EXIT \
    exit:
#endif

#ifdef __cplusplus
}
#endif

#endif