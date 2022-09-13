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
		if (*str[k] && str[k][strlen(str[k]) - 1] == '\n') { // 끝 줄바꿈 문자 제거
			str[k][strlen(str[k]) - 1] = 0;
		}
		printf("%s\n", str[k]); // 문자열 출력
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
			// 중간에서 @를 만나면
			if (str[i][k] == '@') {
				// 이전 3개의 문자를 swap, 양끝 문자 두개만.
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
	// 한 글자씩 앞에서 하나, 뒤에서 하나씩 읽고.
	// 두개가 같으면 tempstr에 추가. 대신 다른 문자 나오면 바로 break 후 출력
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

			// 만약 끝까지 전부 같을 때
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
		printf("파일이 없음\n");
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
		case 'd': // 뒤집기
			reverse(str);
			print_text(str);
			break;
		case 'e': // 동일 간격 특정 문자 일정 갯수 삽입 (3글자 후 @@ 삽입)
			if (a_inserted) {
				delete_a(str);
				a_inserted = 0;
			}
			else{
				insert_a(str);
				a_inserted = 1;
			}
			break;
		case 'f': // @@ 기준으로 문자 뒤집기
			reverse_a(str);
			print_text(str);
			break;
		case 'g': // 특정 문자를 다른 문자로 바꾸기
			fseek(stdin, 0, SEEK_END);
			printf("\n바꿀 문자, 새롭게 입력할 문자 : ");
			scanf("%c %c", &from, &to);
			change_char(str, from, to);
			print_text(str);
			break;
		case 'h': // 앞에서 읽었을 때, 뒤에서 읽었을 때 어디까지 같은 문자인지 출력
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