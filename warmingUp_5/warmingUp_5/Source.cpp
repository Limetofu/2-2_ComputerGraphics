#include "Header.h"

#define MAX_SIZE 10

typedef struct DOT {
	int x;
	int y;
	int z;
	bool in;
} DOT;

typedef DOT element;
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
	for (int i = MAX_SIZE; i > 0; i--) {
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
			q->dot->x = q->dot->y = q->dot->z = 0;
			break;
		}
		if (i == MAX_SIZE - 1) {
			printf("지울 리스트가 없습니다.\n");
		}
	}
}

// d

void delete_rear(DequeType* q)
{
	for (int i = MAX_SIZE; i > 0; i--) {
		if (q->dot[i].in == 1) {
			q->dot[i].in = 0;
			q->dot->x = q->dot->y = q->dot->z = 0;
			break;
		}
		if (i == 1) {
			printf("지울 리스트가 없습니다.\n");
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
	for (int i = MAX_SIZE; i > 0; i--) {
		q->dot[i].in = 0;
		q->dot->x = q->dot->y = q->dot->z = 0;
	}
}

int zero_distance_max(DequeType* q) {
	// sqrt(x*x + y*y + z*z)
	int value[MAX_SIZE] = { 0, };

	for (int i = 0; i < MAX_SIZE; i++) {
		if (q->dot[i].in == 1) {
			value[i] = sqrt((q->dot[i].x * q->dot[i].x) + 
				(q->dot[i].y * q->dot[i].y) + (q->dot[i].z * q->dot[i].z));
		}
	}

	int max_value = 0;
	int max_i = MAX_SIZE + 1;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (value[i] > max_value) {
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
			value[i] = sqrt((q->dot[i].x * q->dot[i].x) +
				(q->dot[i].y * q->dot[i].y) + (q->dot[i].z * q->dot[i].z));
		}
	}

	int min_value = 100000;
	int min_i = MAX_SIZE + 1;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (value[i] < min_value) {
			min_value = value[i];
			min_i = i;
		}
	}

	return min_i;
}



int main() {

	DequeType head;

	init_deque(&head);

	//deque_print(&head);



	while (1) {
		char order;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &order);
		
		printf("\n");

		int k = 0, n = 0;

		int val1 = 0, val2 = 0, val3 = 0;

		switch (order) {
		case '+':
			fseek(stdin, 0, SEEK_END);
			printf("\n  넣을 값 : ");
			scanf("%d %d %d", &val1, &val2, &val3);
			add_front(&head, {val1, val2, val3, 1});

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
			if (k == MAX_SIZE + 1) printf("출력할 값이 없습니다.\n");
			else printf("원점에서 가장 가까운 거리 점 : (%d, %d, %d)\n", head.dot[k].x, head.dot[k].y, head.dot[k].z);
			//deque_print(&head);
			break;
		case 's': // 원점과의 거리 오름(또는 내림)차순 정렬 출력

			//deque_print(&head);
			break;
		case 'q':
			exit(0);
			break;
		}

	}

	return 0;
}