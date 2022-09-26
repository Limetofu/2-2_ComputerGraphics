#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define GWIDTH 800
#define GHEIGHT 800

#define NUM 3

#if NUM == 3

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.0f, 0.0f, 0.0f };

typedef struct RECTANGLE {
	float left;
	float top;
	float right;
	float bottom;
	int selected;
	int prior;
	bool show;
	COLORVALUE color;
};

typedef struct INT_RECT {
	int left;
	int top;
	int right;
	int bottom;
};

RECTANGLE rect[5] = { 0, };
INT_RECT int_rect[5] = { 0, };
static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void random_color(COLORVALUE color);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void convert_deviceXY_to_openglXY(int x, int y, float* ox, float* oy);
void convert_openglXY_to_deviceXY(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(GWIDTH, GHEIGHT); // �������� ũ�� ����
	glutCreateWindow("Exam 1"); // ������ ���� (������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	
	for (int i = 0; i < 5; i++) {
		rect[i].left = rect[i].top = -0.2f;
		rect[i].right = rect[i].bottom = 0.2f;
		rect[i].prior = -1;
	}

	// int������ ��ǥ ��ȯ
	// float���� RECTANGLE ����ü�� ���ԵǾ�����
	for (int i = 0; i < 5; i++) {
		convert_openglXY_to_deviceXY(&int_rect[i].left, &int_rect[i].top, rect[i].left, rect[i].top);
		convert_openglXY_to_deviceXY(&int_rect[i].right, &int_rect[i].bottom, rect[i].right, rect[i].bottom);
	}

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	

	glClearColor(backg_color.r, backg_color.g, backg_color.b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	
	rect[0].color = { 1.0f, 0.0f, 0.0f }; // r
	rect[1].color = { 0.0f, 1.0f, 0.0f }; // g
	rect[2].color = { 0.0f, 0.0f, 1.0f }; // b
	rect[3].color = { 1.0f, 1.0f, 1.0f }; // w
	rect[4].color = { 1.0f, 1.0f, 0.0f }; // y
	
	int wanted_prior = 5 - showed_rect_count;

	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 5; k++) {
			if (rect[k].prior == wanted_prior && rect[k].show) {
				print_rect(k);
				wanted_prior++;
				break;
			}
		}
	}

	// ���� ���� �������� ����ؾ���.
	
	for (int i = 0; i < 5; i++) {
		if (rect[i].selected == 1) {
			print_rect(i);
		}
	} 

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	srand(time(NULL));

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = true;
		beforeX = x, beforeY = y;
		int max_prior = 0, big_rect_num = 0, count = 0;

		for (int i = 0; i < 5; i++) {
			if (x >= int_rect[i].left && x <= int_rect[i].right && y >= int_rect[i].top && y <= int_rect[i].bottom) {
				count++;
				if (rect[i].prior >= max_prior) {
					big_rect_num = i;
					max_prior = rect[i].prior;
				}
			}
		}

		// (1) - rect �ϳ��� �� ���� ��
		// (2) - rect 2�� ~ 4�� ���̷� �� ���� ��
		// (3) - rect 5�� ���� ������ ��

		if (count > 0) // ���콺�� Ŭ���� rect�� �ϳ� �̻��� ��
		{
			rect[big_rect_num].selected = TRUE;
			rect[big_rect_num].prior = 4;

			switch (max_prior) {
			case 0:
				for (int i = showed_rect_count - 1; i >= 0; i--) {
					if (rect[i].prior != -1)
						if (i != big_rect_num)
						rect[i].prior--;
				}
				break;
			case 1:
				for (int i = showed_rect_count - 1; i >= 0; i--) {
					if (rect[i].prior != 0 && rect[i].prior != -1)
						if (i != big_rect_num)
						rect[i].prior--;
				}
				break;
			case 2:
				for (int i = showed_rect_count - 1; i >= 0; i--) {
					if (rect[i].prior != 0 && rect[i].prior != 1 && rect[i].prior != -1)
						if (i != big_rect_num)
						rect[i].prior--;
				}
				break;
			case 3:
				for (int i = showed_rect_count - 1; i >= 0; i--) {
					if (rect[i].prior != 0 && rect[i].prior != 1 && rect[i].prior != 2 && rect[i].prior != -1)
						if (i != big_rect_num)
						rect[i].prior--;
				}
				break;
			case 4:
				
				break;
			}
		}
	}
		
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// ���콺 ��ư�� �� ��
		left_button = false;

		for (int i = 0; i < 5; i++) {
			rect[i].selected = false;
		}
	}
		
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button = true;
	}
		
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		right_button = false;
	}
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		for (int i = 0; i < 5; i++) {
			if (rect[i].selected == 1) {
				int_rect[i].left += afterX;
				int_rect[i].top += afterY;
				int_rect[i].right += afterX;
				int_rect[i].bottom += afterY;
			}
		}

		beforeX = x, beforeY = y;
		// ���콺 xy�� �簢�� �ȿ� �� ������,
		for (int i = 0; i < 5; i++) {
			if (rect[i].selected == 1) {
				convert_deviceXY_to_openglXY(int_rect[i].left, int_rect[i].top, &rect[i].left, &rect[i].top);
				convert_deviceXY_to_openglXY(int_rect[i].right, int_rect[i].bottom, &rect[i].right, &rect[i].bottom);
			}
		}
	}

	// ������ ���콺�� ������ ��
	if (right_button == true) {
		;
	}
	else {
		;
	}

	glutPostRedisplay();
}

void convert_deviceXY_to_openglXY(int x, int y, float* ox, float* oy) {
	int w = GWIDTH;
	int h = GHEIGHT;
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void convert_openglXY_to_deviceXY(int* x, int* y, float ox, float oy) {
	float w = GWIDTH;
	float h = GHEIGHT;
	*x = (int)((ox * w + w) / 2);
	*y = (int)((oy * w + w) / 2);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'A': case 'a':
		// �簢�� ����
		for (int i = 0; i < 5; i++) { // i = 3
			if (rect[i].show == 0) {
				rect[i].show = 1;
				rect[i].prior = 4;
				for (int k = i - 1; k >= 0; k--) {
					rect[k].prior--;
				}
				showed_rect_count++;
				break;
			}
		}

		// show�Ӹ� �ƴ϶�... prior�� ���������
		// �켱���� Ŭ����, ���� ���õ�

		// �簢�� ���� ������? prior�� ���� �������� ��.
		// �Ʒ��� �ִ� �͵��� ��� prior-- 

		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void print_rect(int i) {
	glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}

void random_color(COLORVALUE color) {
	color.r = (float)rand() / (RAND_MAX);
	color.g = (float)rand() / (RAND_MAX);
	color.b = (float)rand() / (RAND_MAX);
}

#elif NUM == 4

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.2f, 0.2f, 0.2f };

typedef struct RECTANGLE {
	float left;
	float top;
	float right;
	float bottom;
	int selected;
	int prior;
	bool show;
	int to;
	int zig_to;
	int zig_count;
	int shape_count_X;
	int shape_count_Y;
	int origin_X;
	int origin_Y;
	COLORVALUE color;
};

typedef struct INT_RECT {
	int left;
	int top;
	int right;
	int bottom;
};

RECTANGLE rect[5] = { 0, };
INT_RECT int_rect[5] = { 0, };
static int showed_rect_count = 0;
int move_a = 0, move_zig = 0, change_shape = 0;


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void random_color(COLORVALUE color);
void Mouse(int button, int state, int x, int y);
void convert_deviceXY_to_openglXY(int x, int y, float* ox, float* oy);
void convert_openglXY_to_deviceXY(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
void print_rect(int i);




void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(GWIDTH, GHEIGHT); // �������� ũ�� ����
	glutCreateWindow("Exam 1"); // ������ ���� (������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	for (int i = 0; i < 5; i++) {
		rect[i].left = rect[i].top = -0.2f;
		rect[i].right = rect[i].bottom = 0.2f;
		rect[i].prior = -1;
		rect[i].shape_count_X = rand() % 150;
		rect[i].shape_count_Y = rand() % 150;
		rect[i].zig_count = rand() % 20;
	}

	// int������ ��ǥ ��ȯ
	// float���� RECTANGLE ����ü�� ���ԵǾ�����
	for (int i = 0; i < 5; i++) {

	}

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(backg_color.r, backg_color.g, backg_color.b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	rect[0].color = { 1.0f, 0.0f, 0.0f }; // r
	rect[1].color = { 0.0f, 1.0f, 0.0f }; // g
	rect[2].color = { 0.0f, 0.0f, 1.0f }; // b
	rect[3].color = { 1.0f, 1.0f, 1.0f }; // w
	rect[4].color = { 1.0f, 1.0f, 0.0f }; // y

	int wanted_prior = 5 - showed_rect_count;

	for (int i = 0; i < 5; i++) {
		if (rect[i].show)
		print_rect(i);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	srand(time(NULL));

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// ���� ��ġ�� ������ ��

		for (int i = 0; i < 5; i++) { // i = 3
			if (rect[i].show == 0) {

				// ���� ���� ����
				rect[i].origin_X = x;
				rect[i].origin_Y = y;

				convert_deviceXY_to_openglXY(x - 40, y - 40, &rect[i].left, &rect[i].top);
				convert_deviceXY_to_openglXY(x + 40, y + 40, &rect[i].right, &rect[i].bottom);

				convert_openglXY_to_deviceXY(&int_rect[i].left, &int_rect[i].top, rect[i].left, rect[i].top);
				convert_openglXY_to_deviceXY(&int_rect[i].right, &int_rect[i].bottom, rect[i].right, rect[i].bottom);

				rect[i].show = 1;

				showed_rect_count++;
				break;
			}
		}

		left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button = true;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		right_button = false;
	}
}

void convert_deviceXY_to_openglXY(int x, int y, float* ox, float* oy) {
	int w = GWIDTH;
	int h = GHEIGHT;
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void convert_openglXY_to_deviceXY(int* x, int* y, float ox, float oy) {
	float w = GWIDTH;
	float h = GHEIGHT;
	*x = (int)((ox * w + w) / 2);
	*y = GHEIGHT - (int)((oy * h + h) / 2);
}

void move_LT(int i) {
	rect[i].left -= 0.005f;
	rect[i].top += 0.005f;
	rect[i].right -= 0.005f;
	rect[i].bottom += 0.005f;
}

void move_LB(int i) {
	rect[i].left -= 0.005f;
	rect[i].top -= 0.005f;
	rect[i].right -= 0.005f;
	rect[i].bottom -= 0.005f;
}

void move_RB(int i) {
	rect[i].left += 0.005f;
	rect[i].top -= 0.005f;
	rect[i].right += 0.005f;
	rect[i].bottom -= 0.005f;
}

void move_RT(int i) {
	rect[i].left += 0.005f;
	rect[i].top += 0.005f;
	rect[i].right += 0.005f;
	rect[i].bottom += 0.005f;
}

void move_zig_UP(int i) {
	if (rect[i].zig_count <= 10) {
		rect[i].left += 0.01f;
		rect[i].right += 0.01f;
		rect[i].top += 0.003f;
		rect[i].bottom += 0.003f;
		rect[i].zig_count++;
	}
	else if (10 < rect[i].zig_count && rect[i].zig_count <= 20) {
		rect[i].left -= 0.01f;
		rect[i].right -= 0.01f;
		rect[i].top += 0.003f;
		rect[i].bottom += 0.003f;
		rect[i].zig_count++;
	}
	else {
		rect[i].zig_count = 0;
	}
}

void move_zig_DW(int i) {
	if (rect[i].zig_count <= 10) {
		rect[i].left += 0.01f;
		rect[i].right += 0.01f;
		rect[i].top -= 0.003f;
		rect[i].bottom -= 0.003f;
		rect[i].zig_count++;
	}
	else if (10 < rect[i].zig_count && rect[i].zig_count <= 20) {
		rect[i].left -= 0.01f;
		rect[i].right -= 0.01f;
		rect[i].top -= 0.003f;
		rect[i].bottom -= 0.003f;
		rect[i].zig_count++;
	}
	else {
		rect[i].zig_count = 0;
	}
}

void random_size(int i) {
	if (rect[i].shape_count_X <= 300) {
		rect[i].left -= 0.0005f;
		rect[i].right += 0.0005f;
		rect[i].shape_count_X++;
	}
	else if (300 < rect[i].shape_count_X && rect[i].shape_count_X <= 600) {
		rect[i].left += 0.0005f;
		rect[i].right -= 0.0005f;
		rect[i].shape_count_X++;
	}
	else {
		rect[i].shape_count_X = 0;
	}

	if (rect[i].shape_count_Y <= 300) {
		rect[i].top += 0.0005f;
		rect[i].bottom -= 0.0005f;
		rect[i].shape_count_Y++;
	}
	else if (300 < rect[i].shape_count_Y && rect[i].shape_count_Y <= 600) {
		rect[i].top -= 0.0005f;
		rect[i].bottom += 0.0005f;
		rect[i].shape_count_Y++;
	}
	else {
		rect[i].shape_count_Y = 0;
	}
}

void back_to_origin() {
	for (int i = 0; i < 5; i++) {
		if (rect[i].show) {
			convert_deviceXY_to_openglXY(rect[i].origin_X - 40, rect[i].origin_Y - 40, 
				&rect[i].left, &rect[i].top);
			convert_deviceXY_to_openglXY(rect[i].origin_X + 40, rect[i].origin_Y + 40, 
				&rect[i].right, &rect[i].bottom);
		}
	}
}

void TimerFunction(int value) {
	RECT temp;
	RECT wout_left = { -100, 0, 0, GHEIGHT };
	RECT wout_right = { GWIDTH, 0, GWIDTH + 100, GHEIGHT };
	RECT wout_top = { 0, -100, GWIDTH, 0 };
	RECT wout_bottom = { 0, GHEIGHT, GWIDTH, GHEIGHT + 100 };
	RECT t_rect[5] = { 0 };

	for (int i = 0; i < 5; i++) {
		convert_openglXY_to_deviceXY(&int_rect[i].left, &int_rect[i].top, rect[i].left, rect[i].top);
		convert_openglXY_to_deviceXY(&int_rect[i].right, &int_rect[i].bottom, rect[i].right, rect[i].bottom);

		t_rect[i].left = int_rect[i].left;
		t_rect[i].top = int_rect[i].top;
		t_rect[i].right = int_rect[i].right;
		t_rect[i].bottom = int_rect[i].bottom;

		if (rect[i].show && move_a) {
			switch (rect[i].to) {

				// ������ �� ����
			case 0:
				move_RT(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) rect[i].to = 3;
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) rect[i].to = 1;
				break;

				// ������ �Ʒ� ����
			case 1:
				move_RB(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) rect[i].to = 2;
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) rect[i].to = 0;
				break;

				// ���� �Ʒ� ����
			case 2:
				move_LB(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) rect[i].to = 1;
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) rect[i].to = 3;
				break;

				// ���� �� ����
			case 3:
				move_LT(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) rect[i].to = 0;
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) rect[i].to = 2;
				break;
			}
		}
		if (move_zig && rect[i].show) {
			switch (rect[i].zig_to) {
			// �Ʒ���
			case 0:
				move_zig_DW(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) rect[i].zig_to = 1;
				break;
			// ����
			case 1:
				move_zig_UP(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_top)) rect[i].zig_to = 0;
				break;
			}
		}
		if (change_shape && rect[i].show) {
			random_size(i);
			if (move_a)
			switch (rect[i].to) {

				// ������ �� ����
			case 0:
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) rect[i].to = 3;
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) rect[i].to = 1;
				break;

				// ������ �Ʒ� ����
			case 1:
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) rect[i].to = 2;
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) rect[i].to = 0;
				break;

				// ���� �Ʒ� ����
			case 2:
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) rect[i].to = 1;
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) rect[i].to = 3;
				break;

				// ���� �� ����
			case 3:
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) rect[i].to = 0;
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) rect[i].to = 2;
				break;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A': case 'a':
		if (move_a) move_a = 0;
		else move_a = 1;
		break;
	case 'I': case 'i':
		if (move_zig) move_zig = 0;
		else move_zig = 1;
		break;
	case 'C': case 'c':
		if (change_shape) change_shape = 0;
		else change_shape = 1;
		break;
	case 'S': case 's':
		move_a = 0, move_zig = 0, change_shape = 0;
		break;
	case 'M': case 'm':
		back_to_origin();
		break;
	case 'R': case 'r':
		for (int i = 0; i < 5; i++) {
			rect[i].show = 0;
			rect[i].prior = -1;
			rect[i].selected = 0;
			showed_rect_count = 0;
		}
		move_a = 0, move_zig = 0, change_shape = 0;
		break;
	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void print_rect(int i) {
	glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}

void random_color(COLORVALUE color) {
	color.r = (float)rand() / (RAND_MAX);
	color.g = (float)rand() / (RAND_MAX);
	color.b = (float)rand() / (RAND_MAX);
}




#endif