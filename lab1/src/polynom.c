#include <stdio.h>

int n = 3; // ������� ��������
int k[] = {1, 2, 3, 4}; // ������������
int x = 2; // �������� X

int main()
{
	int i;
	
	int res = 0;	
	int xi = 1; // x^i, i=0 -> x^0 = 1
	for(i=0; i<=n; i++)
	{
		printf("%d X^%d %d k = %d\n", i, i, xi, k[n-i]);
		res += k[n-i] * xi;
		xi *= x; // ���������� x^(i+1)
	}
	printf("res: %d\n", res);
	
	return 0;
}

