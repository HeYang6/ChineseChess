#include "list.h"

enum Orientation
{
	LeftTop,
	RightTop,
	LeftBottom,
	RightBottom
};
List* currentStatuList;//当前记录的走棋数据(将军重置)
List* historyStatuList;//记录历史的走棋数据
Font font;
Font fontFinish;
const char text[] = "将帅士仕象相马车俥炮兵卒菜鸟大神楚河漢界胜利败北再来一局";
Image rookieImg;
Image manitoImg;
Texture rookie;//菜鸟
Texture manito;//大神
ChessPiece location[32];
Sound sound;//“将军”音效
Sound threeSound;//“三将无赖”音效
bool isRedVictory;//是否是红方取得胜利
Music music;
Music victory;
Music defeat;
#pragma region 用来判断三将无赖后恢复棋盘的条件
ChessPiece eatPiece;//当前黑方被红方吃掉的棋子
ChessPiece* currentPiece = NULL;//红方当前选中要走的棋子
#pragma endregion
#pragma region 恢复原图形的函数(画棋盘)
//一般的原位置图形
void boardConvertion(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y - 36, x, y + 36, GRAY);//竖
	DrawLine(x - 36, y, x + 36, y, GRAY);//横
}
//靠近顶部的原位置图形(马象将)下河道5个(不靠边，不遮挡字)
void boardTopAndBottomLiver(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y, x, y + 36, GRAY);//竖
	DrawLine(x - 36, y, x + 36, y, GRAY);//横
}
//靠近底部的原位置图形(马相帅)、上河道5个(不靠边，不遮挡字)
void boardBottomAndTopLiver(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y - 36, x, y, GRAY);//竖
	DrawLine(x - 36, y, x + 36, y, GRAY);//横
}
void board00(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y, x, y + 36, GRAY);//竖
	DrawLine(x, y, x + 36, y, GRAY);//横
}
void board03(unsigned short x, unsigned short y)
{
	boardTopAndBottomLiver(x, y);
	DrawLine(x, y, x + 74, y + 74, GRAY);
}
void board05(unsigned short x, unsigned short y)
{
	boardTopAndBottomLiver(x, y);
	DrawLine(x, y, x - 74, y - 74, GRAY);
}
void board08(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y, x, y + 36, GRAY);//竖
	DrawLine(x - 36, y, x, y, GRAY);//横
}
void boardLeft(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y - 36, x, y + 36, GRAY);//竖
	DrawLine(x, y, x + 36, y, GRAY);//横
}
//将帅可行走范围的中心点位置
void boardCross(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x - 36, y - 36, x + 36, y + 36, GRAY);
	DrawLine(x - 36, y - 36, x + 36, y + 36, GRAY);
}
void boardRight(unsigned short x, unsigned short y)
{
	DrawCircle(x, y, 36, RAYWHITE);
	DrawLine(x, y - 36, x, y + 36, GRAY);//竖
	DrawLine(x, y, x - 36, y, GRAY);//横
}
void board21(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y - 74, x + 36, y, GRAY);
}
void board23(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y - 74, x - 36, y, GRAY);
}
void board38(unsigned short x, unsigned short y)
{
	boardBottomAndTopLiver(x, y);
	DrawTextEx(font, "楚河", (Vector2) { 420, 349 }, 50, 5, GRAY);
}
void board42(unsigned short x, unsigned short y)
{
	boardBottomAndTopLiver(x, y);
	DrawTextEx(font, "漢界", (Vector2) { 716, 349 }, 50, 5, GRAY);
}
void board47(unsigned short x, unsigned short y)
{
	boardTopAndBottomLiver(x, y);
	DrawTextEx(font, "楚河", (Vector2) { 420, 349 }, 50, 5, GRAY);
}
void board51(unsigned short x, unsigned short y)
{
	boardTopAndBottomLiver(x, y);
	DrawTextEx(font, "漢界", (Vector2) { 716, 349 }, 50, 5, GRAY);
}
void board66(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y, x + 74, y + 74, GRAY);
}
void board68(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y, x - 74, y - 74, GRAY);
}
void board81(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y, x, y - 36, GRAY);//竖
	DrawLine(x, y, x + 36, y, GRAY);//横
}
void board84(unsigned short x, unsigned short y)
{
	boardBottomAndTopLiver(x, y);
	DrawLine(x, y, x + 36, y - 74, GRAY);
}
void board86(unsigned short x, unsigned short y)
{
	boardBottomAndTopLiver(x, y);
	DrawLine(x, y, x - 36, y - 74, GRAY);
}
void board89(unsigned short x, unsigned short y)
{
	boardConvertion(x, y);
	DrawLine(x, y, x, y - 36, GRAY);//竖
	DrawLine(x, y, x - 36, y, GRAY);//横
}
#pragma endregion
// 二维数组，记录棋盘状态，0 表示该位置没有棋子，1 表示有棋子
bool board[10][9] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 1, 0},
	{1, 0, 1, 0, 1, 0, 1, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 1, 0, 1, 0, 1, 0, 1},
	{0, 1, 0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1}
};
void (*boardP[10][9])(unsigned short, unsigned short) = {
	{board00, boardTopAndBottomLiver, boardTopAndBottomLiver, board03, boardTopAndBottomLiver, board05, boardTopAndBottomLiver, boardTopAndBottomLiver, board08},
	{boardLeft, boardConvertion, boardConvertion, boardConvertion, boardCross, boardConvertion, boardConvertion, boardConvertion, boardRight},
	{boardLeft, boardConvertion, boardConvertion, board21, boardConvertion, board23, boardConvertion, boardConvertion, boardRight},
	{boardLeft, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardRight},
	{boardLeft, boardBottomAndTopLiver, board38, boardBottomAndTopLiver, boardBottomAndTopLiver, boardBottomAndTopLiver, board42, boardBottomAndTopLiver, boardRight},
	{boardLeft, boardTopAndBottomLiver, board47, boardTopAndBottomLiver, boardTopAndBottomLiver, boardTopAndBottomLiver, board51, boardTopAndBottomLiver, boardRight},
	{boardLeft, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardConvertion, boardRight},
	{boardLeft, boardConvertion, boardConvertion, board66, boardConvertion, board68, boardConvertion, boardConvertion, boardRight},
	{boardLeft, boardConvertion, boardConvertion, boardConvertion, boardCross, boardConvertion, boardConvertion, boardConvertion, boardRight},
	{board81, boardBottomAndTopLiver, boardBottomAndTopLiver, board84, boardBottomAndTopLiver, board86, boardBottomAndTopLiver, boardBottomAndTopLiver, board89},
};//画每一个棋盘点，可以用来清空棋盘点上面的棋子，暂时没有用到
char blackGeneralIndex[] = { 0,4 };//黑棋将军位置
char redMarshalIndex[] = { 9,4 };//红棋将军位置
//重置棋盘状态
void resetBoard()
{
	for (char i = 0; i < 10; i++) {
		for (char j = 0; j < 9; j++) {
			if (i == 0 || i == 9) board[i][j] = 1;
			else if(i==1||i==4||i==5||i==8) board[i][j] = 0;
			else if (i == 2 || i == 7)
			{
				if(j==1||j==7) board[i][j] = 1;
				else board[i][j] = 0;
			}
			else
			{
				if(j%2==0) board[i][j] = 1;
				else board[i][j] = 0;
			}
		}
	}
	clear(currentStatuList);
	clear(historyStatuList);
	blackGeneralIndex[0] = 0;
	blackGeneralIndex[1] = 4;
	redMarshalIndex[0] = 9;
	redMarshalIndex[1] = 4;
}
//获取棋盘的位置索引
int(*calCulateBoardIndex(unsigned short x, unsigned short y))[2]
{
	int* temp = (int*)calloc(2, sizeof(int));
temp[1] = (x - 311) / 74;
temp[0] = (y - 40) / 74;
	return temp;
}
//获取棋盘的位置像素
int(*calCulateBoardPixel(char row, char col))[2]
{
	int* temp = (int*)calloc(2, sizeof(int));
temp[1] = row * 74 + 40;
temp[0] = col * 74 + 311;
	return temp;
}

#pragma region 马走法规则判断
// 判断从 (row, col) 到 (targetRow, targetCol) 的路线上是否有其他棋子挡路
bool no_blockHorse(char row, char col, char targetRow, char targetCol) {
	if (row - targetRow == 2 && col - targetCol == 1) { // “日”字向上走一格，再向左走两格
		return board[row - 1][col] == 0; // 路线上是否有棋子挡路
	}
	else if (row - targetRow == 2 && targetCol - col == 1) { // “日”字向上走一格，再向右走两格
		return board[row - 1][col] == 0;
	}
	else if (targetRow - row == 2 && col - targetCol == 1) { // “日”字向下走一格，再向左走两格
		return board[row + 1][col] == 0;
	}
	else if (targetRow - row == 2 && targetCol - col == 1) { // “日”字向下走一格，再向右走两格
		return board[row + 1][col] == 0;
	}
	else if (row - targetRow == 1 && col - targetCol == 2) { // “日”字向左走一格，再向上走两格
		return board[row][col - 1] == 0;
	}
	else if (row - targetRow == 1 && targetCol - col == 2) { // “日”字向右走一格，再向上走两格
		return board[row][col + 1] == 0;
	}
	else if (targetRow - row == 1 && col - targetCol == 2) { // “日”字向左走一格，再向下走两格
		return board[row][col - 1] == 0;
	}
	else if (targetRow - row == 1 && targetCol - col == 2) { // “日”字向右走一格，再向下走两格
		return board[row][col + 1] == 0;
	}
	return false; // 不满足上述情况，说明不是“日”字走法
}
// 判断一个“马”是否可以从 (row, col) 移动到 (targetRow, targetCol)
bool canMoveHorse(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag) if (row == targetRow && col == targetCol) return false;
	char dx = targetRow - row;
	char dy = targetCol - col;
	if (dx * dx + dy * dy != 5) { // 不满足“日”字走法
		return false;
	}
	return no_blockHorse(row, col, targetRow, targetCol); // 路线上是否有棋子挡路
}
#pragma endregion
#pragma region 车走法规则判断
// 判断是否在同一行
bool inSameRowPing(char row, char col, char targetRow, char targetCol) {
	return row == targetRow;
}

// 判断是否在同一列
bool inSameColPing(char row, char col, char targetRow, char targetCol) {
	return col == targetCol;
}

// 判断两点之间是否有棋子阻隔
bool hasChessPing(char row, char col, char targetRow, char targetCol) {
	int i, j;

	// 在同一行上
	if (inSameRowPing(row, col, targetRow, targetCol)) {
		int min_col = col < targetCol ? col : targetCol;
		int max_col = col > targetCol ? col : targetCol;
		for (j = min_col + 1; j < max_col; j++) {
			if (board[row][j] != 0) {
				return true;
			}
		}
	}

	// 在同一列上
	if (inSameColPing(row, col, targetRow, targetCol)) {
		int min_row = row < targetRow ? row : targetRow;
		int max_row = row > targetRow ? row : targetRow;
		for (i = min_row + 1; i < max_row; i++) {
			if (board[i][col] != 0) {
				return true;
			}
		}
	}

	return false;
}

// 判断车是否能走到目标位置
bool canMovePing(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag) if (row == targetRow && col == targetCol) return false;
	// 不在同一行也不在同一列，不能走
	if (!inSameRowPing(row, col, targetRow, targetCol) && !inSameColPing(row, col, targetRow, targetCol)) {
		return false;
	}

	// 两点之间有棋子阻隔，不能走
	if (hasChessPing(row, col, targetRow, targetCol)) {
		return false;
	}

	return true;
}
#pragma endregion
#pragma region 象走法规则判断
// 判断是否在同一个斜线上（如"象"的走法，需要判断是否在同一个对角线）
bool inSameLineElephant(char row, char col, char targetRow, char targetCol) {
	return abs(row - targetRow) == abs(col - targetCol);
}

// 判断象是否能够走到目标位置
bool canMoveElephant(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag) if (row == targetRow && col == targetCol) return false;
	// 计算两个位置之间的行、列距离
	int row_distance = abs(row - targetRow);
	int col_distance = abs(col - targetCol);

	// 如果不在同一对角线，不能走
	if (!inSameLineElephant(row, col, targetRow, targetCol)) {
		return false;
	}
	if (flag)
	{
		// 如果走到对方阵营，则不能走
		if (targetRow >= 5) {
			return false;
		}
		// 如果当前在后三排，且目标位置在另一个侧面，则不能走
		if (row < 5 && abs(col - targetCol) == 4) {
			return false;
		}
	}
	else
	{
		// 如果走到对方阵营，则不能走
		if (targetRow < 5) {
			return false;
		}
		// 如果当前在后三排，且目标位置在另一个侧面，则不能走
		if (row >= 5 && abs(col - targetCol) == 4) {
			return false;
		}
	}
	// 如果中间隔着棋子，则不能走
	if (row_distance == 2 && col_distance == 2) {
		char mid_row = (row + targetRow) / 2;
		char mid_col = (col + targetCol) / 2;
		if (board[mid_row][mid_col] != 0) {
			return false;
		}
	}
	if (abs(row - targetRow) == 2 && abs(col - targetCol) == 2) return true;
	else return false;
}
#pragma endregion
#pragma region 士走法规则判断
// 判断是否在士的可行走范围内
bool inValidRangeBachelor(char row, char col) {
	return (row >= 7 && row <= 9 && col >= 3 && col <= 5) ||
		(row >= 0 && row <= 2 && col >= 3 && col <= 5);
}

// 判断士是否能够走到目标位置
bool canMoveBachelor(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag) if (row == targetRow && col == targetCol) return false;
	// 如果目标位置不在己方阵营内，不能走
	if (!inValidRangeBachelor(targetRow, targetCol)) {
		return false;
	}

	// 如果不是斜着走一格，不能走
	if (abs(row - targetRow) != 1 || abs(col - targetCol) != 1) {
		return false;
	}

	// 如果目标位置有自己的棋子，不能走
	if (board[targetRow][targetCol]) {
		return false;
	}

	return true;
}
#pragma endregion
#pragma region 将走法规则判断
// 判断目的地是否在将的可行走范围内
bool inValidRangeGeneral(char row, char col, bool flag) {
	if (flag) return row >= 0 && row <= 2 && col >= 3 && col <= 5;
	else return row >= 7 && row <= 9 && col >= 3 && col <= 5;
}

// 判断是否与对方将相对
bool isFaceToFaceGeneral(char row, char col, char targetRow, bool flag) {
	// 判断是否有棋子在中间位置
	//取将/帅其中一个纵坐标位置索引
	char index = redMarshalIndex[1];
	if(redMarshalIndex[1]!= blackGeneralIndex[1]) return false;
	char blackX = blackGeneralIndex[0];
	char redX = redMarshalIndex[0];
	for (char i = blackX+1; i < redX; i++)
	{
		if (board[i][index]) return false;
	}
	return true;
}
//不能直接杀到对面将军后的将军判断
bool generalValidDetail(char row, char col, char targetRow, char targetCol, bool flag)
{
	// 如果目标位置不在将的可行走范围内，不能走
	if (!inValidRangeGeneral(targetRow, targetCol, flag)) {
		return false;
	}
	// 如果不是横向或纵向移动一格，不能走
	if (row != targetRow && col != targetCol) {
		return false;
	}
	if (abs(row - targetRow) > 1 || abs(col - targetCol) > 1) {
		return false;
	}
	return true;
}
// 判断将是否能够走到目标位置
bool canMoveGeneral(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag)//黑将
	{
		if (row == targetRow && col == targetCol) return false;
		// 能直接杀到对面将军
		if (col == redMarshalIndex[1]&&col==targetCol) if (isFaceToFaceGeneral(row, col, targetRow, flag)) return true;else return generalValidDetail(row, col, targetRow, targetCol, flag);//不能直接杀到对面将军
		else return generalValidDetail(row, col, targetRow, targetCol, flag);//不能直接杀到对面将军
	}
	else//红帅
	{
		// 能直接杀到对面将军
		if (col == blackGeneralIndex[1]&&col==targetCol) if (isFaceToFaceGeneral(row, col, targetRow, flag)) return true;else return generalValidDetail(row, col, targetRow, targetCol, flag);//不能直接杀到对面将军
		else return generalValidDetail(row, col, targetRow, targetCol, flag);//不能直接杀到对面将军
	}
}
#pragma endregion
#pragma region 炮走法规则判断
// 判断炮是否能够走到目标位置
bool canMoveCannon(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag) if (row == targetRow && col == targetCol) return false;
	// 如果不是横向或纵向移动，不能走（普通移动）
	if (row != targetRow && col != targetCol) {
		return false;
	}
	int i, count = 0;
	for (i = row + 1; i < targetRow; i++) {//向右走
		if (board[i][targetCol] != 0) {
			count++;
		}
	}
	if (count > 0) goto exit;
	for (i = row - 1; i > targetRow; i--) {//向左走
		if (board[i][targetCol] != 0) {
			count++;
		}
	}
	if (count > 0) goto exit;
	for (i = col + 1; i < targetCol; i++) {//向下走
		if (board[targetRow][i] != 0) {
			count++;
		}
	}
	if (count > 0) goto exit;
	for (i = col - 1; i > targetCol; i--) {//向上走
		if (board[targetRow][i] != 0) {
			count++;
		}
	}
exit:
	if (board[targetRow][targetCol] != 0)//目标位置是否有棋子
	{
		//判断目标位置棋子是否是我方棋子
		if (flag)//黑棋
		{
			for (char i = 0; i < 16; i++)
			{
				int(*temp)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y);
				if (targetRow == (*temp)[0] && targetCol == (*temp)[1])
				{
					free(temp);
					return false;
				}
				free(temp);
			}
		}
		else//红棋
		{
			for (char i = 16; i < 32; i++)
			{
				int(*temp)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y);
				if (targetRow == (*temp)[0] && targetCol == (*temp)[1])
				{
					free(temp);
					return false;
				}
				free(temp);
			}
		}
		if (count == 1) return true;
		else return false;
	}
	else return !count;//目标位置无棋子
}
#pragma endregion
#pragma region 兵走法规则判断
// 判断兵是否能够走到目标位置
bool canMoveArms(char row, char col, char targetRow, char targetCol, bool flag) {
	if (flag)//黑棋
	{
		if (row == targetRow && col == targetCol) return false;
		if (row > targetRow) {
			return false; // 不能后退
		}
		int delta = row - targetRow;
		if (row < 5)//未过河
		{
			// 判断起点和终点是否在同一列上，只能前进一格
			if (col != targetCol || delta < -1) {
				return false;
			}
			//是否有自己的棋子挡住
			if (board[targetRow][targetCol])
			{
				for (char i = 0; i < 16; i++)
				{
					if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
				}
			}
		}
		else//已过河
		{
			// 左右移动的情况下，还需要检查目的地是否在同一行上
			if (!delta) {//0
				int delta2 = col - targetCol;
				if (abs(delta2) == 1)
				{
					//是否有自己的棋子挡住
					if (board[targetRow][targetCol])
					{
						for (char i = 0; i < 16; i++)
						{
							if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
						}
					}
				}
				else return false;
			}
			else if (delta < -1) return false;//代表前进了好几步
			else//前进一步
			{
				if (col != targetCol) return false;//代表斜着走的
				//是否有自己的棋子挡住
				if (board[targetRow][targetCol])
				{
					for (char i = 0; i < 16; i++)
					{
						if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
					}
				}
			}
		}
	}
	else//红棋
	{
		if (targetRow > row) {
			return false; // 不能后退
		}
		int delta = row - targetRow;
		if (row > 4)//未过河
		{
			// 判断起点和终点是否在同一列上，只能前进一格
			if (col != targetCol || delta > 1) {
				return false;
			}
			//是否有自己的棋子挡住
			if (board[targetRow][targetCol])
			{
				for (int i = 16; i < 32; i++)
				{
					if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
				}
			}
		}
		else//已过河
		{
			// 左右移动的情况下，还需要检查目的地是否在同一行上
			if (!delta) {//0
				int delta2 = col - targetCol;
				if (abs(delta2) == 1)
				{
					//是否有自己的棋子挡住
					if (board[targetRow][targetCol])
					{
						for (int i = 16; i < 32; i++)
						{
							if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
						}
					}
				}
				else return false;
			}
			else if (delta > 1) return false;//代表前进了好几步
			else//前进一步
			{
				if (col != targetCol) return false;//代表斜着走的
				//是否有自己的棋子挡住
				if (board[targetRow][targetCol])
				{
					for (int i = 16; i < 32; i++)
					{
						if (location[i].pos.x == targetRow && location[i].pos.y == targetCol) return false;
					}
				}
			}
		}
	}
	return true;
}
#pragma endregion
//画炮和兵的坐标符
void DrawOrientation(int x, int y, int len, enum Orientation array[])
{
	int offsetX;
	int offsetY;
	for (int i = 0; i < len; i++)
	{
		switch (array[i])
		{
		case LeftTop:
			offsetX = x - 5;
			offsetY = y - 5;
			DrawLine(offsetX, offsetY, offsetX, offsetY - 15, GRAY);
			DrawLine(offsetX, offsetY, offsetX - 15, offsetY, GRAY);
			break;
		case RightTop:
			offsetX = x + 5;
			offsetY = y - 5;
			DrawLine(offsetX, offsetY, offsetX, offsetY - 15, GRAY);
			DrawLine(offsetX, offsetY, offsetX + 15, offsetY, GRAY);
			break;
		case LeftBottom:
			offsetX = x - 5;
			offsetY = y + 5;
			DrawLine(offsetX, offsetY, offsetX, offsetY + 15, GRAY);
			DrawLine(offsetX, offsetY, offsetX - 15, offsetY, GRAY);
			break;
		case RightBottom:
			offsetX = x + 5;
			offsetY = y + 5;
			DrawLine(offsetX, offsetY, offsetX, offsetY + 15, GRAY);
			DrawLine(offsetX, offsetY, offsetX + 15, offsetY, GRAY);
			break;
		}
	}
}
//画棋子
void DrawChessPiece(ChessPiece piece)
{
	DrawCircleV(piece.pos, 33, (Color) { 230, 210, 160, 255 });
	DrawTextEx(font, piece.type, (Vector2) { piece.pos.x - 20, piece.pos.y - 26 }, 50, 0, piece.color);
}
//函数内容是是否播放“将军”语音提醒,返回值：true：没有出现三将无赖的情况，false：红方将军时出现了三将无赖的情况
bool IsGenelalRemind(bool flag,ChessPiece piece,int redThreeRow,int redThreeCol,int redThreeTargetRow,int redThreeTargetCol)
{
	if (flag)//黑棋“将军”
	{
		push(currentStatuList, piece);
		push(historyStatuList, piece);
		for (char i = 0; i < 16; i++)
		{
			if (!location[i].flag) continue;
			int(*p_arrCurrent)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y);
			int row = (*p_arrCurrent)[0];
			int col = (*p_arrCurrent)[1];
			free(p_arrCurrent);
			for (char j = 16; j < 32; j++)
			{
				if (!location[j].flag || location[j].type != "帅") continue;
				int(*p_arrTarget)[2] = calCulateBoardIndex(location[j].pos.x, location[j].pos.y);
				int targetRow = (*p_arrTarget)[0];
				int targetCol = (*p_arrTarget)[1];
				free(p_arrTarget);
				if (location[i].canMove(row, col, targetRow, targetCol, true))
				{
					printf("黑棋将军\n");
					PlaySound(sound);
					return true;
				}
				break;
			}
		}
		return true;
	}
	else//红棋“将军”
	{
		bool threeGenelalFlag = true;
		for (char i = 16; i < 32; i++)
		{
			if (!location[i].flag) continue;
			int(*p_arrCurrent)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y);
			int row = (*p_arrCurrent)[0];
			int col = (*p_arrCurrent)[1];
			free(p_arrCurrent);
			for (char j = 0; j < 16; j++)
			{
				if (!location[j].flag || location[j].type != "将") continue;
				int(*p_arrTarget)[2] = calCulateBoardIndex(location[j].pos.x, location[j].pos.y);
				int targetRow = (*p_arrTarget)[0];
				int targetCol = (*p_arrTarget)[1];
				free(p_arrTarget);
				if (location[i].canMove(row, col, targetRow, targetCol, false))
				{
					int currentLength = size(currentStatuList);
					int historyLength = size(historyStatuList);
					if (historyLength < currentLength * 3) goto label;
					short offsetLength = historyLength - currentLength * 3;
					bool differentFlag = false;
					for (char j = 3; j > 0; --j)
					{
						for (short k = currentLength - 1; k >= 0; --k)
						{
							ChessPiece current = get(currentStatuList, k);
							ChessPiece history = get(historyStatuList, offsetLength + currentLength *j - k-1);
							if (current.pos.x != history.pos.x && current.pos.y != history.pos.y && current.pieceID != history.pieceID)
							{
								differentFlag = true;
								break;
							}
						}
						if (differentFlag) break;
						if (!j)
						{
							PlaySound(threeSound);
							printf("三将无赖\n");
							//恢复棋盘棋子
							if (eatPiece.pos.x)//有吃棋子
							{
								board[redThreeTargetRow][redThreeTargetCol] = 1;
								location[eatPiece.id].flag = true;
							}
							else//没有吃棋子
							{
								board[redThreeTargetRow][redThreeTargetCol] = 0;
							}
							if (currentPiece->type == "帅")
							{
								redMarshalIndex[0] = row;
								redMarshalIndex[1] = col;
							}
							board[row][col] = 1;
							location[piece.id].pos = (Vector2){ currentPiece->pos.x,currentPiece->pos.y };
							threeGenelalFlag = false;
							goto exit;
						}
					}
					label:
					printf("红棋将军\n");
					PlaySound(sound);
					clear(currentStatuList);//“将军”重置当前走棋记录
					goto exit;
				}
				break;
			}
		}
		push(currentStatuList, piece);
		push(historyStatuList, piece);
	exit:
		return threeGenelalFlag;
	}
}
List* pieceList;
//电脑(黑棋)走棋
void ComputerChessGo()
{
	auto temp = GetRandomValue(0, size(pieceList) - 1);
	ChessPiece currPiece = get(pieceList, temp);
	int(*p_arrCurrent)[2] = calCulateBoardIndex(currPiece.pos.x, currPiece.pos.y); // 调用函数并将其返回值指针赋值给指针变量
	int row = (*p_arrCurrent)[0];
	int col = (*p_arrCurrent)[1];
	free(p_arrCurrent);
	for (char i = 16; i < 32; i++)
	{
		if (!location[i].flag) continue;
		int(*p_arrTarget)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y); // 调用函数并将其返回值指针赋值给指针变量
		int targetRow = (*p_arrTarget)[0];
		int targetCol = (*p_arrTarget)[1];
		free(p_arrTarget);
		if (currPiece.canMove(row, col, targetRow, targetCol, true))
		{
			location[i].flag = false;
			if (location[i].type == "帅") isRedVictory = false;
			if (currPiece.type == "将")
			{
				blackGeneralIndex[0] = targetRow;
				blackGeneralIndex[1] = targetCol;
			}
			printf("黑：row(%d),col(%d),targetRow(%d),targetCol(%d)\n", row, col, targetRow, targetCol);
			location[currPiece.id].pos = location[i].pos;
			board[row][col] = 0;
			board[targetRow][targetCol] = 1;
			IsGenelalRemind(true, location[currPiece.id], 0, 0, 0, 0);
			return;
		}
	}
	erase(pieceList, temp);
	if (!size(pieceList))//无棋可吃
	{
		for (char i = 0; i < 16; i++)
		{
			int(*p_arrCurrent)[2] = calCulateBoardIndex(location[i].pos.x, location[i].pos.y); // 调用函数并将其返回值指针赋值给指针变量
			row = (*p_arrCurrent)[0];
			col = (*p_arrCurrent)[1];
			free(p_arrCurrent);
			for (char j = 0; j < 10; j++)
			{
				for (char k = 0; k < 9; k++)
				{
					if (board[j][k]) continue;
					if (location[i].canMove(row, col, j, k, true))
					{
						if (currPiece.type == "将")
						{
							blackGeneralIndex[0] = j;
							blackGeneralIndex[1] = k;
						}
						printf("黑：row(%d),col(%d),j(%d),k(%d)\n", row, col, j, k);
						int(*p_arrTarget)[2] = calCulateBoardPixel(j, k);
						location[i].pos = (Vector2){ (*p_arrTarget)[0],(*p_arrTarget)[1] };
						board[row][col] = 0;
						board[j][k] = 1;
						free(p_arrTarget);
						IsGenelalRemind(true, location[i], 0, 0, 0, 0);
						return;
					}
				}
			}
		}
		printf("黑棋无棋可走，游戏结束");
		location[4].flag=isRedVictory = false;
		return;
	}
	ComputerChessGo();
}
void init()
{
	// 设置代码页为 UTF-8
	setlocale(0, "en_US.utf8");
	InitWindow(990, 768, "中国象棋");
	Image icon = LoadImage("ico.png");
	SetWindowIcon(icon);
	Image rookieImg = LoadImage("rookie.png");
	rookie = LoadTextureFromImage(rookieImg);
	rookie.height = 384;
	rookie.width = 200;
	Image manitoImg = LoadImage("manito.png");
	manito = LoadTextureFromImage(manitoImg);
	manito.height = 384;
	manito.width = 200;
	//读取字体文件
	unsigned int fileSize;
	unsigned char* fontFileData = LoadFileData("STFANGSO.ttf", &fileSize);
	// 将字符串中的字符逐一转换成Unicode码点，得到码点表
	int codepointsCount;
	int* codepoints = LoadCodepoints(text, &codepointsCount);
	// 读取仅码点表中各字符的字体
	font = LoadFontFromMemory(".ttf", fontFileData, fileSize, 50, codepoints, codepointsCount);
	fontFinish = LoadFontFromMemory(".ttf", fontFileData, fileSize, 80, codepoints, codepointsCount);
	UnloadImage(icon);
	// 释放码点表
	UnloadCodepoints(codepoints);
	UnloadFileData(fontFileData);
	pieceList = createList();
	currentStatuList = createList();
	historyStatuList = createList();
	InitAudioDevice();
	music = LoadMusicStream("music.mp3");
	victory = LoadMusicStream("victory.mp3");
	defeat = LoadMusicStream("defeat.mp3");
	sound=LoadSound("general.wav");
	threeSound = LoadSound("threeGeneral.wav");
}
void close()
{
	UnloadTexture(rookie);
	UnloadTexture(manito);
	UnloadImage(rookieImg);
	UnloadImage(manitoImg);
	destroyList(pieceList);
	destroyList(currentStatuList);
	destroyList(historyStatuList);
	//释放字体
	UnloadFont(font);
	UnloadFont(fontFinish);
	//释放音效
	UnloadSound(sound);
	UnloadSound(threeSound);
	StopMusicStream(music);
	UnloadMusicStream(music);
	StopMusicStream(victory);
	UnloadMusicStream(victory);
	StopMusicStream(defeat);
	UnloadMusicStream(defeat);
	CloseAudioDevice();
	CloseWindow();
}
int main()
{
	init();
	PlayMusicStream(music);
	while (!WindowShouldClose()) {
		Music temp;
		if (IsAudioStreamPlaying(music.stream)) temp = music;
		else if (IsAudioStreamPlaying(victory.stream)) temp = victory;
		else temp = defeat;
		UpdateMusicStream(temp);
		bool handFlag=true;//不走棋状态下鼠标移动到棋子上方光标是否变成手型
		BeginDrawing();
		ClearBackground(RAYWHITE);
#pragma region 画玩家
		DrawTexture(rookie, 0, 0, WHITE);
		DrawTexture(manito, 0, 385, WHITE);
		DrawTextEx(font, "菜鸟", (Vector2) { 180, 200 }, 50, 0, BLACK);
		DrawTextEx(font, "大神", (Vector2) { 180, 600 }, 50, 0, RED);
#pragma endregion
#pragma region 画棋盘
		for (char i = 0; i < 10; i++) {
			DrawLine(311, i * 74 + 40, 8 * 74 + 311, i * 74 + 40, GRAY);// 绘制横线
			if (i == 9) break;
			DrawLine(i * 74 + 311, 40, i * 74 + 311, 9 * 74 + 40, GRAY);// 绘制纵线
		}
		DrawRectangle(380, 337, 474, 73, RAYWHITE);//重写河道线
		DrawTextEx(font, "楚河", (Vector2) { 420, 349 }, 50, 5, GRAY);
		DrawTextEx(font, "漢界", (Vector2) { 716, 349 }, 50, 5, GRAY);
		DrawLine(681, 706, 533, 558, GRAY);
		DrawLine(533, 706, 681, 558, GRAY);
		DrawLine(681, 40, 533, 188, GRAY);
		DrawLine(533, 40, 681, 188, GRAY);
		//点缀炮位置
		DrawOrientation(385, 558, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//红
		DrawOrientation(829, 558, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//红
		DrawOrientation(385, 188, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//黑
		DrawOrientation(829, 188, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//黑
		//点缀兵位置
		DrawOrientation(311, 484, 2, (enum Orientation[]) { RightTop, RightBottom });//红
		DrawOrientation(459, 484, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//红
		DrawOrientation(607, 484, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//红
		DrawOrientation(755, 484, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//红
		DrawOrientation(903, 484, 2, (enum Orientation[]) { LeftTop, LeftBottom });//红
		DrawOrientation(311, 262, 2, (enum Orientation[]) { RightTop, RightBottom });//黑
		DrawOrientation(459, 262, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//黑
		DrawOrientation(607, 262, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//黑
		DrawOrientation(755, 262, 4, (enum Orientation[]) { LeftTop, RightTop, LeftBottom, RightBottom });//黑
		DrawOrientation(903, 262, 2, (enum Orientation[]) { LeftTop, LeftBottom });//黑
#pragma endregion
#pragma region 画棋子
		if (location[0].pos.x == 0)//棋盘初始化
		{
			ChessPiece piece;
			piece.flag = true;
			piece.id = 0;
			piece.color = BLACK;
			piece.pos = (Vector2){ 311,40 };
			piece.type = "俥";
			piece.canMove = canMovePing;
			piece.pieceID = BlackPing;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[0] = piece;
			piece.id = 1;
			piece.pos = (Vector2){ 385,40 };
			piece.type = "马";
			piece.canMove = canMoveHorse;
			piece.pieceID = BlackHorse;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[1] = piece;
			piece.id = 2;
			piece.pos = (Vector2){ 459,40 };
			piece.type = "象";
			piece.canMove = canMoveElephant;
			piece.pieceID = BlackElephant;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[2] = piece;
			piece.id = 3;
			piece.pos = (Vector2){ 533,40 };
			piece.type = "士";
			piece.canMove = canMoveBachelor;
			piece.pieceID = BlackBachelor;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[3] = piece;
			piece.id = 4;
			piece.pos = (Vector2){ 607,40 };
			piece.type = "将";
			piece.canMove = canMoveGeneral;
			piece.pieceID = BlackGeneral;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[4] = piece;
			piece.id = 5;
			piece.pos = (Vector2){ 681,40 };
			piece.type = "士";
			piece.canMove = canMoveBachelor;
			piece.pieceID = BlackBachelor;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[5] = piece;
			piece.id = 6;
			piece.pos = (Vector2){ 755,40 };
			piece.type = "象";
			piece.canMove = canMoveElephant;
			piece.pieceID = BlackElephant;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[6] = piece;
			piece.id = 7;
			piece.pos = (Vector2){ 829,40 };
			piece.type = "马";
			piece.canMove = canMoveHorse;
			piece.pieceID = BlackHorse;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[7] = piece;
			piece.id = 8;
			piece.pos = (Vector2){ 903,40 };
			piece.type = "俥";
			piece.canMove = canMovePing;
			piece.pieceID = BlackPing;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[8] = piece;
			piece.id = 9;
			piece.pos = (Vector2){ 385,188 };
			piece.type = "炮";
			piece.canMove = canMoveCannon;
			piece.pieceID = BlackCannon;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[9] = piece;
			piece.id = 10;
			piece.pos = (Vector2){ 829,188 };
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[10] = piece;
			piece.id = 11;
			piece.pos = (Vector2){ 311,262 };
			piece.type = "卒";
			piece.canMove = canMoveArms;
			piece.pieceID = BlackArms;
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[11] = piece;
			piece.id = 12;
			piece.pos = (Vector2){ 459,262 };
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[12] = piece;
			piece.id = 13;
			piece.pos = (Vector2){ 607,262 };
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[13] = piece;
			piece.id = 14;
			piece.pos = (Vector2){ 755,262 };
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[14] = piece;
			piece.id = 15;
			piece.pos = (Vector2){ 903,262 };
			DrawChessPiece(piece);
			push(pieceList, piece);
			location[15] = piece;
			piece.id = 16;
			piece.color = RED;
			piece.pos = (Vector2){ 311,484 };
			piece.type = "兵";
			piece.pieceID = RedArms;
			DrawChessPiece(piece);
			location[16] = piece;
			piece.id = 17;
			piece.pos = (Vector2){ 459,484 };
			DrawChessPiece(piece);
			location[17] = piece;
			piece.id = 18;
			piece.pos = (Vector2){ 607,484 };
			DrawChessPiece(piece);
			location[18] = piece;
			piece.id = 19;
			piece.pos = (Vector2){ 755,484 };
			DrawChessPiece(piece);
			location[19] = piece;
			piece.id = 20;
			piece.pos = (Vector2){ 903,484 };
			DrawChessPiece(piece);
			location[20] = piece;
			piece.id = 21;
			piece.pos = (Vector2){ 385,558 };
			piece.type = "炮";
			piece.canMove = canMoveCannon;
			piece.pieceID = RedCannon;
			DrawChessPiece(piece);
			location[21] = piece;
			piece.id = 22;
			piece.pos = (Vector2){ 829,558 };
			DrawChessPiece(piece);
			location[22] = piece;
			piece.id = 23;
			piece.pos = (Vector2){ 311,706 };
			piece.type = "车";
			piece.canMove = canMovePing;
			piece.pieceID = RedPing;
			DrawChessPiece(piece);
			location[23] = piece;
			piece.id = 24;
			piece.pos = (Vector2){ 385,706 };
			piece.type = "马";
			piece.canMove = canMoveHorse;
			piece.pieceID = RedHorse;
			DrawChessPiece(piece);
			location[24] = piece;
			piece.id = 25;
			piece.pos = (Vector2){ 459,706 };
			piece.type = "相";
			piece.canMove = canMoveElephant;
			piece.pieceID = RedElephant;
			DrawChessPiece(piece);
			location[25] = piece;
			piece.id = 26;
			piece.pos = (Vector2){ 533,706 };
			piece.type = "仕";
			piece.canMove = canMoveBachelor;
			piece.pieceID = RedBachelor;
			DrawChessPiece(piece);
			location[26] = piece;
			piece.id = 27;
			piece.pos = (Vector2){ 607,706 };
			piece.type = "帅";
			piece.canMove = canMoveGeneral;
			piece.pieceID = RedGeneral;
			DrawChessPiece(piece);
			location[27] = piece;
			piece.id = 28;
			piece.pos = (Vector2){ 681,706 };
			piece.type = "仕";
			piece.canMove = canMoveBachelor;
			piece.pieceID = RedBachelor;
			DrawChessPiece(piece);
			location[28] = piece;
			piece.id = 29;
			piece.pos = (Vector2){ 755,706 };
			piece.type = "相";
			piece.canMove = canMoveElephant;
			piece.pieceID = RedElephant;
			DrawChessPiece(piece);
			location[29] = piece;
			piece.id = 30;
			piece.pos = (Vector2){ 829,706 };
			piece.type = "马";
			piece.canMove = canMoveHorse;
			piece.pieceID = RedHorse;
			DrawChessPiece(piece);
			location[30] = piece;
			piece.id = 31;
			piece.pos = (Vector2){ 903,706 };
			piece.type = "车";
			piece.canMove = canMovePing;
			piece.pieceID = RedPing;
			DrawChessPiece(piece);
			location[31] = piece;
		}
		else//棋盘复原
		{
			for (char i = 0; i < 32; i++)
			{
				if (!location[i].flag) continue;
				DrawChessPiece(location[i]);
				if (i < 16)
					push(pieceList, location[i]);
			}
		}
#pragma endregion
		if (!location[4].flag || !location[27].flag)//游戏结束
		{
			StopMusicStream(music);
			unsigned short centerX = GetScreenWidth() / 2;
			unsigned short centerY = GetScreenHeight() / 2;
			unsigned short X = centerX - 50;
			unsigned short Y = centerY - 10;
			Rectangle rect = { X,Y,202,70 };
			DrawRectangleRec(rect, GOLD);
			DrawTextEx(fontFinish, "再来一局", (Vector2) { X, Y }, 66, 0, BLACK);
			if (isRedVictory)
			{
				PlayMusicStream(victory);
				DrawTextEx(fontFinish, "胜利", (Vector2) { centerX -20, centerY - 100 }, 80, 3, RED);
				if (CheckCollisionPointRec(GetMousePosition(), rect))
				{
					DrawRectangleRec(rect, GREEN);
					DrawTextEx(fontFinish, "再来一局", (Vector2) { X, Y }, 66, 0, BLACK);
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						StopMusicStream(victory);
						PlayMusicStream(music);
						//棋盘初始化
						location[0].pos.x = 0;
						isRedVictory = false;
						resetBoard();
					}
				}
			}
			else
			{
				PlayMusicStream(defeat);
				DrawTextEx(fontFinish, "败北", (Vector2) { centerX - 20, centerY - 100 }, 80, 3, GRAY);
				if (CheckCollisionPointRec(GetMousePosition(), rect))
				{
					DrawRectangleRec(rect, GREEN);
					DrawTextEx(fontFinish, "再来一局", (Vector2) { X, Y }, 66, 0, BLACK);
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						StopMusicStream(defeat);
						PlayMusicStream(music);
						//棋盘初始化
						location[0].pos.x = 0;
						resetBoard();
					}
				}
			}
			goto exit;
		}
		if (currentPiece)
		{
			DrawCircleLines(currentPiece->pos.x, currentPiece->pos.y, 36, GREEN);
			unsigned short x = 0, y = 0;
			for (unsigned short i = 311; i <= 903; i += 74)
			{
				auto temp = i - GetMousePosition().x;
				if ((temp <= 33 && temp >= 0) || (temp >= -33 && temp <= 0))
				{
					x = i;
					break;
				}
			}
			if (x)
			{
				for (unsigned short i = 40; i <= 706; i += 74)
				{
					auto temp = i - GetMousePosition().y;
					if ((temp <= 33 && temp >= 0) || (temp >= -33 && temp <= 0))
					{
						y = i;
						break;
					}
				}
			}
			if (x && y)
			{
				bool flag = true;
				for (char i = 16; i < 32; i++)
				{
					if (!location[i].flag) continue;
					if (x == location[i].pos.x && y == location[i].pos.y)
					{
						flag = false;
						break;
					}
				}
				if (flag)
				{
					int(*p_arrCurrent)[2] = calCulateBoardIndex(currentPiece->pos.x, currentPiece->pos.y); // 调用函数并将其返回值指针赋值给指针变量
					int(*p_arrTarget)[2] = calCulateBoardIndex(x, y); // 调用函数并将其返回值指针赋值给指针变量
					int row = (*p_arrCurrent)[0];
					int col = (*p_arrCurrent)[1];
					int targetRow = (*p_arrTarget)[0];
					int targetCol = (*p_arrTarget)[1];
					bool threeFlag = false;//是否触发三将无赖
					if (currentPiece->canMove(row, col, targetRow, targetCol, false))//我走棋
					{
						SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
						{
							printf("红：row(%d),col(%d),targetRow(%d),targetCol(%d)\n", row, col, targetRow, targetCol);
#pragma region 如果目标位置有敌方棋子的话就吃掉
							for (size_t i = 0; i < 16; i++)
							{
								if (!location[i].flag) continue;
								if (x == location[i].pos.x && y == location[i].pos.y)
								{
									if (location[i].type == "将")
									{
										isRedVictory = true;
									}
									location[i].flag = false;
									eatPiece = location[i];
									break;
								}
							}
#pragma endregion
							if (currentPiece->type == "帅")
							{
								redMarshalIndex[0] = targetRow;
								redMarshalIndex[1] = targetCol;
							}
							board[row][col] = 0;
							board[targetRow][targetCol] = 1;
							location[currentPiece->id].pos = (Vector2){ x,y };
							if (!isRedVictory)
							{
								if (IsGenelalRemind(false, location[currentPiece->id], row, col, targetRow, targetCol))
								{
									handFlag = false;
									SetMouseCursor(MOUSE_CURSOR_DEFAULT);
									free(currentPiece);
									currentPiece = NULL;
								}
							}
							else
							{
								handFlag = false;
								SetMouseCursor(MOUSE_CURSOR_DEFAULT);
								free(currentPiece);
								currentPiece = NULL;
								free(p_arrCurrent);
								free(p_arrTarget);
								goto exit;
							}
							ComputerChessGo();//电脑走棋
						}
					}
					else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
					free(p_arrCurrent);
					free(p_arrTarget);
				}
			}
			else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
		if (handFlag)//画当前棋子光标
		{
			for (char i = 16; i < 32; i++)
			{
				if (i == 31 && !currentPiece) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
				if (!location[i].flag) continue;
				if (CheckCollisionPointCircle(GetMousePosition(), location[i].pos, 33))
				{
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						if (currentPiece) free(currentPiece);
						currentPiece = (ChessPiece*)calloc(1, sizeof(ChessPiece));
						*currentPiece = location[i];
						DrawCircleLines(location[i].pos.x, location[i].pos.y, 36, GREEN);
					}
					break;
				}
			}
		}
		exit:
		clear(pieceList);
		EndDrawing();
	}
	close();
	return 0;
}
