#include "Header.h"

#define MAX_LENGTH 300

int main(void) {
	FILE* File = fopen("data.txt", "r");
	if (File == NULL) {
		printf("������ ����\n");
		return 1;
	}

	char str[MAX_LENGTH];

	fgets(str, MAX_LENGTH, File);

	int num_count = 0;
	int char_count = 0;
	int upper_count = 0;

	bool is_number = 0, is_char = 0, is_upper = 0;



	while (fgets(str, MAX_LENGTH, File) != NULL) {

		// ���� �ϳ� ��, �ܾ� +1 ..?
		// ���ڴ� �������� ������� x
		// ���� ���� ���ڰ� ������ ���� ����

		// ��������, isdigit
		// ��������, isdigit
		//    --> �˻��ؼ� bool�� �ϳ� �����

		// �빮������
		// �� ���� ������ ��
		// isupper �����ϸ� upper_cnt += 1

		// ������ ���������� ��� Ȯ��..? 
		// ���ڿ� ������ ��ȣ�� �ٴٸ��� ���� ���ö����� �ڷ� �ݺ��� ������

		if (*str && str[strlen(str) - 1] == '\n') { // �� �ٹٲ� ���� ����
			str[strlen(str) - 1] = 0;
		}

		printf("%s\n", str); // ���ڿ� ���

		int if_first = 0;
		for (int i = 0; i < strlen(str); i++) {
			is_number = 0, is_char = 0, is_upper = 0;

			// ������ ������ ��
			if (i == strlen(str) - 1) {

				// ó�� �ܾ����� ������ ������ ��
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
				
				// ó�� �����̸�
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