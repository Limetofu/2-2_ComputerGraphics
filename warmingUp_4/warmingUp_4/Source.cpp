#include "Header.h"
#define MOVING_VALUE 40

void print_rect(RECT rect_1, RECT rect_2) {
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

void move_up(RECT *rect) {
	if (rect->top - MOVING_VALUE < 0) {
		printf("out of range\n");
	}
	else {
		rect->top -= MOVING_VALUE;
		rect->bottom -= MOVING_VALUE;
	}
}

void move_down(RECT *rect) {
	if (rect->bottom + MOVING_VALUE > 600) {
		printf("out of range\n");
	}
	else {
		rect->top += MOVING_VALUE;
		rect->bottom += MOVING_VALUE;
	}
}

void move_left(RECT *rect) {
	if (rect->left - MOVING_VALUE < 0) {
		printf("out of range\n");
	}
	else {
		rect->left -= MOVING_VALUE;
		rect->right -= MOVING_VALUE;
	}
}

void move_right(RECT *rect) {
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