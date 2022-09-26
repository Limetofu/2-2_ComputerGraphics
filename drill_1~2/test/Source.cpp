#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define NUM 1

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

COLORVALUE backg_color = { 0.0f, 1.0f, 0.0f };
COLORVALUE rec_color = { 1.0f, 0.0f, 0.0f };

typedef struct RECVALUE {
	float l;
	float r;
};

RECVALUE rec = { -0.5f, 0.5f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void random_color(COLORVALUE color);
void Mouse(int button, int state, int x, int y);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(600, 600); // 윈도우의 크기 지정
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
	glClearColor(backg_color.r, backg_color.g, backg_color.b, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	glColor3f(rec_color.r, rec_color.g, rec_color.b);
	glRectf(rec.l, rec.l, rec.r, rec.r);

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y) {
	// 사각형의 size? -> float형. 0 ~ 1 사이 실수.
	// int 사이즈로 구하려면? -> window 사이즈 /2만큼 곱해야 함.
	srand(time(NULL));

	float rect_right = rec.r * 300 + 300;
	
	float rect_left = 300 - (rect_right - 300);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && 
		(x >= rect_left && x <= rect_right && y >= rect_left && y <= rect_right)) {
		rec_color.r = (float)rand() / (RAND_MAX);
		rec_color.g = (float)rand() / (RAND_MAX);
		rec_color.b = (float)rand() / (RAND_MAX);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		backg_color.r = (float)rand() / (RAND_MAX);
		backg_color.g = (float)rand() / (RAND_MAX);
		backg_color.b = (float)rand() / (RAND_MAX);
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN &&
		(x >= rect_left && x <= rect_right && y >= rect_left && y <= rect_right)) {
		if (rec.r <= 0.8f) {
			rec.r += 0.1f;
			rec.l -= 0.1f;
		}
			
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (rec.r >= 0.2f) {
			rec.r -= 0.1f;
			rec.l += 0.1f;
		}
			
	}
}


void random_color(COLORVALUE color) {
	color.r = (float)rand() / (RAND_MAX);
	color.g = (float)rand() / (RAND_MAX);
	color.b = (float)rand() / (RAND_MAX);
}




#endif