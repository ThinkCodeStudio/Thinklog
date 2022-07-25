#ifndef __NDLOG_H__
#define __NDLOG_H__

/**
 *函数名有问题 需要修改
 * 2022-07-25 liuxh
 *
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "ndPlatform.h"
    // #define RELEASE

    typedef enum
    {
        NDLOG_FAIL = -100,
        NDLOG_SUCCESS = 0,
    } NDLOG_CODE;

#ifdef RELEASE

#define NDLOG int
#define NDLOG_PRINT(log, ...)
#define NDLOG_LOG(function) rc = function
#define NDLOG_LOG_CODE(name, arg) name arg
#define NDLOG_SUCCESS(function) rc = function
#define NDLOG_SUCCESS_EXIT(function) rc = function
#define NDLOG_ERROR(function) rc = function
#define NDLOG_ERROR_EXIT(function) rc = function
#define NDLOG_GETCODE rc
#define NDLOG_SETCODE(code) rc = code
#define NDLOG_SETMESSAGE(msg, ...)
#define NDLOG_CREATE NDLOG rc
#define NDLOG_RETURN return rc
#define NDLOG_RETURN_FUNC(func) return func
#define NDLOG_STRING(log)
#define NDLOG_EXIT
#define PRINTF(format, ...)

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

#define FLAG "nd"
// #define SHOW_FILE __FILE__"}\r\n"
#define SHOW_FILE

/******* style_1
 * <file_path>}
 * #<code_line> | <app_flag>><log_flag>:[function_name]<msg>
 ******/
#define PRINT_FORMAT(f, fmt, ...) SHOW_FILE "#%-4d| " FLAG ">" f ":[%-30s]" fmt "\r\n", __LINE__, __FUNCTION__, ##__VA_ARGS__
/****** style_2
 * #<code_line> | <file_path> | <app_flag>><log_flag>:[function_name]<msg>
 ******/
// #define PRINT_FORMAT(f, fmt, ...) "#%-4d| %-80s | " FLAG ">" f ":[%-30s]" fmt "\r\n", __LINE__, __FILE__, __FUNCTION__, ##__VA_ARGS__
/****** style_3
 * <file_path>}
 * <app_flag>><log_flag>:[function_name]#<code_line><msg>
 ******/
// #define PRINT_FORMAT(f, fmt, ...) SHOW_FILE FLAG ">" f ":[%-30s]#%-4d" fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__

typedef struct
{
    const char *name;
    const char *file;
    unsigned int line;
    int code;
    char message[64];
} ndLog;

#define NDLOG_INIT                                 \
    {                                              \
        __FUNCTION__, __FILE__, __LINE__, 0, { 0 } \
    }

#define NDLOG_NULL              \
    {                           \
        NULL, NULL, 0, 0, { 0 } \
    }

#define NDLOG ndLog
//创建LOG对象
#define NDLOG_CREATE                             \
    NDLOG rc = NDLOG_INIT, results = NDLOG_NULL; \
    int rcode = 0
//返回数据
#define NDLOG_RETURN return rc
//将函数的返回值返回
#define NDLOG_RETURN_FUNC(function) return function
//获得返回码，if判断使用
#define NDLOG_GETCODE results.code
//设置返回码
#define NDLOG_SETCODE(v) rc.code = v
//设置返回信息
#define NDLOG_SETMESSAGE(msg, ...) sprintf(rc.message, msg, ##__VA_ARGS__)
//打印只输出文本
#define NDLOG_STRING(log)                  \
    PRINTF(PRINT_FORMAT("P", ":%s", log)); \
//打印输出消息
#define NDLOG_PRINT(log, ...) \
    PRINTF(PRINT_FORMAT("P", ":" log, ##__VA_ARGS__))
//解析返回数据并打印,正常输出
#define NDLOG_LOG(function) \
    results = function;     \
    PRINTF(PRINT_FORMAT("L", "<-[%s]#%-4d:%s. code=%X", results.name, results.line, results.message, results.code))
//适配返回int值的函数
#define NDLOG_LOG_CODE(function) \
    PRINTF(PRINT_FORMAT("L", "<-[%s]:code=%X", #function, function))
//检查函数是否失败
#define NDLOG_CHECK_RC(function)             \
    rcode = function;                        \
    if (rcode != 0)                          \
    {                                        \
        NDLOG_SETMESSAGE(#function " fail"); \
        NDLOG_SETCODE(rcode);                \
        goto exit;                           \
    }
//解析返回数据并打印,返回成功才打印
#define NDLOG_SUCCESS(function)        \
    results = function;                \
    if (results.code >= NDLOG_SUCCESS) \
    PRINTF(PRINT_FORMAT("S", "<-[%s]#%-4d:%s. code=%X", results.name, results.line, results.message, results.code))
//解析返回数据并打印,返回成功才打印，并跳转到退出代码
#define NDLOG_SUCCESS_EXIT(function)                                                                                     \
    results = function;                                                                                                  \
    if (results.code >= NDLOG_SUCCESS)                                                                                   \
    {                                                                                                                    \
        PRINTF(PRINT_FORMAT("S", "<-[%s]#%-4d:%s. code=%X", results.name, results.line, results.message, results.code)); \
        goto exit;                                                                                                       \
    }
//解析返回数据并打印,返回失败才打印
#define NDLOG_ERROR(function)         \
    results = function;               \
    if (results.code < NDLOG_SUCCESS) \
    PRINTF(PRINT_FORMAT("E", "<-[%s]#%-4d:%s. code=%X. in:%s", results.name, results.line, results.message, results.code, results.file))
//解析返回数据并打印,返回失败才打印,并跳转到退出代码
#define NDLOG_ERROR_EXIT(function)                                                                                                            \
    results = function;                                                                                                                       \
    if (results.code < NDLOG_SUCCESS)                                                                                                         \
    {                                                                                                                                         \
        PRINTF(PRINT_FORMAT("E", "<-[%s]#%-4d:%s. code=%X. in:%s", results.name, results.line, results.message, results.code, results.file)); \
        goto exit;                                                                                                                            \
    }
//跳转退出
#define NDLOG_EXIT \
    exit:
#endif

#ifdef __cplusplus
}
#endif

#endif