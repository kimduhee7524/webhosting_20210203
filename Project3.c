#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#undef random
#define random() ((double)rand() / (RAND_MAX + 1))

#pragma region Windows GDI
// 출력 핸들 선언
HANDLE hStdout;

// 색깔 리스트
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

// 옵션2번 관련. 내림차순
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
// 출력 관련. 뽑힌 숫자 색깔.
void setTextBallColor(int x) {
	switch (x) {
	case 1:	case 2:	case 3:	case 4:case 5:	case 6:	case 7:	case 8:	case 9:	case 10:
		SetColor(14);// 노랑색
		break;
	case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
		SetColor(11);// 하늘색
		break;
	case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:
		SetColor(12);// 빨간색
		break;
	case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:
		SetColor(8);// 회색
		break;
	case 41:case 42:case 43:case 44:case 45:
		SetColor(10);// 녹색
		break;
	}
}
// 옵션1번. Top12중 3개, 1-45중 3개
int getSixNum() {
	int res[6] = { 0,0,0,0,0,0 };
	int i;
	int temp = 0;

	for (i = 0; i < 6; i++) {// 6번 추첨
		if (i < 3)
			temp = getNumFromTop12(random());
		else
			temp = getNumFrom45(random());
		res[i] = temp;// 우선 저장 후 비교
		for (int j = 0; j < i; j++) {
			if (res[j] == temp) {// 중복 시 재추첨
				i--;
				break;
			}
		}
	}
	// 출력
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

// 옵션1번 관련. 733-946회 통계기반 Top12개의 숫자만으로 확률표 구성(다트판)
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

// (733-946회) 개별 확률. 하드코딩.
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
/// 옵션1번 관련. getNumFrom45() 내부 누계용 메소드 | ex) sumXtoY(1,3) = percentage733to946[1] + percentage733to946[2] + percentage733to946[3];
/// </summary>
/// <param name="x">시작점</param>
/// <param name="y">종료점</param>
/// <returns></returns>
double sumXtoY(int x, int y) {
	double sum = 0.017912773f;// percentage733to946[0] 값
	for (int i = x; i < y; i++) {
		sum += percentage733to946[i];
	}
	return sum;
}
// 옵션1번 관련. 733-946회 통계기반 확률표(다트판)에 랜덤수(다트)를 매개변수로 입력하면 1-45 숫자 반환
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

// 옵션2번. 입력한 12개의 숫자 중 통계상 고확률 숫자 내림차수와 비교하여 순서대로 출력
int getNumFromHope12(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10, int n11, int n12) {
	// arr안에 12개의 수. length는 길이.
	int input12[12] = { n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12 };
	int res[6] = { 0,0,0,0,0,0 };
	int i;
	int idx = 0;

	// desc_num이 개별 내림차순이니 앞쪽 인덱스부터 arr와 비교하여 해당되는 번호가 있으면 바로 출력하는 형태.
	for (i = 0; i < 45; i++) {
		for (int j = 0; j < 12; j++) {
			if (desc_num[i] == input12[j]) {
				res[idx] = desc_num[i];
				if (idx != 5) idx++;// Run-Time Check Failure #2 방지
			}
		}
	}

	// 출력
	//for (i = 0; i < 6; i++) printf("%d번째 수는 [%2d]\n", (i + 1), res[i]);
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
//	// ┌│┐─└　┘
//	char logoL_01 = "┌┐　　";
//	char logoL_02 = "││　　";
//	char logoL_03 = "││　　";
//	char logoL_04 = "││　　";
//	char logoL_05 = "│└─┐";
//	char logoL_06 = "└──┘";
//	char logoO_01 = "　　　　";
//	char logoO_02 = "┌──┐";
//	char logoO_03 = "│┌┐│";
//	char logoO_04 = "││││";
//	char logoO_05 = "│└┘│";
//	char logoO_06 = "└──┘";
//	char logot_01 = "　┌┐　";
//	char logot_02 = "┌┘└┐";
//	char logot_03 = "└┐┌┘";
//	char logot_04 = "　││　";
//	char logot_05 = "　│└┐";
//	char logot_06 = "　└─┘";
//}
void printMenu() {
	char square_top[] = "┌─────────────────────────────────────────────────────────┐\n";
	char square_pipe[] = "│";
	char square_bottom[] = "└─────────────────────────────────────────────────────────┘\n";
	char sel_opt[] = "[옵션 선택]";
	char opt01[] = ">> 1. 누적추첨률 높은 12개 중 3자동 그리고 나머지 3자동";
	char opt02[] = ">> 2. 원하는 수 12개 입력 후 산출";
	char opt00[] = ">> 0. 종료";
	printf("%s", square_top);
	printf("%s %-56s%s\n", square_pipe, sel_opt, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt01, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt02, square_pipe);
	printf("%s %-56s%s\n", square_pipe, opt00, square_pipe);
	printf("%s", square_bottom);
}

int main(void) {
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);// 출력 핸들 초기화
	system("title 로또 번호 추첨기");// 제목
	system("mode con:cols=60 lines=30");// 창 크기60*30 (숫자, 알파벳 1칸, 한글 2칸)

	// 변수 선언
	int opt = -1;// 메뉴 옵션
	int pick12[12];// 옵션2번 선택시 입력할 숫자 저장용 배열

	srand((unsigned)time(NULL));// 난수 입력

	while (1) {
		cls(hStdout);// 콘솔 내용 지우기
		printMenu();
		printf("입력 --> \t");
		scanf_s("%d", &opt);
		if (opt == 1) {
			getSixNum();
		}
		else if (opt == 2) {// 희망하는 12개의 숫자 입력
			for (int i = 0; i < 12; i++) {
				printf("[%2d]번째 수 입력 --> \t", (i + 1));
				scanf_s("%d", &pick12[i]);
				if (pick12[i] < 1 || pick12[i] > 45) {// 1~45 범위 벗어나면 다시 입력
					printf("ERR >> 범위 초과! 다시 입력하세요.\n");
					i--; continue;
				}
				for (int j = 0; j < i; j++) // 중복 시 재입력
					if (pick12[j] == pick12[i]) {
						printf("ERR >> [%2d]와 중복! 다시 입력하세요.\n", pick12[j]);
						i--;
						break;
					}
			}
			getNumFromHope12(pick12[0], pick12[1], pick12[2], pick12[3], pick12[4], pick12[5], pick12[6], pick12[7], pick12[8], pick12[9], pick12[10], pick12[11]);
		}
		else if (opt == 0)// 루프 탈출
			break;
		system("pause");
	}

	return 0;
}


///// <summary>
///// qsort용 오름차순 비교
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
///// qsort용 내림차순 비교
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
//// TODO 매크로 작성
//int setSortedNumbers() {
//	int matchNum = 214 * 6;// (946-733+1) * (숫자 개수)
//	// 숫자별 뽑힌 횟수
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
//	// 숫자(1 - 45) 개별 백분율.ex) percentage[0] 숫자 1의 백분율.
//	double percentage[45];
//	// 계산용 임시저장 배열
//	double temp1[45];
//	double temp2[45];
//	// 백분율 오름차순(숫자가 작은 것 부터)
//	double asc_nums[45];
//	// 백분율 내림차순(숫자가 큰 것 부터)
//	double desc_nums[45];
//	// C에 콜렉션이 없어서 사용하는 인덱스.
//	int idx[45];
//
//	// map이 없으니... 생각해보자
//	// 1. 개별 수 퍼센티지(idx 동일)
//	// 2. 오름차순 정렬(idx 바뀜)
//	// 3. percentage와 비교해서 인덱스 저장
//	// 4. 내림차순 정렬
//	// 5. 23에서 바뀐 인덱스에 새로 대입
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
//	// 오름차순 저장
//	for (int i = 0; i < 45; i++) temp1[i] = percentage[i];
//	qsort(temp1, 45, sizeof(double), compareAsc);
//	for (int i = 0; i < 45; i++) asc_nums[i] = temp1[i];
//
//	// 내림차순 저장
//	for (int i = 0; i < 45; i++) temp2[i] = percentage[i];
//	qsort(temp2, 45, sizeof(double), compareDesc);
//	for (int i = 0; i < 45; i++) desc_nums[i] = temp2[i];
//
//	//// 테스트 출력
//	//printf("오름차순\n");
//	//for (int i = 0; i < 45; i++) printf("%f\n", asc_nums[i]);
//	//printf("내림차순\n");
//	//for (int i = 0; i < 45; i++) printf("%f\n", desc_nums[i]);
//
//	return 0;
//};