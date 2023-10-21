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
    unsigned char id;//索引
    Vector2 pos;//坐标
    bool flag;//是否有效
    Color color;//棋子颜色
    char* type;//棋子类型（根据中国象棋规则,共有7种类型）
	enum PieceId pieceID;//棋子ID
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