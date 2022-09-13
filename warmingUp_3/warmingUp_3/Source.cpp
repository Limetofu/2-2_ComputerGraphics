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

void insert(char* m, char* s, int n)
{
	memmove(m + n, m, strlen(m));
	memmove(m + n, s, strlen(s));
}

void insert_a(char str[][MAX_LENGTH]) {
	char input[2];
	input[0] = '@';
	input[1] = '@';

	int count = 0;

	{
		std::string s(str[0]);
		std::string s1(str[1]);
		std::string s2(str[2]);
		std::string s3(str[3]);
		std::string s4(str[4]);
		std::string s5(str[5]);
		std::string s6(str[6]);
		std::string s7(str[7]);
		std::string s8(str[8]);
		std::string s9(str[9]);

		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s1.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s1.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s2.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s2.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s3.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s3.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s4.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s4.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s5.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s5.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s6.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s6.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s7.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s7.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s8.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s8.insert(i, "@@");
			}
		}
		for (int i = 3; i < MAX_LENGTH; i += 5) {
			if (i >= s9.length()) break;
			if (i % 5 == 3 && (i < MAX_LENGTH - 1)) {
				s9.insert(i, "@@");
			}
		}

		strcpy(str[0], s.c_str());
		strcpy(str[1], s1.c_str());
		strcpy(str[2], s2.c_str());
		strcpy(str[3], s3.c_str());
		strcpy(str[4], s4.c_str());
		strcpy(str[5], s5.c_str());
		strcpy(str[6], s6.c_str());
		strcpy(str[7], s7.c_str());
		strcpy(str[8], s8.c_str());
		strcpy(str[9], s9.c_str());
	}
	/*
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < strlen(str[i]); k++) {
			count++;
			if (count == 3) {
				count = 0;
				s[i].insert()
			}
		}
	}*/

	//printf("array : %s\n", str[0]);
	print_text(str);

}

void eliminate(char* str, char ch) {
	for (; *str != '\0'; str++) {
		if (*str == ch) {
			strcpy(str, str + 1);
			str--;
		}
	}
}

void delete_a(char str[][MAX_LENGTH]) {
	for (int i = 0; i < 10; i++) {
		eliminate(str[i], '@');
	}
	print_text(str);
}

void change_char(char str[][MAX_LENGTH], char from, char to) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < strlen(str[i]); k++) {
			if (str[i][k] == from) {
				str[i][k] = to;
			}
		}
	}
}

void reverse_a(char str[][MAX_LENGTH]) {
	int count = 0;
	bool if_a_printed = 0;
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < strlen(str[i]); k++) {
			// �߰����� @�� ������
			if (str[i][k] == '@') {
				// ���� 3���� ���ڸ� swap, �糡 ���� �ΰ���.
				if (if_a_printed == 0) {
					SWAP(str[i][k - 3], str[i][k - 1], char);
					if_a_printed = 1;
				}
				else 
					if_a_printed = 0;
			}
			if (k == strlen(str[i]) - 1) {
				for (int j = strlen(str[i]) - 1; j > 0; j--) {
					if (str[i][j] == '@') {
						if (count == 2) {
							SWAP(str[i][j + 1], str[i][j + 2], char);
							count = 0;
							break;
						}
						else if (count == 3) {
							SWAP(str[i][j + 1], str[i][j + 3], char);
							count = 0;
							break;
						}
						else {
							count = 0;
							break;
						}
					}
					count++;
				}
			}
		}
	}
}

void same_char(char str[][MAX_LENGTH]) {
	// �� ���ھ� �տ��� �ϳ�, �ڿ��� �ϳ��� �а�.
	// �ΰ��� ������ tempstr�� �߰�. ��� �ٸ� ���� ������ �ٷ� break �� ���
	char tempstr[MAX_LENGTH] = {'\0'};
	int len = 0;
	int count = 0;
	bool print_same = 0;

	for (int i = 0; i < 10; i++) {

		printf("%s", str[i]);
		len = strlen(str[i]);
		if (i == 9)
			printf(" ");
		for (int k = 0; k < len / 2; k++) {
			if (str[i][k] == str[i][strlen(str[i]) - 1 - k]) {
				tempstr[count] = str[i][k];
				count++;
			}
			else {
				count = 0;
				print_same = 1;
			}

			// ���� ������ ���� ���� ��
			if (k == len / 2 - 1) print_same = 1;

			if (print_same) {
				if (tempstr[0] == '\0') printf(" / no same char\n");
				else {
					printf(" / : %s\n", tempstr);
					for (int h = 0; h < MAX_LENGTH; h++) {
						tempstr[h] = '\0';
					}
				}
				print_same = 0;
				break;
			}
		}
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
	bool a_inserted = 0;

	char from = 0, to = 0;

	init_text(File, str);

	//print_text(str);

	while (1) {
		char order = 0;
		fseek(stdin, 0, SEEK_END);
		printf("\n-------------------------------------\n Command : ");
		scanf("%c", &order);
		printf("-------------------------------------\n\n");

		switch (order) {
		case 'd': // ������
			reverse(str);
			print_text(str);
			break;
		case 'e': // ���� ���� Ư�� ���� ���� ���� ���� (3���� �� @@ ����)
			if (a_inserted) {
				delete_a(str);
				a_inserted = 0;
			}
			else{
				insert_a(str);
				a_inserted = 1;
			}
			break;
		case 'f': // @@ �������� ���� ������
			reverse_a(str);
			print_text(str);
			break;
		case 'g': // Ư�� ���ڸ� �ٸ� ���ڷ� �ٲٱ�
			fseek(stdin, 0, SEEK_END);
			printf("\n�ٲ� ����, ���Ӱ� �Է��� ���� : ");
			scanf("%c %c", &from, &to);
			change_char(str, from, to);
			print_text(str);
			break;
		case 'h': // �տ��� �о��� ��, �ڿ��� �о��� �� ������ ���� �������� ���
			same_char(str);
			break;
		case 'q':
			exit(0);
			break;
		}
	}


	fclose(File);
	return 0;
}