#pragma once
#include <raylib.h>
#include <stdio.h>

enum PieceId
{
	None,
	BlackPing,
	BlackHorse,
	BlackElephant,
	BlackBachelor,
	BlackGeneral,
	BlackCannon,
	BlackArms,
	RedPing,
	RedHorse,
	RedElephant,
	RedBachelor,
	RedGeneral,
	RedCannon,
	RedArms
};
typedef struct 
{
    unsigned char id;//����
    Vector2 pos;//����
    bool flag;//�Ƿ���Ч
    Color color;//������ɫ
    char* type;//�������ͣ������й��������,����7�����ͣ�
	enum PieceId pieceID;//����ID
    bool (*canMove)(char row, char col, char targetRow, char targetCol, bool flag);
} ChessPiece;
typedef struct _Node 
{
    ChessPiece data;
    struct _Node* prev;
    struct _Node* next;
} Node;
typedef struct _List
{
    Node* data;
    int size;
} List;
Node* createNode(ChessPiece data);
List* createList();
void destroyList(List* list);
void push(List* list, ChessPiece data);
int size(List* list);
ChessPiece get(List* list, int index);
void erase(List* list, int index);
void clear(List* list);