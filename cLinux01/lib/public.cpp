#include "public.h"  // 包含自定义函数声明的头文件

//g++ -c -o libpublic.a public.cpp
//g++ -fPIC -shared -o libpublic.so public.cpp
 
void func()   // 自定义函数的实现
{
  printf("我心匪石，不可转也。我心匪席，不可卷也。威仪棣棣，不可选也。\n");
  //printf("生活美好如鲜花，不懂享受是傻瓜；\n");
  //printf("傻呀傻呀傻呀傻，比不上小鸟和乌鸦。\n");
  //printf("芳草地啊美如画，谁要不去是傻瓜；\n");
  //printf("我是一只傻傻鸟，独在枯枝丫上趴。\n");
}