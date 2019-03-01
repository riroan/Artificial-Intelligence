#include"good.h"

//===============================================================================================//

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define LAKE 0
#define HOLE 1
#define GOAL 2
#define RATE 0.9		// 감가율

//===============================================================================================//

int AI_search();
int AI_max(double a[4]);						// 배열 arr의 최대값의 인덱스를 반환한다. 단, 4를 반환하면 모든 값이 같다.(arr의 길이는 4이다.)
void AI_Qupdate();							// 현재 상태에서 q값 업데이트(실시간)
void log_move(int x);

//===============================================================================================//

int px = 2;
int py = 1;
int count = 0;
double Q[4][4][4] = { 0 };
/*
※ Q[a][b][c]에 대한 설명
-  Q[a][b]는 arr[a][b]를 뜻한다.	(state)
-  c는 행할 수 있는 액션을 뜻한다.  (action)
-  action에서 c는 0 : LEFT, 1 : RIGHT, 2 : UP, 3 : DOWN
-  Q[a][b][c]는 현재 state에서 할 action의 최적수를 찾는다.
*/
double reward[4][4] = { 0 };

//===============================================================================================//

void log_move(int x)
{
	gotoxy(2, 9);
	printf("행동 : ");
	switch (x)
	{
	case LEFT:
		printf("←");
		break;
	case RIGHT:
		printf("→");
		break;
	case UP:
		printf("↑");
		break;
	case DOWN:
		printf("↓");
		break;
	}
}

//===============================================================================================//

int AI_max(double a[4])
{
	int i = 0;
	int result = 0;
	double max = 0.0;
	if (px == 2 && py == 1)
	{
		max = a[1];
		result = 1;
		for (i = 2; i < 4; i++)
		{
			if (i == 2)	continue;
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (px == 14 && py == 1)
	{
		max = a[0];
		result = 0;
		for (i = 1; i < 4; i++)
		{
			if (i == 1 || i == 2)	continue;
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (px == 2 && py == 7)
	{
		max = a[1];
		result = 1;
		for (i = 2; i < 4; i++)
		{
			if (i == 3)	continue;
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (px == 14 && py == 7)
	{
		max = a[0];
		result = 0;
		for (i = 1; i < 4; i++)
		{
			if (i == 1 || i == 3)	continue;
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (px == 2)
	{
		max = a[1];
		result = 1;
		for (i = 2; i < 4; i++)
		{
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (px == 14)
	{
		max = a[0];
		result = 0;
		for (i = 2; i < 4; i++)
		{
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (py == 1)
	{
		max = a[0];
		result = 0;
		for (i = 1; i < 4; i++)
		{
			if (i == 2)	continue;
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else if (py == 7)
	{
		max = a[0];
		result = 0;
		for (i = 1; i < 3; i++)
		{
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	else
	{
		max = a[0];
		result = 0;
		for (i = 0; i < 4; i++)
		{
			if (max < a[i])
			{
				max = a[i];
				result = i;
			}
		}
	}
	if (max == 0)
	{
		while (1)
		{
			result = random(4);								// 0 : LEFT, 1 : RIGHT, 2 : UP, 3 : DOWN
			if (py == 1 && result == 2)			continue;	// 가장 위쪽에 있는데 위쪽방향으로 가려하는 경우
			else if (px == 2 && result == 0)		continue;	// 가장 왼쪽에 있는데 왼쪽방향으로 가려하는 경우
			else if (px == 14 && result == 1)	continue;	// 가장 오른쪽에 있는데 오른쪽방향으로 가려하는 경우
			else if (py == 7 && result == 3)		continue;	// 가장 밑쪽에 있는데 밑방향으로 가려하는 경우
			else break;
		}
	}
	return result;
}

//===============================================================================================//

int AI_search()
{
	int index;
	index = AI_max(Q[(py - 1) / 2][(px - 2) / 4]);
	switch (index)
	{
	case 0:
		if (px != 2)
			return LEFT;
		break;
	case 1:
		if (px != 14)
			return RIGHT;
		break;
	case 2:
		if (py != 1)
			return UP;
		break;
	case 3:
		if (py != 7)
			return DOWN;
		break;
	}
}

//===============================================================================================//

void AI_Qupdate()
{
	if (px != 2)
		Q[(py - 1) / 2][(px - 2) / 4][0] = reward[(py - 1) / 2][(px - 6) / 4] + RATE * Q[(py - 1) / 2][(px - 6) / 4][AI_max(Q[(py - 1) / 2][(px - 6) / 4])];		// 왼쪽
	if (px != 14)
		Q[(py - 1) / 2][(px - 2) / 4][1] = reward[(py - 1) / 2][(px + 2) / 4] + RATE * Q[(py - 1) / 2][(px + 2) / 4][AI_max(Q[(py - 1) / 2][(px + 2) / 4])];		// 오른쪽
	if (py != 1)
		Q[(py - 1) / 2][(px - 2) / 4][2] = reward[(py - 3) / 2][(px - 2) / 4] + RATE * Q[(py - 3) / 2][(px - 2) / 4][AI_max(Q[(py - 3) / 2][(px - 2) / 4])];		// 위쪽
	if (py != 7)
		Q[(py - 1) / 2][(px - 2) / 4][3] = reward[(py + 1) / 2][(px - 2) / 4] + RATE * Q[(py + 1) / 2][(px - 2) / 4][AI_max(Q[(py + 1) / 2][(px - 2) / 4])];		// 아래쪽
}