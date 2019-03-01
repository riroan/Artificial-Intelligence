#include"good.h"

//===============================================================================================//

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define LAKE 0
#define HOLE 1
#define GOAL 2
#define RATE 0.9		// ������

//===============================================================================================//

int AI_search();
int AI_max(double a[4]);						// �迭 arr�� �ִ밪�� �ε����� ��ȯ�Ѵ�. ��, 4�� ��ȯ�ϸ� ��� ���� ����.(arr�� ���̴� 4�̴�.)
void AI_Qupdate();							// ���� ���¿��� q�� ������Ʈ(�ǽð�)
void log_move(int x);

//===============================================================================================//

int px = 2;
int py = 1;
int count = 0;
double Q[4][4][4] = { 0 };
/*
�� Q[a][b][c]�� ���� ����
-  Q[a][b]�� arr[a][b]�� ���Ѵ�.	(state)
-  c�� ���� �� �ִ� �׼��� ���Ѵ�.  (action)
-  action���� c�� 0 : LEFT, 1 : RIGHT, 2 : UP, 3 : DOWN
-  Q[a][b][c]�� ���� state���� �� action�� �������� ã�´�.
*/
double reward[4][4] = { 0 };

//===============================================================================================//

void log_move(int x)
{
	gotoxy(2, 9);
	printf("�ൿ : ");
	switch (x)
	{
	case LEFT:
		printf("��");
		break;
	case RIGHT:
		printf("��");
		break;
	case UP:
		printf("��");
		break;
	case DOWN:
		printf("��");
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
			if (py == 1 && result == 2)			continue;	// ���� ���ʿ� �ִµ� ���ʹ������� �����ϴ� ���
			else if (px == 2 && result == 0)		continue;	// ���� ���ʿ� �ִµ� ���ʹ������� �����ϴ� ���
			else if (px == 14 && result == 1)	continue;	// ���� �����ʿ� �ִµ� �����ʹ������� �����ϴ� ���
			else if (py == 7 && result == 3)		continue;	// ���� ���ʿ� �ִµ� �ع������� �����ϴ� ���
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
		Q[(py - 1) / 2][(px - 2) / 4][0] = reward[(py - 1) / 2][(px - 6) / 4] + RATE * Q[(py - 1) / 2][(px - 6) / 4][AI_max(Q[(py - 1) / 2][(px - 6) / 4])];		// ����
	if (px != 14)
		Q[(py - 1) / 2][(px - 2) / 4][1] = reward[(py - 1) / 2][(px + 2) / 4] + RATE * Q[(py - 1) / 2][(px + 2) / 4][AI_max(Q[(py - 1) / 2][(px + 2) / 4])];		// ������
	if (py != 1)
		Q[(py - 1) / 2][(px - 2) / 4][2] = reward[(py - 3) / 2][(px - 2) / 4] + RATE * Q[(py - 3) / 2][(px - 2) / 4][AI_max(Q[(py - 3) / 2][(px - 2) / 4])];		// ����
	if (py != 7)
		Q[(py - 1) / 2][(px - 2) / 4][3] = reward[(py + 1) / 2][(px - 2) / 4] + RATE * Q[(py + 1) / 2][(px - 2) / 4][AI_max(Q[(py + 1) / 2][(px - 2) / 4])];		// �Ʒ���
}