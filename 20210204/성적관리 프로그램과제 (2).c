#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
	int arr[5][6] = { NULL };
	int i, j;
	int	sum = 0;
	printf("ö��,����,����,ö�� ������ ��,��,��,��,�� �Է�\n");

	//���� �Է¹���
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			scanf_s("%d", &arr[i][j]);

	//�л� ���� ���
	for (int i = 0; i < 4; i++) {
		sum = 0;
		for (int j = 0; j < 5; j++) {
			sum += arr[i][j];
			arr[i][5] = sum;  //������ �࿡ ���� ����
		}
	}

	//���� ���� ���� ���
	for (int i = 0; i < 5; i++) {
		sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += arr[j][i];
			arr[4][i] = sum;  //������ ���� ���� ����
		}
	}
	arr[4][5] = 0;

	//�迭��ü ���
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			printf("%3d", arr[i][j]);
		}
		printf("\n");
	}
	
	//��հ� ���
	printf("���� ��: %d\n", arr[4][0]/4);
	printf("���� ��: %d\n", arr[4][1]/4);
	printf("���� ��: %d\n", arr[4][2]/4);
	printf("��ȸ ��: %d\n", arr[4][3]/4);
	printf("���� ��: %d\n", arr[4][4]/4);
			
	
	
	return 0;
}