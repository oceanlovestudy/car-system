#ifndef DATA_STRUCT_LIST
#define DATA_STRUCT_LIST

#include "driver_client_struct.h"
#include "passenger_client_struct.h"
#include "indent_struct.h"
#include "passenger_client_func.h"

//范型链表
//sizeof(struct List) = 8;即两个指针的大小
typedef struct List
{
    void* data;
    struct List* next;
}List;
/*
    函数功能：创建一个链表
    函数参数：节点信息data， 链表的头
    函数返回值：返回链表的头
   */
struct List* create_list(void* data, int length, struct List* head);

/*
    函数功能：查找节点信息
    函数参数：需要查找的节点信息data, 链表的头head
    函数返回值：返回查找到的信息
   */
struct List* search_list(void* n,void* p, struct List* head, int(*compare)(void* num, void* pass, void* number, void* password));

 /*
     函数功能：查找节点信息
     函数参数：
     1,head为泛型链表头指针
     2,data是需要查找值的引用
     3,compare一个函数指针,指向自定义的一个比较函数
     (1)参数1为关键值类型的引用,参数2为数据结构的引用
     (2)查找的值与数据结构引用中的成员值相等，返回真,否则假
     函数返回值：返回查找到的信息
  */
struct List* search_list_all(void* n, struct List* in, struct List* head, int(*compare)(void* num, void* number));

/*
    函数功能：删除节点信息
    函数参数：需要删除的节点信息data, 链表的头head
    函数返回值：返回删除后的链表头节点
   */
struct List* delete_list(void* data, struct List* head, int(*compare)(void* , void* ));

/*
    函数功能：显示链表信息
    函数参数：链表的头head
    函数返回值：void
   */
void show_list(struct List* head, void(*show)(void*));

/*
    函数功能：显示链表某个节点的信息
    函数参数：链表的某个节点的指针p,自定义的show函数
    函数返回值：void
   */
void show_node(struct List* p, void(*show)(void*));

/*
    函数功能：释放链表的空间
    函数参数：链表的头head
    函数返回值：void
   */
void free_list(struct List* head);
#endif
