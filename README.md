# Thinklog

> Thinklog是一个日志打印工具，它是一个宏定义头文件，没有实现文件。  
> Thinklog定义了一个标准的返回数据结构，通过简单的调用宏定义就可以方便的控制打印数据。  
> 通过在log头文件定义或在main文件的最顶部定义 `#define RELEASE` 发布模式，就可以关闭所有log功能，并替换成 `int` 类型，功能宏定义全部变为简单的赋值，删除所有打印信息，减小目标文件大小。
> Thinklog可以自动显示函数调用信息，方便查看调用关系

* 文件：Thinklog.h

**gcc编译时必须加-g参数日志才能正常使用**

# Version
* 2022-06-23 think 增强功能 开源
* 2022-05-14 think 完成

# 未来
* 能信息筛选的串口工具
* 解决 `__FILE__` 在不同编译器中内容不同如何显示的问题

# 使用

## 修改标识
```c
#define FLAG "think"
```
`FLAG` 宏定义是每个打印消息开头的内容，索引这个标识会排除掉其他打印的消息

## 适配
```c
#if (defined __LINUX__) || (defined __HISI__)
#define PRINTF(format, ...) printf(format, ##__VA_ARGS__)
#endif
#ifdef __OPENLUAT__
#include "iot_debug.h"
#define PRINTF(format, ...) iot_debug_print(format, ##__VA_ARGS__)
#endif
```
`PRINTF(format, ...)` 就是需要调用打印函数

## 发布模式
```c
// #define RELEASE
```
取消注释 `RELEASE` 定义将不会打印任何信息。

## 接口

| 定义 | 描述 |
| :---: |:---|
|THINKLOG| 类型，通常在返回类型定义中使用|
|THINKLOG_PRINT(log, ...)| 普通信息打印|
|THINKLOG_LOG(function)| 接收函数返回，打印返回值的内容|
|THINKLOG_LOG_CODE(function)| 接收函数返回，打印返回值的内容，适配返回int值的函数|
|THINKLOG_SUCCESS(function)| 返回的消息成功才打印|
|THINKLOG_SUCCESS_EXIT(function)| 返回的结果码成功才打印，并且跳转到THINKLOG_EXIT|
|THINKLOG_ERROR(function)| 返回的结果码失败才打印|
|THINKLOG_ERROR_EXIT(function)| 返回的结果码失败才打印，并且跳转到THINKLOG_EXIT|
|THINKLOG_GETCODE| 获得函数返回的结果码，常用于if判断|
|THINKLOG_SETRESULT(v, msg, ...)| 设置结果值，v：结果码，msg：返回消息，支持格式化|
|THINKLOG_SETRESULT_EXIT(v, msg, ...) |设置结果值,并且跳转到THINKLOG_EXIT|
|THINKLOG_INIT| 变量初始化，带 * 的不需要初始化|
|THINKLOG_RETURN| 返回结果值|
|THINKLOG_RETURN_FUNC(func)| 不做任何操作，直接返回函数的返回值|
|PRINTF(format, ...)| 打印输出函数适配|
|THINKLOG_STRING(str)| 只打印字符串|
|THINKLOG_EXIT| 跳转标志|

## 案例

```c

THINKLOG func(int a, int b, int *ret){
	THINKLOG_INIT;
	if(b == 0){
		THINKLOG_SETRESULT_EXIT(THINKLOG_CODE_FAIL, "%d不能为除数", b);
	}
	*ret = a / b;
	THINKLOG_SETRESULT(THINKLOG_CODE_SUCCESS, "成功");
THINKLOG_EXIT:
	THINKLOG_RETURN
}

int main(){
	THINKLOG_INIT;
	THINKLOG_STRING("main start");
	int a = 9, b = 3, c = 0;
    int *p = &c;
	THINKLOG_LOG(func(a, b, p));
	if(THINKLOG_GETCODE == THINKLOG_CODE_SUCCESS){
		THINKLOG_PRINT("%d", c);
	}S
	THINKLOG_STRING("main end");
	return 0;
}
```