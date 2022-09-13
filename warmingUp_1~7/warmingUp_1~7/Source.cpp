#define NUMB 7

#if NUMB == 1

#include "Header_1.h"

void print_information() {
	printf("--------------------------------------------------\n");
	printf("  m : 곱셈\n");
	printf("  a : 덧셈\n");
	printf("  d : 뺼셈\n");
	printf("  r : 행렬식\n");
	printf("  t : 전치\n");
	printf("  h : to 4x4, det\n");
	printf("  s : 랜덤 설정\n");
	printf("  q : 종료\n");
	printf("--------------------------------------------------\n order : ");
}

void print_line() {
	printf("\n--------------------------------------------------\n\n");

}

void print_array(int array[2][3][3], int j) {
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			printf("%d ", array[j][i][k]);
		}
		printf("\n");
	}
	printf("\n");
}

void sum_array(int array[2][3][3]) {

	print_array(array, 0);
	printf("\n  +  \n\n");

	print_array(array, 1);
	printf("\n  =  \n\n");

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			printf("%d ", (array[0][i][k]) + array[1][i][k]);
		}
		printf("\n");
	}
}

void dis_array(int array[2][3][3]) {

	print_array(array, 0);
	printf("\n  -  \n\n");

	print_array(array, 1);
	printf("\n  =  \n\n");

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			printf("%d ", (array[0][i][k]) - array[1][i][k]);
		}
		printf("\n");
	}
}

void mult_array(int array[2][3][3]) {

	print_array(array, 0);
	printf("\n  *  \n");

	print_array(array, 1);
	printf("\n  =  \n");



	int result[3][3] = { 0, }, temp = 0;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			temp = 0;
			for (int i = 0; i < 3; i++)
				temp += array[0][y][i] * array[1][i][x];
			result[y][x] = temp;
		}
	}
	

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			printf("%d ", result[i][k]);
		}
		printf("\n");
	}
	printf("\n");
}

int det_array(int array[2][3][3], int j, bool if_print) {

	int det = array[j][0][0] * (array[j][1][1] * array[j][2][2] - (array[j][1][2] * array[j][2][1]))
		- array[j][0][1] * (array[j][1][0] * array[j][2][2] - (array[j][1][2] * array[j][2][0]))
		+ array[j][0][2] * (array[j][1][0] * array[j][2][1] - (array[j][1][1] * array[j][2][0]));

	if (if_print) {
		print_array(array, j);
		printf("\n  det = %d\n\n", det);
	}

	return det;
}

void trans_array(int array[2][3][3]) {
	int transposed_array[2][3][3] = { 0, };
	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				transposed_array[j][i][k] = array[j][k][i];
			}
		}

	print_array(array, 0);
	printf("\n  ->  \n\n");

	//print_array(transposed_array, 0);
	det_array(transposed_array, 0, 1); // 행렬 출력도 같이 있

	print_line();

	print_array(array, 1);
	printf("\n  ->  \n\n");

	det_array(transposed_array, 1, 1);
}

void quad_array_det(int array[2][4][4], int num) {
	int det = 0;
	int temp_array[4][3][3] = { 0, };

	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 3; i++) { // i = 행
			for (int k = 0; k < 3; k++) { // k = 열
				if (j == 0)
					temp_array[j][i][k] = array[num][i + 1][k + 1];
				else if (j == 1) {
					if (i == 0)
						temp_array[j][i][k] = array[num][i + 1][k];
					else
						temp_array[j][i][k] = array[num][i + 1][k + 1];
				}
				else if (j == 2) {
					if (i <= 1)
						temp_array[j][i][k] = array[num][i + 1][k];
					else
						temp_array[j][i][k] = array[num][i + 1][k + 1];
				}
				else
					temp_array[j][i][k] = array[num][i + 1][k];
			}
		}

	for (int i = 0; i < 4; i++) {
		det += array[num][0][i] * (int)pow((double)-1, (double)(2 + i))
			* det_array(temp_array, 0, 0);
	}

	printf("\n  det = %d\n\n", det);
}

int main(void) {
	int array[2][3][3] = { 0, };
	int array_4[2][4][4] = { 0, };

	srand(unsigned int(time(NULL)));

	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < 3; k++)
				array[j][i][k] = rand() % 3;

	print_array(array, 0);
	print_array(array, 1);
	
	print_information();

	while (1) {
		char order;
		scanf("%c", &order);

		printf("\n\n");

		switch (order) {
		case 'm': // 곱셈
			mult_array(array);
			print_line();
			break;
		case 'a': // 덧셈
			sum_array(array);
			print_line();
			break;
		case 'd': // 뺄셈
			dis_array(array);
			print_line();
			break;
		case 'r': // det (2개 전부)
			det_array(array, 0, 1);
			det_array(array, 1, 1);
			print_line();
			break;
		case 't': // transpose
			trans_array(array);
			print_line();
			break;
		case 'h': // to 4x4
			for (int j = 0; j < 2; j++)
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						array_4[j][i][k] = array[j][i][k];
			array_4[0][3][3] = 1;
			array_4[1][3][3] = 1;

			//출력, det도 같이 출력해야 함
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 4; i++) {
					for (int k = 0; k < 4; k++) {
						printf("%d ", array_4[j][i][k]);
					}
					printf("\n");
				}
				quad_array_det(array_4, j);
				printf("\n\n\n");
			}

			print_line();
			break;
		case 's': // 새로 랜덤
			srand(unsigned int(time(NULL)));
			for (int j = 0; j < 2; j++)
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						array[j][i][k] = rand() % 3;
			print_array(array, 0);
			print_array(array, 1);
			print_line();
			break;
		case 'q': // 종료
			print_line();
			exit(0);
			break;
		default:
			break;
		}
	}



	return 0;
}


#elif NUMB == 2

#include "Header_2.h"

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


#elif NUMB == 3

#include "Header_3.h"

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
	char tempstr[MAX_LENGTH] = { '\0' };
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
			else {
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


#elif NUMB == 4

#include "Header_4.h"
#define MOVING_VALUE 40

void print_rect(RECT rect_1, RECT rect_2) {
	system("cls");

	printf("Rect 1 : (%3d, %3d) (%3d, %3d)\n",
		rect_1.left, rect_1.top, rect_1.right, rect_1.bottom);
	printf("Rect 2 : (%3d, %3d) (%3d, %3d)\n",
		rect_2.left, rect_2.top, rect_2.right, rect_2.bottom);
	RECT temp;

	// 두 개가 겹치면
	if (IntersectRect(&temp, &rect_1, &rect_2)) {
		printf("  Rectangle 1 & Rectangle 2 collide!\n");
	}
}

void move_up(RECT* rect) {
	if (rect->top - MOVING_VALUE < 0) {
		printf("out of range\n");
	}
	else {
		rect->top -= MOVING_VALUE;
		rect->bottom -= MOVING_VALUE;
	}
}

void move_down(RECT* rect) {
	if (rect->bottom + MOVING_VALUE > 600) {
		printf("out of range\n");
	}
	else {
		rect->top += MOVING_VALUE;
		rect->bottom += MOVING_VALUE;
	}
}

void move_left(RECT* rect) {
	if (rect->left - MOVING_VALUE < 0) {
		printf("out of range\n");
	}
	else {
		rect->left -= MOVING_VALUE;
		rect->right -= MOVING_VALUE;
	}
}

void move_right(RECT* rect) {
	if (rect->right + MOVING_VALUE > 800) {
		printf("out of range\n");
	}
	else {
		rect->right += MOVING_VALUE;
		rect->left += MOVING_VALUE;
	}
}

int main() {

	srand(unsigned int(time(NULL)));

	static RECT rect_1 = { 0, };
	static RECT rect_2 = { 0, };

	// 800 x 600

	rect_1.left = rand() % 200;
	rect_1.top = rand() % 200;
	rect_1.right = rand() % 400 + 200;
	rect_1.bottom = rand() % 400 + 200;

	rect_2.left = rand() % 100 + 300;
	rect_2.top = rand() % 200 + 100;
	rect_2.right = rand() % 300 + 500;
	rect_2.bottom = rand() % 400 + 200;

	print_rect(rect_1, rect_2);



	while (1) {
		printf("\n-------------------------------------\n");
		printf("command : ");

		char order;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);


		printf("-------------------------------------\n\n");


		switch (order) {
		case 'w':
			move_up(&rect_1);
			print_rect(rect_1, rect_2);
			break;
		case 'a':
			move_left(&rect_1);
			print_rect(rect_1, rect_2);
			break;
		case 's':
			move_down(&rect_1);
			print_rect(rect_1, rect_2);
			break;
		case 'd':
			move_right(&rect_1);
			print_rect(rect_1, rect_2);
			break;

			/*--------------------------------------*/

		case 'i':
			move_up(&rect_2);
			print_rect(rect_1, rect_2);
			break;
		case 'j':
			move_left(&rect_2);
			print_rect(rect_1, rect_2);
			break;
		case 'k':
			move_down(&rect_2);
			print_rect(rect_1, rect_2);
			break;
		case 'l':
			move_right(&rect_2);
			print_rect(rect_1, rect_2);
			break;

		case 'q':
			exit(0);
			break;
		}
	}




	return 0;
}


#elif NUMB == 5

#include "Header_5.h"

#define MAX_SIZE 10

typedef struct DOT {
	int x;
	int y;
	int z;
	int in;
} DOT;

typedef DOT element;

typedef struct DOT_D {
	int x;
	int y;
	int z;
	int in;
	int dis;
};

typedef DOT_D element_d;

typedef struct {
	element dot[MAX_SIZE];
	int front, rear;
} DequeType;

void error(const char* message)
{
	fprintf(stderr, "%s\n", message);
	//exit(1);
}

void init_deque(DequeType* q) {
	q->front = q->rear = 0;
	for (int i = 0; i < MAX_SIZE; i++) {
		q->dot[i].in = 0;
	}
}

int is_empty(DequeType* q)
{
	return (q->front == q->rear);
}

int is_full(DequeType* q)
{
	return ((q->rear + 1) % MAX_SIZE == q->front);
}

// 앞에서 뒤로
// 필요한 건 뒤에서 앞으로. 거꾸로
void deque_print(DequeType* q)
{
	int temp;
	// printf("DEQUE(front=%d rear=%d) = ", q->front, q->rear);
	for (int j = 0; j < MAX_SIZE; j++) {
		if (q->dot[j].in == 0) {
			printf(" %d ||               ||\n", j);
		}
		else {
			printf(" %d ||  %3d %3d %3d  ||\n", j,
				q->dot[j].x, q->dot[j].y, q->dot[j].z);
		}
	}
	/*
	int i = q->front;
	do {
		i = (i + 1) % (MAX_SIZE);
		if (q->dot[i].x != NULL)
		printf(" %d ||  %3d %3d %3d  ||\n", i,
			q->dot[i].x, q->dot[i].y, q->dot[i].z);
		if (i == q->rear) {

			break;
		}

	} while (i != q->front);

	*/

	printf("\n");
}

// e

void add_rear(DequeType* q, element item)
{
	for (int i = MAX_SIZE - 1; i >= 0; i--) {
		if (q->dot[i].in == 0) {
			q->dot[i] = item;
			break;
		}
		if (i == 1) {
			printf("리스트를 넣을 칸이 없습니다.\n");
		}
	}
}

// + 

void add_front(DequeType* q, element val)
{
	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 0) {
			q->dot[i] = val;
			break;
		}
		if (i == MAX_SIZE - 1) {
			printf("리스트를 넣을 칸이 없습니다.\n");
		}
	}
}


// -

void delete_front(DequeType* q)
{
	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 1) {
			q->dot[i].in = 0;
			q->dot[i].x = q->dot[i].y = q->dot[i].z = 0;
			break;
		}
	}
}

// d

void delete_rear(DequeType* q)
{
	for (int i = MAX_SIZE - 1; i >= 0; i--) {
		if (q->dot[i].in == 1) {
			q->dot[i].in = 0;
			q->dot[i].x = q->dot[i].y = q->dot[i].z = 0;
			break;
		}
	}
}

int print_num(DequeType* q) {
	int count = 0;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 1) {
			count++;
		}
	}
	return count;
}

void delete_all(DequeType* q) {
	for (int i = MAX_SIZE - 1; i >= 0; i--) {
		q->dot[i].in = 0;
		q->dot[i].x = q->dot[i].y = q->dot[i].z = 0;
	}
}

int zero_distance_max(DequeType* q) {
	// sqrt(x*x + y*y + z*z)
	int value[MAX_SIZE] = { 0, };

	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 1) {
			value[i] = sqrt(pow(q->dot[i].x, 2) +
				pow(q->dot[i].y, 2) + pow(q->dot[i].z, 2));
		}
	}

	int max_value = 0;
	int max_i = MAX_SIZE + 1;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (value[i] > max_value && q->dot[i].in) {
			max_value = value[i];
			max_i = i;
		}
	}

	return max_i;
}


int zero_distance_min(DequeType* q) {
	// sqrt(x*x + y*y + z*z)
	int value[MAX_SIZE] = { 0, };

	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 1) {
			value[i] = sqrt(pow(q->dot[i].x, 2) +
				pow(q->dot[i].y, 2) + pow(q->dot[i].z, 2));
		}
	}

	int min_value = 1000000;
	int min_i = MAX_SIZE + 1;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (value[i] < min_value && q->dot[i].in) {
			min_value = value[i];
			min_i = i;
		}
	}

	return min_i;
}

/*
void selection_sort(element_d sorted[]) {
	int minIndex;
	for (int i = 0; i < MAX_SIZE - 1; i++) {
		minIndex = i;
		for (int j = i + 1; j < MAX_SIZE; j++)
			if (sorted[j].dis < sorted[minIndex].dis)
				minIndex = i;
		swap(&sorted[i].x, &sorted[minIndex].x);
		swap(&sorted[i].y, &sorted[minIndex].y);
		swap(&sorted[i].z, &sorted[minIndex].z);
		swap(&sorted[i].in, &sorted[minIndex].in);
	}
} */

void sort_distance(DequeType* q) {
	element_d sorted[MAX_SIZE] = { 0, };

	for (int i = 0; i < MAX_SIZE; i++) {
		sorted[i].x = q->dot[i].x;
		sorted[i].y = q->dot[i].y;
		sorted[i].z = q->dot[i].z;
		if (q->dot[i].in == 1)
			sorted[i].in = 1;
		sorted[i].dis = pow(q->dot[i].x, 2) +
			pow(q->dot[i].y, 2) + pow(q->dot[i].z, 2);
	}

	element_d temp = { 0, };
	for (int i = 0; i < MAX_SIZE - 1; i++)
	{
		for (int j = 0; j < MAX_SIZE - 1 - i; j++)
		{
			if (sorted[j].dis > sorted[j + 1].dis)
			{
				temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
		}
	}

	int count = 0;
	for (int j = 0; j < MAX_SIZE; j++) {
		if (sorted[j].in == 0) {
			// printf(" %d ||               ||\n", j);
		}
		else {
			printf(" %d ||  %3d %3d %3d  ||\n", count,
				sorted[j].x, sorted[j].y, sorted[j].z);
			count++;
		}
	}

	for (int i = count + 1; i < MAX_SIZE; i++) {
		printf(" %d ||               ||\n", i);
	}
}

int main() {

	DequeType head;

	init_deque(&head);

	bool print_sorted = 1;

	while (1) {
		char order;
		printf("---------------------------------\n");

		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);

		printf("---------------------------------\n");

		printf("\n");

		int k = 0, n = 0;

		int val1 = 0, val2 = 0, val3 = 0;



		switch (order) {
		case '+':
			fseek(stdin, 0, SEEK_END);
			printf("\n  넣을 값 : ");
			scanf("%d %d %d", &val1, &val2, &val3);
			add_front(&head, { val1, val2, val3, 1 });

			deque_print(&head);
			break;
		case '-':
			delete_front(&head);
			deque_print(&head);
			break;
		case 'e':
			fseek(stdin, 0, SEEK_END);
			printf("\n  넣을 값 : ");
			scanf("%d %d %d", &val1, &val2, &val3);
			add_rear(&head, { val1, val2, val3, 1 });

			deque_print(&head);
			break;
		case 'd':
			delete_rear(&head);
			deque_print(&head);
			break;
		case 'l': // 리스트에 저장된 점 갯수 출력 (리스트 길이)
			printf("리스트 길이 : %d\n", print_num(&head));
			break;
		case 'c': // 리스트 비우기
			delete_all(&head);
			deque_print(&head);
			break;
		case 'm': // 원점에서 가장 먼 거리 점 좌표
			k = zero_distance_max(&head);
			if (k == MAX_SIZE + 1) printf("출력할 값이 없습니다.\n");
			else printf("원점에서 가장 먼 거리 점 : (%d, %d, %d)\n", head.dot[k].x, head.dot[k].y, head.dot[k].z);
			//deque_print(&head);
			break;
		case 'n': // 원점에서 가장 가까운 거리 점 좌표
			n = zero_distance_min(&head);
			if (n == MAX_SIZE + 1) printf("출력할 값이 없습니다.\n");
			else printf("원점에서 가장 가까운 거리 점 : (%d, %d, %d)\n", head.dot[n].x, head.dot[n].y, head.dot[n].z);
			//deque_print(&head);
			break;
		case 's': // 원점과의 거리 오름(또는 내림)차순 정렬 출력
			if (print_sorted) {
				if (print_num(&head) == 0) {
					printf("출력할 값이 없습니다.\n");
				}
				else {
					sort_distance(&head);
					print_sorted = 0;
				}
			}
			else {
				if (print_num(&head) == 0) {
					printf("출력할 값이 없습니다.\n");
				}
				else {
					deque_print(&head);
					print_sorted = 1;
				}
			}

			break;
		case 'q':
			exit(0);
			break;
		}

	}

	return 0;
}


#elif NUMB == 6

#include "Header_6.h"

#define SIZE 30

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

int limit_w = 0;
int limit_a = 0;
int limit_s = 0;
int limit_d = 0;

// 최소를 정해야 하는지?

int is_possible(int cur[], int num) {
	// 가는 위치는 중복 가능
	// 30x30 그리드 밖으로 나가는 것은 불가
	switch (num) {
	case 0: // w
		return (cur[1] >= 1 && (limit_w <= 8 || limit_w >= 1));
		break;
	case 1: // a
		return (cur[0] >= 1 && (limit_a <= 8 || limit_a >= 1));
		break;
	case 2: // s
		return (cur[1] <= 28 && (limit_s <= 8 || limit_s >= 1));
		break;
	case 3: // d
		return (cur[0] <= 28 && (limit_d <= 8 || limit_d >= 1));
		break;
	}

	return 0;
}

void move_w(int* x, int* y) {
	*y -= 1;
}

void move_a(int* x, int* y) {
	*x -= 1;
}

void move_s(int* x, int* y) {
	*y += 1;
}

void move_d(int* x, int* y) {
	*x += 1;
}


void print_grid(int arr[][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int k = 0; k < SIZE; k++) {
			printf(" %5d", arr[i][k]);
		}
		printf("\n");
	}
}



void choose_direction(int arr[][SIZE], int cur[], int* count) {
	switch (rand() % 4) {
	case 0:
		if (is_possible(cur, 0)) {
			limit_a = limit_s = limit_d = 0;
			move_w(&cur[0], &cur[1]);
			*count += 1;
			arr[cur[0]][cur[1]] = *count;
		}
		else {
			choose_direction(arr, cur, count);
		}
		break;
	case 1:
		if (is_possible(cur, 1)) {
			limit_w = limit_s = limit_d = 0;
			move_a(&cur[0], &cur[1]);
			*count += 1;
			arr[cur[0]][cur[1]] = *count;
		}
		else {
			choose_direction(arr, cur, count);
		}
		break;
	case 2:
		if (is_possible(cur, 2)) {
			limit_w = limit_a = limit_d = 0;
			move_s(&cur[0], &cur[1]);
			*count += 1;
			arr[cur[0]][cur[1]] = *count;
		}
		else {
			choose_direction(arr, cur, count);
		}
		break;
	case 3:
		if (is_possible(cur, 3)) {
			limit_w = limit_a = limit_s = 0;
			move_d(&cur[0], &cur[1]);
			*count += 1;
			arr[cur[0]][cur[1]] = *count;
		}
		else {
			choose_direction(arr, cur, count);
		}
		break;
	}
}

void find_path(int arr[][SIZE], int cur[]) {
	int count = 1;

	while (1) {
		choose_direction(arr, cur, &count);
		if (cur[0] == SIZE - 1 && cur[1] == SIZE - 1) {
			break;
		}
	}
}

int main() {
	srand(unsigned int(time(NULL)));

	int arr[SIZE][SIZE] = { 0, };

	// 현재 위치 나타내는 함수
	int cur[2] = { 0, 0 };
	arr[0][0] = 1;

	find_path(arr, cur);
	print_grid(arr);


	char order = 0;

	while (1) {

		printf("\n-------------------------------\n Command : ");
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);
		printf("\n-------------------------------\n");

		switch (order) {
		case 'r':
			for (int i = 0; i < SIZE; i++)
				for (int k = 0; k < SIZE; k++)
					arr[i][k] = 0;
			arr[0][0] = 1;
			cur[0] = cur[1] = 0;
			find_path(arr, cur);
			print_grid(arr);
			break;
		case 'a': // 좌측
			for (int i = 0; i < SIZE; i++) {
				for (int k = 0; k < SIZE - 1; k++) {
					SWAP(arr[i][k], arr[i][k + 1], int);
				}
			}
			print_grid(arr);


			break;
		case 'd': // 우측
			for (int i = 0; i < SIZE; i++) {
				for (int k = SIZE - 1; k > 1; k--) {
					SWAP(arr[i][k], arr[i][k - 1], int);
				}
			}
			print_grid(arr);

			break;
		case 'q':

			exit(0);

			break;
		}
	}

	return 0;
}


#elif NUMB == 7

#include "Header_7.h"
#define TIME_TO_SHOW 2000

// 구조체 하나 둬서
// 알파벳 하나씩 두기.
// int pos[2][2]


typedef struct {
	int num;
	int pos[2][2];
} ALPHA;


// 4x4는 배열로 구현
// 필요한 것? : 알파벳 alpha(num), 

// int show
// show = 0 : *
// show = 1 : 잠시 눌렀을 때.
// show = 2 : 맞춤. 계속 보여주기

typedef struct {
	int alpha;
	int show;
} GRID;

ALPHA A[8] = { 0, };
GRID G[4][4] = { 0, };
bool show_cursor = 0;

bool is_possible(int inserted[], int num, int x, int y) {
	// inserted가 2일 때 false
	// grid에 이미 다른 알파벳이 찼을 때 (alpha == 8) false
	return (inserted[num] <= 1 && G[x][y].alpha == 8);
}

int set_color(int x, int y) {
	switch (G[x][y].alpha) {
	case 0:
		return 4;
		break;
	case 1:
		return 6;
		break;
	case 2:
		return 14;
		break;
	case 3:
		return 10;
		break;
	case 4:
		return 3;
		break;
	case 5:
		return 8;
		break;
	case 6:
		return 13;
		break;
	case 7:
		return 15;
		break;
	}
}

void print_grid(int pos[]) {
	system("cls");

	// 알파벳 당 색 배정.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("    a  b  c  d\n");

	for (int i = 0; i < 4; i++) { // i : y
								  // k : x
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf(" %d ", i + 1);
		for (int k = 0; k < 4; k++) {
			if (show_cursor && pos[0] == i && pos[1] == k) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf(" O ");
			}
			else if (G[i][k].show == 1 || G[i][k].show == 2) {
				int color = set_color(i, k);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
				switch (G[i][k].alpha) {
				case 0:
					printf(" A ");
					break;
				case 1:
					printf(" B ");
					break;
				case 2:
					printf(" C ");
					break;
				case 3:
					printf(" D ");
					break;
				case 4:
					printf(" E ");
					break;
				case 5:
					printf(" F ");
					break;
				case 6:
					printf(" G ");
					break;
				case 7:
					printf(" H ");
					break;
				}
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				printf(" * ");
			}


		}
		printf("\n");
	}
}

void init_pos(int inserted[], int i_start, int k_start) {

	int al = 8;
	int rand_value = rand() % 8;

	int if_error = 0;
	bool cont = 1;

	// pos 초기화, 랜덤 배치
	for (int i = i_start; i < 4; i++) {
		for (int k = k_start; k < 4; k++) {
			cont = 1;
			rand_value = rand() % 8;
			while (cont) {
				switch (rand_value) {
				case 0:
					al = 0;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[0]][0] = i; // inserted가능 : 0, 1
						A[al].pos[inserted[0]][1] = k;
						inserted[al]++;
						cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 1:
					al = 1;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[1]][0] = i;
						A[al].pos[inserted[1]][1] = k;
						inserted[al]++;
						cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 2:
					al = 2;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[2]][0] = i;
						A[al].pos[inserted[2]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 3:
					al = 3;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[3]][0] = i;
						A[al].pos[inserted[3]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 4:
					al = 4;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[4]][0] = i;
						A[al].pos[inserted[4]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 5:
					al = 5;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[5]][0] = i;
						A[al].pos[inserted[5]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 6:
					al = 6;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[6]][0] = i;
						A[al].pos[inserted[6]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value++;
						continue;
					}
					break;
				case 7:
					al = 7;
					if (is_possible(inserted, al, i, k)) {
						G[i][k].alpha = al;
						A[al].pos[inserted[7]][0] = i;
						A[al].pos[inserted[7]][1] = k;
						inserted[al]++; cont = 0;
					}
					else {
						rand_value = 0;
						continue;
					}
					break;
				}
			}
		}
	}
}

void init_grid(int inserted[]) {
	// 8가지 알파벳.
	// A B C D E F G H

	// num 고유값 초기화
	for (int i = 0; i < 8; i++) {
		A[i].num = i;
	}

	// GRID alpha값 빈칸으로 초기화
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			G[i][k].alpha = 8;
		}
	}
}

int main() {
	srand(unsigned int(time(NULL)));

	int pos[2] = { 0, 0 };

	int inserted[8] = { 0, };

	init_grid(inserted);
	init_pos(inserted, 0, 0);

	show_cursor = 0;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			G[i][k].show = 1;
		}
	}
	print_grid(pos);

	Sleep(TIME_TO_SHOW);

	show_cursor = 1;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			G[i][k].show = 0;
		}
	}

	print_grid(pos);

	int enter_cnt = 0;
	int showed_alpha_num = 8;
	int enter_x = 0, enter_y = 0;
	char order = 0;

	int score = 100;

	while (1) {
		printf("\n\n  Command : ");
		fseek(stdin, 0, SEEK_END);
		order = _getch();
		switch (order) {
		case 72: // w
			show_cursor = 1;
			if (pos[0] >= 1) {
				pos[0]--;
			}
			break;
		case 75: // a
			show_cursor = 1;
			if (pos[1] >= 1) {
				pos[1]--;
			}
			break;
		case 80: // s
			show_cursor = 1;
			if (pos[0] <= 2) {
				pos[0]++;
			}
			break;
		case 77: // d
			show_cursor = 1;
			if (pos[1] <= 2) {
				pos[1]++;
			}
			break;
		case 13: // ENTER
			if (enter_cnt == 0 && (G[pos[0]][pos[1]].show == 0)) {
				show_cursor = 0;
				G[pos[0]][pos[1]].show = 1;
				showed_alpha_num = G[pos[0]][pos[1]].alpha;
				enter_x = pos[0];
				enter_y = pos[1];
				enter_cnt++;
			}

			else if (enter_cnt == 1 && (G[pos[0]][pos[1]].show == 0)) {

				if (G[pos[0]][pos[1]].alpha == showed_alpha_num) {
					// 두 문자가 같으면
					show_cursor = 0;
					G[pos[0]][pos[1]].show = 2;
					G[enter_x][enter_y].show = 2;
				}
				else {
					// 두 문자가 다르면
					show_cursor = 0;
					G[pos[0]][pos[1]].show = 1;
					print_grid(pos);
					Sleep(TIME_TO_SHOW);
					G[pos[0]][pos[1]].show = 0;
					G[enter_x][enter_y].show = 0;
					if (score - 5 >= 0) score -= 5;
				}

				enter_cnt = 0;
			}
			break;

		case 'q':
			printf("\n");
			exit(0);
			break;

		case 'r':
			pos[0] = pos[1] = 0;
			for (int i = 0; i < 8; i++)
				inserted[i] = 0;

			init_grid(inserted);
			init_pos(inserted, 0, 0);

			show_cursor = 0;
			for (int i = 0; i < 4; i++) {
				for (int k = 0; k < 4; k++) {
					G[i][k].show = 1;
				}
			}
			print_grid(pos);

			Sleep(TIME_TO_SHOW);

			show_cursor = 1;
			for (int i = 0; i < 4; i++) {
				for (int k = 0; k < 4; k++) {
					G[i][k].show = 0;
				}
			}

			print_grid(pos);

			enter_cnt = 0;
			showed_alpha_num = 8;
			enter_x = 0, enter_y = 0;
			order = 0;

			break;
		}

		print_grid(pos);

		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				if (G[i][k].show == 2) cnt++;
			}
		}

		if (cnt >= 16) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("\n\n Finished!       Score : %d\n\n", score);
		}


		// 다른 방향키 누르면 다시 pos 출력
		// 엔터 누르면 그 위치에 해당하는 알파벳 show값 1로 변경
	}


	return 0;
}




#endif