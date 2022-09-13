#include "Header.h"

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
	
	int pos[2] = {0, 0};

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
			printf("\n\n Finished!       Score : \n\n");
		}


		// 다른 방향키 누르면 다시 pos 출력
		// 엔터 누르면 그 위치에 해당하는 알파벳 show값 1로 변경
	}


	return 0;
}