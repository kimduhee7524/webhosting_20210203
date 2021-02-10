#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#undef random
#define random() ((double)rand() / (RAND_MAX + 1))

#pragma region Windows GDI
// ��� �ڵ� ����
HANDLE hStdout;

// ���� ����Ʈ
// 0 = Black	1 = Blue		2 = Green		 3 = Aqua	 	 4 = Red		 5 = Purple			 6 = Yellow			 7 = White
// 8 = Gray		9 = Light Blue	A = Light Green	 B = Light Aqua	 C = Light Red	 D = Light Purple	 E = Light Yellow	 F = Bright White

/// <summary>
/// set cursor position into (x, y)
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void cur(short x, short y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/// <summary>
/// set foreground color
/// </summary>
/// <param name="ForgC"></param>
void SetColor(int ForgC) {
	WORD wColor;
	//This handle is needed to get the current background attribute
	//HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	//csbi is used for wAttributes word

	if (GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		//To mask out all but the background attribute, and to add the color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdout, wColor);
	}
}
/// <summary>
/// cls(clear screen)
/// </summary>
/// <param name="hConsole"></param>
void cls(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	// Scroll the rectangle of the entire buffer.
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.UnicodeChar = TEXT(' ');
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}
#pragma endregion

// �ɼ�2�� ����. ��������
int desc_num[] = {
	12,43,18,39,38,// 1-5
	21,42,16,34,45,// 6-10
	10,19,13,28,33,// 11-15
	3,17,36,15,24,// 16-20
	31,14,41,2,6,// 21-25
	11,27,30,44,5,// 26-30
	9,22,26,7,32,// 31-35
	1,4,8,23,29,// 36-40
	35,25,37,40,20// 41-45
};
// ��� ����. ���� ���� ����.
void setTextBallColor(int x) {
	switch (x) {
	case 1:	case 2:	case 3:	case 4:case 5:	case 6:	case 7:	case 8:	case 9:	case 10:
		SetColor(14);// �����
		break;
	case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
		SetColor(11);// �ϴû�
		break;
	case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:
		SetColor(12);// ������
		break;
	case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:
		SetColor(8);// ȸ��
		break;
	case 41:case 42:case 43:case 44:case 45:
		SetColor(10);// ���
		break;
	}
}
// �ɼ�1��. Top12�� 3��, 1-45�� 3��
int getSixNum() {
	int res[6] = { 0,0,0,0,0,0 };
	int i;
	int temp = 0;

	for (i = 0; i < 6; i++) {// 6�� ��÷
		if (i < 3)
			temp = getNumFromTop12(random());
		else
			temp = getNumFrom45(random());
		res[i] = temp;// �켱 ���� �� ��
		for (int j = 0; j < i; j++) {
			if (res[j] == temp) {// �ߺ� �� ����÷
				i--;
				break;
			}
		}
	}
	// ���
	for (i = 0; i < 6; i++) {
		Sleep(500);
		printf("[");
		setTextBallColor(res[i]);
		printf("%2d", res[i]);
		SetColor(7);
		printf("]");
	}
	printf("\n\n");
	return 0;
}

// �ɼ�1�� ����. 733-946ȸ ����� Top12���� ���ڸ����� Ȯ��ǥ ����(��Ʈ��)
int getNumFromTop12(double x) {
	if (0 < x && x <= 0.09557171)		return 12;
	else if (x <= 0.186481386f)		return 43;
	else if (x <= 0.275060045f)		return 18;
	else if (x <= 0.361307686f)		return 39;
	else if (x <= 0.44522431f)		return 38;
	else if (x <= 0.526809916f)		return 21;
	else if (x <= 0.608395523f)		return 42;
	else if (x <= 0.687650112f)		return 16;
	else if (x <= 0.766904701f)		return 34;
	else if (x <= 0.84615929f)		return 45;
	else if (x <= 0.923082862f)		return 10;
	else if (x <= 1.0f)		return 19;
}

// (733-946ȸ) ���� Ȯ��. �ϵ��ڵ�.
double percentage733to946[] = {
	0.017912773f,	0.021028037f,	0.023364486f,	0.017912773f,	0.019470405f,// 5
	0.021028037f,	0.018691589f,	0.017912773f,	0.019470405f,	0.025700935f,// 10
	0.021028037f,	0.031931464f,	0.024922118f,	0.021806854f,	0.02258567f,// 15
	0.026479751f,	0.023364486f,	0.029595016f,	0.025700935f,	0.012461059f,// 20
	0.027258567f,	0.019470405f,	0.017912773f,	0.02258567f,	0.017133956f,// 25
	0.019470405f,	0.021028037f,	0.024922118f,	0.017912773f,	0.021028037f,// 30
	0.02258567f,	0.018691589f,	0.024143302f,	0.026479751f,	0.017912773f,// 35
	0.023364486f,	0.01635514f,	0.028037383f,	0.028816199f,	0.01635514f,// 40
	0.021806854f,	0.027258567f,	0.030373832f,	0.020249221f,	0.026479751f// 45
};
/// <summary>
/// �ɼ�1�� ����. getNumFrom45() ���� ����� �޼ҵ� | ex) sumXtoY(1,3) = percentage733to946[1] + percentage733to946[2] + percentage733to946[3];
/// </summary>
/// <param name="x">������</param>
/// <param name="y">������</param>
/// <returns></returns>
double sumXtoY(int x, int y) {
	double sum = 0.017912773f;// percentage733to946[0] ��
	for (int i = x; i < y; i++) {
		sum += percentage733to946[i];
	}
	return sum;
}
// �ɼ�1�� ����. 733-946ȸ ����� Ȯ��ǥ(��Ʈ��)�� ������(��Ʈ)�� �Ű������� �Է��ϸ� 1-45 ���� ��ȯ
int getNumFrom45(double x) {
	if (0 < x && x <= sumXtoY(0, 0))		return 1;
	else if (x <= sumXtoY(0, 1))		return 2;
	else if (x <= sumXtoY(0, 2))		return 3;
	else if (x <= sumXtoY(0, 3))		return 4;
	else if (x <= sumXtoY(0, 4))		return 5;
	else if (x <= sumXtoY(0, 5))		return 6;
	else if (x <= sumXtoY(0, 6))		return 7;
	else if (x <= sumXtoY(0, 7))		return 8;
	else if (x <= sumXtoY(0, 8))		return 9;
	else if (x <= sumXtoY(0, 9))		return 10;
	else if (x <= sumXtoY(0, 10))		return 11;
	else if (x <= sumXtoY(0, 11))		return 12;
	else if (x <= sumXtoY(0, 12))		return 13;
	else if (x <= sumXtoY(0, 13))		return 14;
	else if (x <= sumXtoY(0, 14))		return 15;
	else if (x <= sumXtoY(0, 15))		return 16;
	else if (x <= sumXtoY(0, 16))		return 17;
	else if (x <= sumXtoY(0, 17))		return 18;
	else if (x <= sumXtoY(0, 18))		return 19;
	else if (x <= sumXtoY(0, 19))		return 20;
	else if (x <= sumXtoY(0, 20))		return 21;
	else if (x <= sumXtoY(0, 21))		return 22;
	else if (x <= sumXtoY(0, 22))		return 23;
	else if (x <= sumXtoY(0, 23))		return 24;
	else if (x <= sumXtoY(0, 24))		return 25;
	else if (x <= sumXtoY(0, 25))		return 26;
	else if (x <= sumXtoY(0, 26))		return 27;
	else if (x <= sumXtoY(0, 27))		return 28;
	else if (x <= sumXtoY(0, 28))		return 29;
	else if (x <= sumXtoY(0, 29))		return 30;
	else if (x <= sumXtoY(0, 30))		return 31;
	else if (x <= sumXtoY(0, 31))		return 32;
	else if (x <= sumXtoY(0, 32))		return 33;
	else if (x <= sumXtoY(0, 33))		return 34;
	else if (x <= sumXtoY(0, 34))		return 35;
	else if (x <= sumXtoY(0, 35))		return 36;
	else if (x <= sumXtoY(0, 36))		return 37;
	else if (x <= sumXtoY(0, 37))		return 38;
	else if (x <= sumXtoY(0, 38))		return 39;
	else if (x <= sumXtoY(0, 39))		return 40;
	else if (x <= sumXtoY(0, 40))		return 41;
	else if (x <= sumXtoY(0, 41))		return 42;
	else if (x <= sumXtoY(0, 42))		return 43;
	else if (x <= sumXtoY(0, 43))		return 44;
	else if (x <= sumXtoY(0, 44))		return 45;
}

// �ɼ�2��. �Է��� 12���� ���� �� ���� ��Ȯ�� ���� ���������� ���Ͽ� ������� ���
int getNumFromHope12(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10, int n11, int n12) {
	// arr�ȿ� 12���� ��. length�� ����.
	int input12[12] = { n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12 };
	int res[6] = { 0,0,0,0,0,0 };
	int i;
	int idx = 0;

	// desc_num�� ���� ���������̴� ���� �ε������� arr�� ���Ͽ� �ش�Ǵ� ��ȣ�� ������ �ٷ� ����ϴ� ����.
	for (i = 0; i < 45; i++) {
		for (int j = 0; j < 12; j++) {
			if (desc_num[i] == input12[j]) {
				res[idx] = desc_num[i];
				if (idx != 5) idx++;// Run-Time Check Failure #2 ����
			}
		}
	}

	// ���
	//for (i = 0; i < 6; i++) printf("%d��° ���� [%2d]\n", (i + 1), res[i]);
	for (i = 0; i < 6; i++) {
		Sleep(500);
		printf("[");
		setTextBallColor(res[i]);
		printf("%2d", res[i]);
		SetColor(7);
		printf("]");
	}
	printf("\n\n");
	return 0;
}
//void logo() {
//	// ��������������
//	char logoL_01 = "��������";
//	char logoL_02 = "��������";
//	char logoL_03 = "��������";
//	char logoL_04 = "��������";
//	char logoL_05 = "��������";
//	char logoL_06 = "��������";
//	char logoO_01 = "��������";
//	char logoO_02 = "��������";
//	char logoO_03 = "��������";
//	char logoO_04 = "��������";
//	char logoO_05 = "��������";
//	char logoO_06 = "��������";
//	char logot_01 = "��������";
//	char logot_02 = "��������";
//	char logot_03 = "��������";
//	char logot_04 = "��������";
//	char logot_05 = "��������";
//	char logot_06 = "��������";
//}
void printMenu() {
	char square_top[] = "����������������������������������������������������������������������������������������������������������������������\n";
	char square_pipe[] = "��";
	char square_bottom[] = "����������������������������������������������������������������������������������������������������������������������\n";
	char sel_opt[] = "[�ɼ� ����]";
	char opt01[] = ">> 1. ������÷�� ���� 12�� �� 3�ڵ� �׸��� ������ 3�ڵ�";
	char opt02[] = ">> 2. ���ϴ� �� 12�� �Է� �� ����";
	char opt00[] = ">> 0. ����";
	printf("%s", square_top);
	printf("%s %-56s%s\n", square_pipe, sel_opt, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt01, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt02, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt00, square_pipe);
	printf("%s", square_bottom);
}

int main(void) {
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);// ��� �ڵ� �ʱ�ȭ
	system("title �ζ� ��ȣ ��÷��");// ����
	system("mode con:cols=60 lines=30");// â ũ��60*30 (����, ���ĺ� 1ĭ, �ѱ� 2ĭ)

	// ���� ����
	int opt = -1;// �޴� �ɼ�
	int pick12[12];// �ɼ�2�� ���ý� �Է��� ���� ����� �迭

	srand((unsigned)time(NULL));// ���� �Է�

	while (1) {
		cls(hStdout);// �ܼ� ���� �����
		printMenu();
		printf("�Է� --> \t");
		scanf_s("%d", &opt);
		if (opt == 1) {
			getSixNum();
		}
		else if (opt == 2) {// ����ϴ� 12���� ���� �Է�
			for (int i = 0; i < 12; i++) {
				printf("[%2d]��° �� �Է� --> \t", (i + 1));
				scanf_s("%d", &pick12[i]);
				if (pick12[i] < 1 || pick12[i] > 45) {// 1~45 ���� ����� �ٽ� �Է�
					printf("ERR >> ���� �ʰ�! �ٽ� �Է��ϼ���.\n");
					i--; continue;
				}
				for (int j = 0; j < i; j++) // �ߺ� �� ���Է�
					if (pick12[j] == pick12[i]) {
						printf("ERR >> [%2d]�� �ߺ�! �ٽ� �Է��ϼ���.\n", pick12[j]);
						i--;
						break;
					}
			}
			getNumFromHope12(pick12[0], pick12[1], pick12[2], pick12[3], pick12[4], pick12[5], pick12[6], pick12[7], pick12[8], pick12[9], pick12[10], pick12[11]);
		}
		else if (opt == 0)// ���� Ż��
			break;
		system("pause");
	}

	return 0;
}


///// <summary>
///// qsort�� �������� ��
///// </summary>
///// <param name="first"></param>
///// <param name="second"></param>
///// <returns></returns>
//int static compareAsc(const void* first, const void* second)
//{
//	if (*(double*)first > *(double*)second)
//		return 1;
//	else if (*(double*)first < *(double*)second)
//		return -1;
//	else
//		return 0;
//}
///// <summary>
///// qsort�� �������� ��
///// </summary>
///// <param name="first"></param>
///// <param name="second"></param>
///// <returns></returns>
//int static compareDesc(const void* first, const void* second)
//{
//	if (*(double*)first < *(double*)second)
//		return 1;
//	else if (*(double*)first > *(double*)second)
//		return -1;
//	else
//		return 0;
//}
//
//// TODO ��ũ�� �ۼ�
//int setSortedNumbers() {
//	int matchNum = 214 * 6;// (946-733+1) * (���� ����)
//	// ���ں� ���� Ƚ��
//	int cnt_one = 23;
//	int cnt_two = 27;
//	int cnt_three = 30;
//	int cnt_four = 23;
//	int cnt_five = 25;
//	int cnt_six = 27;
//	int cnt_seven = 24;
//	int cnt_eight = 23;
//	int cnt_nine = 25;
//	int cnt_ten = 33;
//	int cnt_eleven = 27;
//	int cnt_twelve = 41;
//	int cnt_thirteen = 32;
//	int cnt_fourteen = 28;
//	int cnt_fifteen = 29;
//	int cnt_sixteen = 34;
//	int cnt_seventeen = 30;
//	int cnt_eighteen = 38;
//	int cnt_nineteen = 33;
//	int cnt_twenty = 16;
//	int cnt_twentyone = 35;
//	int cnt_twentytwo = 25;
//	int cnt_twentythree = 23;
//	int cnt_twentyfour = 29;
//	int cnt_twentyfive = 22;
//	int cnt_twentysix = 25;
//	int cnt_twentyseven = 27;
//	int cnt_twentyeight = 32;
//	int cnt_twentynine = 23;
//	int cnt_thirty = 27;
//	int cnt_thirtyone = 29;
//	int cnt_thirtytwo = 24;
//	int cnt_thirtythree = 31;
//	int cnt_thirtyfour = 34;
//	int cnt_thirtyfive = 23;
//	int cnt_thirtysix = 30;
//	int cnt_thirtyseven = 21;
//	int cnt_thirtyeight = 36;
//	int cnt_thirtynine = 37;
//	int cnt_fourty = 21;
//	int cnt_fourtyone = 28;
//	int cnt_fourtytwo = 35;
//	int cnt_fourtythree = 39;
//	int cnt_fourtyfour = 26;
//	int cnt_fourtyfive = 34;
//
//	// TODO static ? percentage, asc_nums, desc_nums
//	// ����(1 - 45) ���� �����.ex) percentage[0] ���� 1�� �����.
//	double percentage[45];
//	// ���� �ӽ����� �迭
//	double temp1[45];
//	double temp2[45];
//	// ����� ��������(���ڰ� ���� �� ����)
//	double asc_nums[45];
//	// ����� ��������(���ڰ� ū �� ����)
//	double desc_nums[45];
//	// C�� �ݷ����� ��� ����ϴ� �ε���.
//	int idx[45];
//
//	// map�� ������... �����غ���
//	// 1. ���� �� �ۼ�Ƽ��(idx ����)
//	// 2. �������� ����(idx �ٲ�)
//	// 3. percentage�� ���ؼ� �ε��� ����
//	// 4. �������� ����
//	// 5. 23���� �ٲ� �ε����� ���� ����
//	// 6. PROFIT ?
//
//	percentage[0] = (double)cnt_one / matchNum;
//	percentage[1] = (double)cnt_two / matchNum;
//	percentage[2] = (double)cnt_three / matchNum;
//	percentage[3] = (double)cnt_four / matchNum;
//	percentage[4] = (double)cnt_five / matchNum;
//	percentage[5] = (double)cnt_six / matchNum;
//	percentage[6] = (double)cnt_seven / matchNum;
//	percentage[7] = (double)cnt_eight / matchNum;
//	percentage[8] = (double)cnt_nine / matchNum;
//	percentage[9] = (double)cnt_ten / matchNum;
//	percentage[10] = (double)cnt_eleven / matchNum;
//	percentage[11] = (double)cnt_twelve / matchNum;
//	percentage[12] = (double)cnt_thirteen / matchNum;
//	percentage[13] = (double)cnt_fourteen / matchNum;
//	percentage[14] = (double)cnt_fifteen / matchNum;
//	percentage[15] = (double)cnt_sixteen / matchNum;
//	percentage[16] = (double)cnt_seventeen / matchNum;
//	percentage[17] = (double)cnt_eighteen / matchNum;
//	percentage[18] = (double)cnt_nineteen / matchNum;
//	percentage[19] = (double)cnt_twenty / matchNum;
//	percentage[20] = (double)cnt_twentyone / matchNum;
//	percentage[21] = (double)cnt_twentytwo / matchNum;
//	percentage[22] = (double)cnt_twentythree / matchNum;
//	percentage[23] = (double)cnt_twentyfour / matchNum;
//	percentage[24] = (double)cnt_twentyfive / matchNum;
//	percentage[25] = (double)cnt_twentysix / matchNum;
//	percentage[26] = (double)cnt_twentyseven / matchNum;
//	percentage[27] = (double)cnt_twentyeight / matchNum;
//	percentage[28] = (double)cnt_twentynine / matchNum;
//	percentage[29] = (double)cnt_thirty / matchNum;
//	percentage[30] = (double)cnt_thirtyone / matchNum;
//	percentage[31] = (double)cnt_thirtytwo / matchNum;
//	percentage[32] = (double)cnt_thirtythree / matchNum;
//	percentage[33] = (double)cnt_thirtyfour / matchNum;
//	percentage[34] = (double)cnt_thirtyfive / matchNum;
//	percentage[35] = (double)cnt_thirtysix / matchNum;
//	percentage[36] = (double)cnt_thirtyseven / matchNum;
//	percentage[37] = (double)cnt_thirtyeight / matchNum;
//	percentage[38] = (double)cnt_thirtynine / matchNum;
//	percentage[39] = (double)cnt_fourty / matchNum;
//	percentage[40] = (double)cnt_fourtyone / matchNum;
//	percentage[41] = (double)cnt_fourtytwo / matchNum;
//	percentage[42] = (double)cnt_fourtythree / matchNum;
//	percentage[43] = (double)cnt_fourtyfour / matchNum;
//	percentage[44] = (double)cnt_fourtyfive / matchNum;
//
//	// �������� ����
//	for (int i = 0; i < 45; i++) temp1[i] = percentage[i];
//	qsort(temp1, 45, sizeof(double), compareAsc);
//	for (int i = 0; i < 45; i++) asc_nums[i] = temp1[i];
//
//	// �������� ����
//	for (int i = 0; i < 45; i++) temp2[i] = percentage[i];
//	qsort(temp2, 45, sizeof(double), compareDesc);
//	for (int i = 0; i < 45; i++) desc_nums[i] = temp2[i];
//
//	//// �׽�Ʈ ���
//	//printf("��������\n");
//	//for (int i = 0; i < 45; i++) printf("%f\n", asc_nums[i]);
//	//printf("��������\n");
//	//for (int i = 0; i < 45; i++) printf("%f\n", desc_nums[i]);
//
//	return 0;
//};