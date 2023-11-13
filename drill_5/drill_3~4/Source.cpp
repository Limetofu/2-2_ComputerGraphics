#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

#define NUM 4

#if NUM == 4

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
	COLORVALUE color;
};

typedef struct INT_RECT {
	int left;
	int top;
	int right;
	int bottom;
};

RECTANGLE rect[100] = { 0, };
INT_RECT int_rect[100] = { 0, };
static int showed_rect_count = 0;

RECTANGLE eraser = { 0 };
INT_RECT int_eraser = { 0, };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void convert_deviceXY_to_openglXY(int x, int y, float* ox, float* oy);
void convert_openglXY_to_deviceXY(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);
void Motion(int x, int y);


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(GWIDTH, GHEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Exam 1"); // 윈도우 생성 (윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);


	srand(unsigned int(time(NULL)));

	for (int i = 0; i < 100; i++) {
		int xpos = rand() % GWIDTH;
		int ypos = rand() % GHEIGHT;
		convert_deviceXY_to_openglXY(xpos - 10, ypos - 10, &rect[i].left, &rect[i].top);
		convert_deviceXY_to_openglXY(xpos + 10, ypos + 10, &rect[i].right, &rect[i].bottom);
		convert_openglXY_to_deviceXY(&int_rect[i].left, &int_rect[i].top, rect[i].left, rect[i].top);
		convert_openglXY_to_deviceXY(&int_rect[i].right, &int_rect[i].bottom, rect[i].right, rect[i].bottom);

		float rmax = 32767;
		random_color(&rect[i].color);
		rect[i].show = 1;
	}

	eraser.left = -2.0f, eraser.right = -2.0f, eraser.top = -2.0f, eraser.bottom = -2.0f;
	eraser.color = { 1.0f, 1.0f, 0.0f };

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(backg_color.r, backg_color.g, backg_color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int wanted_prior = 5 - showed_rect_count;

	for (int i = 0; i < 100; i++) {
		if (rect[i].show)
			print_rect(i);
	}

	if (eraser.show) {
		glColor3f(eraser.color.r, eraser.color.g, eraser.color.b);
		glRectf(eraser.left, eraser.top, eraser.right, eraser.bottom);
	}

	glutSwapBuffers(); // 화면에 출력하기
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	srand(time(NULL));

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		beforeX = x, beforeY = y;

		convert_deviceXY_to_openglXY(x - 20, y - 20, &eraser.left, &eraser.top);
		convert_deviceXY_to_openglXY(x + 20, y + 20, &eraser.right, &eraser.bottom);

		convert_openglXY_to_deviceXY(&int_eraser.left, &int_eraser.top, eraser.left, eraser.top);
		convert_openglXY_to_deviceXY(&int_eraser.right, &int_eraser.bottom, eraser.right, eraser.bottom);

		eraser.show = 1;
	
		left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		eraser.show = 0;
		left_button = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button = true;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		right_button = false;
	}
	// glutSwapBuffers();
}

void Motion(int x, int y) {

	// 왼쪽 마우스가 눌렸을 때
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		int_eraser.left += afterX;
		int_eraser.top += afterY;
		int_eraser.right += afterX;
		int_eraser.bottom += afterY;

		beforeX = x, beforeY = y;
		convert_deviceXY_to_openglXY(int_eraser.left, int_eraser.top, &eraser.left, &eraser.top);
		convert_deviceXY_to_openglXY(int_eraser.right, int_eraser.bottom, &eraser.right, &eraser.bottom);

		RECT temp;
		RECT t_rect[100];
		RECT t_eraser = { int_eraser.left, int_eraser.top, int_eraser.right, int_eraser.bottom };
		for (int i = 0; i < 100; i++) {
			t_rect[i].left = int_rect[i].left;
			t_rect[i].top = int_rect[i].top;
			t_rect[i].right = int_rect[i].right;
			t_rect[i].bottom = int_rect[i].bottom;
			if (IntersectRect(&temp, &t_eraser, &t_rect[i])) {
				rect[i].show = 0;
			}
		}
	}

	// 오른쪽 마우스가 눌렸을 때
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
	*y = GHEIGHT - (int)((oy * h + h) / 2);
}


GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r': case 'R':
		for (int i = 0; i < 100; i++) {
			int xpos = rand() % GWIDTH;
			int ypos = rand() % GHEIGHT;
			convert_deviceXY_to_openglXY(xpos - 10, ypos - 10, &rect[i].left, &rect[i].top);
			convert_deviceXY_to_openglXY(xpos + 10, ypos + 10, &rect[i].right, &rect[i].bottom);
			convert_openglXY_to_deviceXY(&int_rect[i].left, &int_rect[i].top, rect[i].left, rect[i].top);
			convert_openglXY_to_deviceXY(&int_rect[i].right, &int_rect[i].bottom, rect[i].right, rect[i].bottom);

			float rmax = 32767;
			random_color(&rect[i].color);
			rect[i].show = 1;
		}

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

void random_color(COLORVALUE* color) {
	color->r = (float)rand() / (RAND_MAX);
	color->g = (float)rand() / (RAND_MAX);
	color->b = (float)rand() / (RAND_MAX);
}




#endif