#include "Header.h"

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
		}
	}

	return 0;
}