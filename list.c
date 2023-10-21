#include "list.h"

Node* createNode(ChessPiece data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->prev = node->next = NULL;
    return node;
}
List* createList()
{
    List* list = (List*)malloc(sizeof(List));
    list->data = NULL;
    list->size = 0;
    return list;
}
void destroyList(List* list)
{
    Node* node = list->data;
    while (node)
    {
        Node* next = node->next;
        free(node);
        node = next;
    }
    free(list);
}
void push(List* list, ChessPiece data)
{
    Node* node = createNode(data);
    if (list->data == NULL)
    {
        list->data = node;
    }
    else
    {
        Node* temp = list->data;
        Node* tempNext = temp->next;
        while (tempNext)
        {
            temp = tempNext;
            tempNext = tempNext->next;
        }
        temp->next = node;
        node->prev = temp;
    }
    list->size++;
}
int size(List* list)
{
    return list->size;
}
ChessPiece get(List* list, int index)
{
    Node* node = list->data;
    for (char i = 0; i < list->size; i++)
    {
        if (i == index) return node->data;
        node = node->next;
    }
}
void erase(List* list, int index)
{
    // �ж� index �Ƿ�Ϸ�
    if (index < 0 || index >= list->size) {
        printf("Invalid index!\n");
        return;
    }

    Node* node = list->data;
    for (char i = 0; i < index; i++) {
        node = node->next;
    }

    if (node == NULL) {
        printf("Node to delete is NULL!\n");
        return;
    }

    if (node->prev == NULL) {    // ɾ��ͷ�ڵ�
        if (node->next != NULL) {
            node->next->prev = NULL;
        }
        list->data = node->next;
    }
    else if (node->next == NULL) {    // ɾ��β�ڵ�
        node->prev->next = NULL;
    }
    else {    // ɾ���м�ڵ�
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    free(node);
    list->size--;
}
/* ���List */
void clear(List* list) {
    /* ��ͷ��βɾ�����нڵ� */
    while (list->data) {
        Node* next_node = list->data->next;
        free(list->data);
        list->data = next_node;
    }
    /* ����List��״̬ */
    list->data = NULL;
    list->size = 0;
}