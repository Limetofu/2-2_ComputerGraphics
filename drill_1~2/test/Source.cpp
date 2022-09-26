#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
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
	glutTimerFunc(100, TimerFunction, 1);
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

static bool timer_on = 0;

COLORVALUE color = { 0.0f, };

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(color.r, color.g, color.b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(600, 600); // �������� ũ�� ����
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


	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}



GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(backg_color.r, backg_color.g, backg_color.b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�

	glColor3f(rec_color.r, rec_color.g, rec_color.b);
	glRectf(rec.l, rec.l, rec.r, rec.r);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y) {
	// �簢���� size? -> float��. 0 ~ 1 ���� �Ǽ�.
	// int ������� ���Ϸ���? -> window ������ /2��ŭ ���ؾ� ��.
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