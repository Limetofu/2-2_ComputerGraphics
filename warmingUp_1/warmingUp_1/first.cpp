#include "Header.h"

void print_information() {
	printf("--------------------------------------------------\n");
	printf("  m : °ö¼À\n");
	printf("  a : µ¡¼À\n");
	printf("  d : –E¼À\n");
	printf("  r : Çà·Ä½Ä\n");
	printf("  t : ÀüÄ¡\n");
	printf("  h : to 4x4, det\n");
	printf("  s : ·£´ý ¼³Á¤\n");
	printf("  q : Á¾·á\n");
	printf("--------------------------------------------------\n order : ");
}

void print_line() {
	printf("\n\n--------------------------------------------------\n\n");

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
	printf("\n  *  \n\n");

	print_array(array, 1);
	printf("\n  =  \n\n");

	int temp[3][3] = { 0, };
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				temp[j][i] += array[0][j][i] * array[1][i][k];
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			printf("%d ", temp[i][k]);
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
	det_array(transposed_array, 0, 1); // Çà·Ä Ãâ·Âµµ °°ÀÌ ÀÖ
	
	print_line();

	print_array(array, 1);
	printf("\n  ->  \n\n");

	det_array(transposed_array, 1, 1);
}

void quad_array_det(int array[2][4][4], int num) {

	int det = 0;
	int temp_array[4][3][3] = { 0, };

	for (int j = 0; j < 4; j++)
	for (int i = 0; i < 3; i++) { // i = Çà
		for (int k = 0; k < 3; k++) { // k = ¿­
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
		case 'm': // °ö¼À
			mult_array(array);
			print_line();
			break;
		case 'a': // µ¡¼À
			sum_array(array);
			print_line();
			break;
		case 'd': // »¬¼À
			dis_array(array);
			print_line();
			break;
		case 'r': // det (2°³ ÀüºÎ)
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

			//Ãâ·Â, detµµ °°ÀÌ Ãâ·ÂÇØ¾ß ÇÔ
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
		case 's': // »õ·Î ·£´ý
			srand(unsigned int(time(NULL)));
			for (int j = 0; j < 2; j++)
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						array[j][i][k] = rand() % 3;
			print_array(array, 0);
			print_array(array, 1);
			print_line();
			break;
		case 'q': // Á¾·á
			print_line();
			exit(0);
			break;
		default:
			break;
		}
	}
	


	return 0;
}