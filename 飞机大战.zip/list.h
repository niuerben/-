#include<stdlib.h>
#include<assert.h>

//链表结点的结构体
typedef struct NODE {
	int x;
	int y;
	int speed;
	struct NODE* next;
}Node;

//链表结构的结点
typedef struct LinkList{
	struct NODE* head;//头指针
	struct NODE* end;//尾指针
}LL;

//创建链表的结点函数
Node* Node_init(int x, int y, int speed) {
	Node* temp = (Node*)malloc(sizeof Node);
	assert(temp);
	temp->x = x;
	temp->y = y;
	temp->speed = speed;
	temp->next = NULL;
	return temp;
}


//单链表结点插入函数 尾插法
void LinkList_insert(LL* list, int x, int y, int speed) {
	if (list == NULL) {
		return;
	}
	if (list->head == NULL) {
		list->head = list->end = Node_init(x, y, speed);
	}
	else {
		list->end->next = Node_init(x, y, speed);
		list->end = list->end->next;//更新尾指针
	}
}