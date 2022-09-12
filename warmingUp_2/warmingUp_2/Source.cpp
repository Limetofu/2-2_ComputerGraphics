#include "Header.h"

#define MAX_LENGTH 300

int main(void) {
	FILE* File = fopen("data.txt", "r");
	if (File == NULL) {
		printf("파일이 없음\n");
		return 1;
	}

	char str[MAX_LENGTH];

	fgets(str, MAX_LENGTH, File);

	int num_count = 0;
	int char_count = 0;
	int upper_count = 0;

	bool is_number = 0, is_char = 0, is_upper = 0;



	while (fgets(str, MAX_LENGTH, File) != NULL) {

		// 공백 하나 당, 단어 +1 ..?
		// 숫자는 공백으로 취급하지 x
		// 공백 이전 글자가 뭔지에 따라 구분

		// 숫자인지, isdigit
		// 문자인지, isdigit
		//    --> 검사해서 bool값 하나 만들기

		// 대문자인지
		// 위 조건 만족할 때
		// isupper 만족하면 upper_cnt += 1

		// 마지막 문자인지는 어떻게 확인..? 
		// 문자열 마지막 번호에 다다르면 공백 나올때까지 뒤로 반복문 돌리기

		if (*str && str[strlen(str) - 1] == '\n') { // 끝 줄바꿈 문자 제거
			str[strlen(str) - 1] = 0;
		}

		printf("%s\n", str); // 문자열 출력

		int if_first = 0;
		for (int i = 0; i < strlen(str); i++) {
			is_number = 0, is_char = 0, is_upper = 0;

			// 마지막 문자일 때
			if (i == strlen(str) - 1) {

				// 처음 단어이자 마지막 문자일 때
				if (if_first == 0) {
					for (int k = i; k >= 0; k--) {
						if (isdigit(str[k])) is_number = 1;
						if (!isdigit(str[k])) is_char = 1;
						if (isupper(str[k])) is_upper = 1;
					}
				}

				else {
					for (int k = i; str[k] != ' '; k--) {
						if (isdigit(str[k])) is_number = 1;
						if (!isdigit(str[k])) is_char = 1;
						if (isupper(str[k])) is_upper = 1;
					}
				}
				
				if (is_number && !is_char) num_count++;
				else if (is_char) {
					char_count++;
					if (is_upper) upper_count++;
				}
			}

			if (str[i] == ' ') {
				
				// 처음 문자이면
				if (if_first == 0) {
					for (int k = i - 1; k >= 0; k--) {
						if (isdigit(str[k])) is_number = 1;
						if (!isdigit(str[k])) is_char = 1;
						if (isupper(str[k])) is_upper = 1;
					}
					if_first = 1;
				}

				else {
					for (int k = i - 1; str[k] != ' '; k--) {
						if (isdigit(str[k])) is_number = 1;
						if (!isdigit(str[k])) is_char = 1;
						if (isupper(str[k])) is_upper = 1;
					}
				}

				if (is_number && !is_char) num_count++;
				else if (is_char) {
					char_count++;
					if (is_upper) upper_count++;
				}
			}
		}
	}

	printf("\n");
	printf("word : %d\nnumber : %d\ncapital word : %d\n\n", char_count, num_count, upper_count);

	fclose(File);

	return 0;
}