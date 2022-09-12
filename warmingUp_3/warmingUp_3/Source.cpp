#include "Header.h"

#define MAX_LENGTH 300

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

void init_text(FILE* File, char str[][MAX_LENGTH]) {

	int k = 0;
	while (fgets(str[k], MAX_LENGTH, File) != NULL) {
		if (*str[k] && str[k][strlen(str[k]) - 1] == '\n') { // �� �ٹٲ� ���� ����
			str[k][strlen(str[k]) - 1] = 0;
		}
		printf("%s\n", str[k]); // ���ڿ� ���
		k++;
	}
}

void print_text(char str[][MAX_LENGTH]) {
	for (int i = 0; i < 10; i++) {
		printf("%s\n", str[i]);
	}
}


void reverse(char str[][MAX_LENGTH]) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < strlen(str[i]) / 2; k++) {
			SWAP(str[i][k], str[i][strlen(str[i]) - k - 1], char);
		}
	}
}

void insert_a(char str[][MAX_LENGTH]) {
	const char* input = "@@";
	for (int i = 0; i < 10; i++) {

	}
}


int main(void) {
	FILE* File = fopen("data.txt", "r");
	if (File == NULL) {
		printf("������ ����\n");
		return 1;
	}

	char temp[MAX_LENGTH];
	fgets(temp, MAX_LENGTH, File);

	char str[10][MAX_LENGTH];
	

	init_text(File, str);

	//print_text(str);

	while (1) {
		char order = 0;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);
		switch (order) {
		case 'd': // ������
			reverse(str);
			print_text(str);
			break;
		case 'e': // ���� ���� Ư�� ���� ���� ���� ���� (3���� �� @@ ����)

			break;
		case 'f': // @@ �������� ���� ������

			break;
		case 'g': // Ư�� ���ڸ� �ٸ� ���ڷ� �ٲٱ�

			break;
		case 'h': // �տ��� �о��� ��, �ڿ��� �о��� �� ������ ���� �������� ���

			break;
		case 'q':
			exit(0);
			break;
		}
	}


	fclose(File);
	return 0;
}