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

void insert_a(char str[][MAX_LENGTH]) {
	const char* input = "@@";
	for (int i = 0; i < 10; i++) {

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
	

	init_text(File, str);

	//print_text(str);

	while (1) {
		char order = 0;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);
		switch (order) {
		case 'd': // 뒤집기
			reverse(str);
			print_text(str);
			break;
		case 'e': // 동일 간격 특정 문자 일정 갯수 삽입 (3글자 후 @@ 삽입)

			break;
		case 'f': // @@ 기준으로 문자 뒤집기

			break;
		case 'g': // 특정 문자를 다른 문자로 바꾸기

			break;
		case 'h': // 앞에서 읽었을 때, 뒤에서 읽었을 때 어디까지 같은 문자인지 출력

			break;
		case 'q':
			exit(0);
			break;
		}
	}


	fclose(File);
	return 0;
}