#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../inc/passenger_client_struct.h"
#include "../inc/driver_client_struct.h"
#include "../inc/indent_struct.h"

#include "../inc/data_struct_list.h"

/*
   函数功能：创建一个链表
   函数参数：节点信息data， 链表的头
   函数返回值：返回链表的头
 */
struct List* create_list(void* data, int length, struct List* head)
{
    void* temp = malloc(length);
    assert(temp != NULL);
    memcpy(temp, data, length);
    struct List* p = malloc(sizeof(struct List));
    assert(p != NULL);
    p->data = temp;
    p->next = head;
    head = p;
    return head;
}

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
struct List* search_list(void* n, void* p, struct List* head, int(*compare)(void* num, void* pass, void* number, void* password))
{
    struct List* s = head;
    while(s != NULL)
    {
	char s_number[20];
	char s_password[20];
	char temp_p[40];
	int i, j;
	memcpy(s_number, s->data, sizeof(s_number));
	memcpy(temp_p, s->data, sizeof(temp_p));
	for(j = 0, i = 20; (i<40) && (j<20); i++, j++)
	{
	   s_password[j] = temp_p[i];  
	}
	s_password[j-1] = '\0';
	if(compare(n, p, s_number, s_password))
	{   
	    printf("查找成功！\n");
	    return s;
	}
	printf("查找失败！\n");
	s = s->next;
    }
    return NULL;
}

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
struct List* search_list_all(void* n, struct List* in, struct List* head, int(*compare)(void* num, void* number))
{
    int in_count = 0;
    struct List* s = head;
    while(s != NULL)
    {
	char s_number[20];
	char temp_p[24];
	int i, j;
	memcpy(temp_p, s->data, sizeof(temp_p));
	for(j = 0, i = 4; (i<24) && (j<20); i++, j++)
	{
	   s_number[j] = temp_p[i];  
	}
	s_number[j-1] = '\0';
	if(compare(n, s_number))
	{   
	    printf("插入%s历史订单数组中\n", s_number);
	    in = create_list(s->data, INDENT_SIZE, in);
	}
	s = s->next;
    }
    return in;
}
/*
   函数功能：删除节点信息
   函数参数：
   1,head为泛型链表头指针
   2,data是需要查找值的引用
   3,compare一个函数指针,指向自定义的一个比较函数
   (1)参数1为关键值类型的引用,参数2为数据结构的引用
   (2)查找的值与数据结构引用中的成员值相等，返回真,否则假
   函数返回值：返回删除后的链表头节点
 */
struct List* delete_list(void* data, struct List* head, int(*compare)(void*, void* ))
{
    struct List* s = NULL;
    struct List* p = head;
    while(head != NULL)
    {
	if(compare(data, head->data))
	{
	    s = head;
	    head = head->next;
	    free(s->data);
	    free(s);
	    return head;
	}
	while(p->next != NULL)
	{
	    if(compare(data, p->next->data))
	    {
		s = p->next;
		p->next = s->next;
		free(s->data);
		free(s);
		break;
	    }
	    p = p->next;
	}
    }
    return head;
}

/*
   函数功能：显示链表信息
   函数参数：链表的头head
   函数返回值：void
 */
void show_list(struct List* head, void(*show)(void*))
{
    struct List *p = head;
    while(p != NULL)
    {
	show(p->data);
	p = p->next;
    }
    return ;
}

/*
   函数功能：显示链表某个节点的信息
   函数参数：链表的某个节点的指针p,自定义的show函数
   函数返回值：void
 */
void show_node(struct List* p, void(*show)(void*))
{
    show(p->data);
    return ;
}


/*
   函数功能：释放链表的空间
   函数参数：链表的头head
   函数返回值：void
 */
void free_list(struct List* head)
{
    struct List* p = NULL;
    while(head != NULL)
    {
	p = head->next;
	//先释放数据成员申请的空间
	free(head->data);
	//再释放链表节点申请的空间
	free(head);
	head = p;
    }
    return ;
}

