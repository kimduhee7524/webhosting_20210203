#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
	int arr[5][6] = { NULL };
	int i, j;
	int	sum = 0;
	printf("철수,영희,영수,철희 순으로 국,영,수,사,과 입력\n");

	//점수 입력받음
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			scanf_s("%d", &arr[i][j]);

	//학생 총점 계산
	for (int i = 0; i < 4; i++) {
		sum = 0;
		for (int j = 0; j < 5; j++) {
			sum += arr[i][j];
			arr[i][5] = sum;  //마지막 행에 총점 저장
		}
	}

	//과목 점수 총점 계산
	for (int i = 0; i < 5; i++) {
		sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += arr[j][i];
			arr[4][i] = sum;  //마지막 열에 총점 저장
		}
	}
	arr[4][5] = 0;

	//배열전체 출력
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			printf("%3d", arr[i][j]);
		}
		printf("\n");
	}
	
	//평균값 출력
	printf("국어 평군: %d\n", arr[4][0]/4);
	printf("영어 평군: %d\n", arr[4][1]/4);
	printf("수학 평군: %d\n", arr[4][2]/4);
	printf("사회 평군: %d\n", arr[4][3]/4);
	printf("과학 평군: %d\n", arr[4][4]/4);
			
	
	
	return 0;
}