#include<stdlib.h>
#include<assert.h>

//������Ľṹ��
typedef struct NODE {
	int x;
	int y;
	int speed;
	struct NODE* next;
}Node;

//����ṹ�Ľ��
typedef struct LinkList{
	struct NODE* head;//ͷָ��
	struct NODE* end;//βָ��
}LL;

//��������Ľ�㺯��
Node* Node_init(int x, int y, int speed) {
	Node* temp = (Node*)malloc(sizeof Node);
	assert(temp);
	temp->x = x;
	temp->y = y;
	temp->speed = speed;
	temp->next = NULL;
	return temp;
}


//����������뺯�� β�巨
void LinkList_insert(LL* list, int x, int y, int speed) {
	if (list == NULL) {
		return;
	}
	if (list->head == NULL) {
		list->head = list->end = Node_init(x, y, speed);
	}
	else {
		list->end->next = Node_init(x, y, speed);
		list->end = list->end->next;//����βָ��
	}
}