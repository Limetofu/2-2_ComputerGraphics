#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#define GWIDTH 800
#define GHEIGHT 600

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")


#define NUM 2


void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);


#if NUM == 1

GLvoid drawScene(GLvoid);
GLvoid Reshape (int w, int h);
GLvoid Keyboard (unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
void random_color();

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
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
	glutTimerFunc(100, TimerFunction, 1);
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

static bool timer_on = 0;

COLORVALUE color = { 0.0f, };

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(color.r, color.g, color.b, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();
	if (timer_on) {
		random_color();
	}

	glutTimerFunc(100, TimerFunction, 1);
}

void random_color() {
	srand(time(NULL));
	color.r = (float)rand() / (RAND_MAX);
	color.g = (float)rand() / (RAND_MAX);
	color.b = (float)rand() / (RAND_MAX);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'R': case 'r':
		color.r = 1.0f, color.g = 0.0f, color.b = 0.0f;
		break;
	case 'G': case 'g':
		color.r = 0.0f, color.g = 1.0f, color.b = 0.0f;
		break;
	case 'B': case 'b':
		color.r = 0.0f, color.g = 0.0f, color.b = 1.0f;
		break;
	case 'A': case 'a':
		random_color();
		break;
	case 'K': case 'k':
		color.r = 0.0f, color.g = 0.0f, color.b = 0.0f;
		break;
	case 'W': case 'w':
		color.r = 1.0f, color.g = 1.0f, color.b = 1.0f;
		break;
	case 'T': case 't':
		timer_on = 1;
		break;
	case 'S': case 's':
		timer_on = 0;
		break;
	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}


#elif NUM == 2

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

typedef struct RECVALUE {
	float left;
	float bottom;
	float right;
	float top;
	COLORVALUE color;
};

RECVALUE rec[4] = {
	{-1.0f,  0.0f, 0.0f, 1.0f, {1.0f, 0.0f, 0.0f}},
	{ 0.0f,  0.0f, 1.0f, 1.0f, {0.0f, 1.0f, 0.0f}},
	{-1.0f, -1.0f, 0.0f, 0.0f, {0.0f, 0.0f, 1.0f}},
	{ 0.0f, -1.0f, 1.0f, 0.0f, {0.8f, 0.3f, 0.5f}}
};

RECVALUE backg[4] = {
	{-1.0f,  0.0f, 0.0f, 1.0f, {0.0f, 0.0f, 0.0f}},
	{ 0.0f,  0.0f, 1.0f, 1.0f, {0.0f, 0.0f, 0.0f}},
	{-1.0f, -1.0f, 0.0f, 0.0f, {0.0f, 0.0f, 0.0f}},
	{ 0.0f, -1.0f, 1.0f, 0.0f, {0.0f, 0.0f, 0.0f}}
};



GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void random_color(COLORVALUE color);
void Mouse(int button, int state, int x, int y);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	srand(time(NULL));
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


	glutMainLoop(); // 이벤트 처리 시작 
}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	for (int i = 0; i < 4; ++i) {
		glColor3f(backg[i].color.r, backg[i].color.g, backg[i].color.b);
		glRectf(backg[i].left, backg[i].bottom, backg[i].right, backg[i].top);
	}

	for (int i = 0; i < 4; ++i) {
		glColor3f(rec[i].color.r, rec[i].color.g, rec[i].color.b);
		glRectf(rec[i].left, rec[i].bottom, rec[i].right, rec[i].top);
	}


	glutSwapBuffers(); // 화면에 출력하기
}


GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}


void Mouse(int button, int state, int x, int y) {
	// 사각형의 size? -> float형. 0 ~ 1 사이 실수.
	// int 사이즈로 구하려면? -> window 사이즈 /2만큼 곱해야 함.

	float mouse_x = 0.0f;
	float mouse_y = 0.0f;

	con_D_to_Ogl(x, y, &mouse_x, &mouse_y);

	// 생각해야 할 것. - 경우의 수

	// 1. 마우스 왼쪽 버튼 눌렸을 때
	//   - 사각형 내부인지?           --> Rect inside = true, Backg inside = true
	//   - 사각형 외부인지? (배경인지) --> Rect inside = false, Backg inside = true

	// 2. 마우스 오른쪽 버튼 눌렸을 때
	//   - 사각형 내부인지?           --> Rect inside = true, Backg inside = true
	//   - 사각형 외부인지? (배경인지) --> Rect inside = false, Backg inside = true


	int rect_inside_num = -1;
	int backg_inside_num = -1;



	// 1 - 왼쪽 마우스 버튼 (색 바꾸기)
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 4; i++) {
			// ------ 색 사각형 먼저 판별 ------
			if (mouse_x >= rec[i].left && mouse_x <= rec[i].right && mouse_y >= rec[i].bottom && mouse_y <= rec[i].top) {
				rect_inside_num = i;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			// ------ 그 다음 배경 사각형 판별 ------
			if (mouse_x >= backg[i].left && mouse_x <= backg[i].right && mouse_y >= backg[i].bottom && mouse_y <= backg[i].top) {
				backg_inside_num = i;
				break;
			}
		}


		// 색 사각형 내부일 때
		if (rect_inside_num == backg_inside_num) {
			rec[rect_inside_num].color.r = (float)rand() / (RAND_MAX);
			rec[rect_inside_num].color.g = (float)rand() / (RAND_MAX);
			rec[rect_inside_num].color.b = (float)rand() / (RAND_MAX);
		}
		// 색 사각형 외부일 때 (배경만)
		else {
			backg[backg_inside_num].color.r = (float)rand() / (RAND_MAX);
			backg[backg_inside_num].color.g = (float)rand() / (RAND_MAX);
			backg[backg_inside_num].color.b = (float)rand() / (RAND_MAX);
		}
	}

	rect_inside_num = -1;
	backg_inside_num = -1;

	// 2 - 오른쪽 마우스 버튼 (크기 바꾸기)
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 4; i++) {
			// ------ 색 사각형 먼저 판별 ------
			if (mouse_x >= rec[i].left && mouse_x <= rec[i].right && mouse_y >= rec[i].bottom && mouse_y <= rec[i].top) {
				rect_inside_num = i;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			// ------ 그 다음 배경 사각형 판별 ------
			if (mouse_x >= backg[i].left && mouse_x <= backg[i].right && mouse_y >= backg[i].bottom && mouse_y <= backg[i].top) {
				backg_inside_num = i;
				break;
			}
		}

		// 색 사각형 내부일 때 - 크기 축소
		if (rect_inside_num == backg_inside_num) {
			rec[backg_inside_num].left += 0.05f;
			rec[backg_inside_num].bottom += 0.05f;
			rec[backg_inside_num].right -= 0.05f;
			rec[backg_inside_num].top -= 0.05f;
		}
		// 색 사각형 외부일 때 (배경만) - 크기 확대
		else {
			rec[backg_inside_num].left -= 0.05f;
			rec[backg_inside_num].bottom -= 0.05f;
			rec[backg_inside_num].right += 0.05f;
			rec[backg_inside_num].top += 0.05f;
		}
	}
}


void random_color(COLORVALUE color) {
	color.r = (float)rand() / (RAND_MAX);
	color.g = (float)rand() / (RAND_MAX);
	color.b = (float)rand() / (RAND_MAX);
}

void con_D_to_Ogl(int x, int y, float* ox, float* oy) {
	int w = GWIDTH;
	int h = GHEIGHT;
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void con_Ogl_to_D(int* x, int* y, float ox, float oy) {
	float w = GWIDTH;
	float h = GHEIGHT;
	*x = (int)((ox * w + w) / 2);
	*y = GHEIGHT - (int)((oy * h + h) / 2);
}



#endif