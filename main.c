// ICELAKE GAME
#include "AI.h"
#pragma warning(disable:4996)

//===============================================================================================//

#define SIDE 4

//===============================================================================================//

void draw_field();		// 네모칸을 그림
void draw_unit();		// 별을 그림
void erase_unit();		// 이동시 별을 지움
void move();			// 키보드 입력받음
void Init();			// 구멍과 골인지점 구함
void win();				// 골인인지 확인
void lose();			// 구멍인지 확인
void hint();			// 골인, 구멍지점 표시
void status();			// 점수판
void print_Q();			// Q테이블 출력
BOOL check();			// 골인 구멍에 갈 수 있는지 확인

//===============================================================================================//

int arr[4][4] = { 0 };	// 4x4 array -> value 0 : Lake(can walk), value 1 : Hole(Game over), value 3 : Goal(win)
int score = 0;
int direction = 0;		// 전에 움직인 방향
int test = 0;

//===============================================================================================//

int main()
{
	system("mode con:cols=40 lines=30");
	system("title MyAI");
	randomize();
	setcursortype(NOCURSOR);
	draw_field();
	while (1)
	{
		Init();
		if (check() == TRUE)	break;
	}
	while (1)
	{
		AI_Qupdate();
		hint();
		move();
		win();
		lose();
		status();
		print_Q();
	}
}

//===============================================================================================//

void draw_field()
{
	int i, j;
	printf("┌ ─ ┬ ─ ┬ ─ ┬ ─ ┐\n");
	for (i = 0; i < 3; i++)
	{
		printf("│   │   │   │   │\n");
		printf("├ ─ ┼ ─ ┼ ─ ┼ ─ ┤\n");
	}
	printf("│   │   │   │   │\n");
	printf("└ ─ ┴ ─ ┴ ─ ┴ ─ ┘\n");
	gotoxy(px, py);
	draw_unit();
}

//===============================================================================================//

void draw_unit()
{
	gotoxy(px, py);  printf("★");
}

//===============================================================================================//

void erase_unit()
{
	gotoxy(px, py);	printf("  ");
}

//===============================================================================================//

void move()
{
	erase_unit();
	direction = AI_search();
	switch (direction)
	{
	case LEFT:
		px -= 4;
		break;
	case RIGHT:
		px += 4;
		break;
	case UP:
		py -= 2;
		break;
	case DOWN:
		py += 2;
		break;
	}
	if (py < 1)		py = 1;
	if (px < 2)		px = 2;
	if (px > 14)	px = 14;
	if (py > 7)		py = 7;
	draw_unit();
	log_move(direction);
	delay(100);
}

//===============================================================================================//

void Init()
{
	int a, b, i;
	for (i = 0; i < 3; i++)				// 구멍 정하기
	{
		a = random(4);
		b = random(4);
		if (a == 0 && b == 0)
		{
			i--;
			continue;
		}
		if (arr[a][b] == 1)
			i--;
		arr[a][b] = 1;
		reward[a][b] = -1;
	}
	while (1)							// 골인지점 정하기
	{
		a = random(4);
		b = random(4);
		if (arr[a][b] == 1)	continue;
		if (a == 0 && b == 0)	continue;
		else break;
	}
	arr[a][b] = 2;
	reward[a][b] = 1;
}

//===============================================================================================//

void win()
{
	if (arr[(py - 1) / 2][(px - 2) / 4] == 2)
	{
		score++;
		erase_unit();
		px = 2;
		py = 1;
		draw_unit();
		test++;
	}
}

//===============================================================================================//

void lose()
{
	if (arr[(py - 1) / 2][(px - 2) / 4] == 1)
	{
		score--;
		erase_unit();
		px = 2;
		py = 1;
		draw_unit();
		test++;
	}
}

//===============================================================================================//

void hint()
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if (arr[i][j] == 1)
			{
				gotoxy(j * 4 + 2, i * 2 + 1);	printf("H");
			}
			if (arr[i][j] == 2)
			{
				gotoxy(j * 4 + 2, i * 2 + 1);	printf("G");
			}
		}
}

//===============================================================================================//

void status()
{
	gotoxy(2, 10);
	printf("현재점수 : %+d", score);
	gotoxy(2, 11);
	printf("훈련횟수 : %d", test);
}

//===============================================================================================//

BOOL check()
{
	int i, j;
	if (arr[0][0] == GOAL && arr[0][1] == HOLE && arr[1][0] == HOLE)	return FALSE;
	if (arr[0][3] == GOAL && arr[0][2] == HOLE && arr[1][3] == HOLE)	return FALSE;
	if (arr[3][0] == GOAL && arr[2][0] == HOLE && arr[3][1] == HOLE)	return FALSE;
	if (arr[3][3] == GOAL && arr[3][2] == HOLE && arr[2][3] == HOLE)	return FALSE;
	for (i = 1; i < 3; i++)
		if (arr[0][i] == GOAL && arr[0][i - 1] == HOLE && arr[0][i + 1] == HOLE && arr[1][i] == HOLE)	return FALSE;
	for (i = 1; i < 3; i++)
		if (arr[i][0] == GOAL && arr[i - 1][0] == HOLE && arr[i + 1][0] == HOLE && arr[i][1] == HOLE)	return FALSE;
	for (i = 1; i < 3; i++)
		if (arr[3][i] == GOAL && arr[3][i - 1] == HOLE && arr[3][i + 1] == HOLE && arr[2][i] == HOLE)	return FALSE;
	for (i = 1; i < 3; i++)
		if (arr[i][3] == GOAL && arr[i - 1][3] == HOLE && arr[i + 1][3] == HOLE && arr[i][2] == HOLE)	return FALSE;
	if (arr[0][1] == HOLE && arr[1][0] == HOLE) return FALSE;
	return TRUE;
}

//===============================================================================================//

void print_Q()
{
	int i, j, k;
	gotoxy(0, 12);
	printf("           LEFT RIGHT UP DOWN\n");
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			printf("State %d : %+.2lf %+.2lf %+.2lf %+.2lf\n", 4 * i + j, Q[i][j][0], Q[i][j][1], Q[i][j][2], Q[i][j][3]);
	}
}