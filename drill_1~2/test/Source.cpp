#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	srand(time(NULL));
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


	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}



GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�

	for (int i = 0; i < 4; ++i) {
		glColor3f(backg[i].color.r, backg[i].color.g, backg[i].color.b);
		glRectf(backg[i].left, backg[i].bottom, backg[i].right, backg[i].top);
	}

	for (int i = 0; i < 4; ++i) {
		glColor3f(rec[i].color.r, rec[i].color.g, rec[i].color.b);
		glRectf(rec[i].left, rec[i].bottom, rec[i].right, rec[i].top);
	}


	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}


GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}


void Mouse(int button, int state, int x, int y) {
	// �簢���� size? -> float��. 0 ~ 1 ���� �Ǽ�.
	// int ������� ���Ϸ���? -> window ������ /2��ŭ ���ؾ� ��.

	float mouse_x = 0.0f;
	float mouse_y = 0.0f;

	con_D_to_Ogl(x, y, &mouse_x, &mouse_y);

	// �����ؾ� �� ��. - ����� ��

	// 1. ���콺 ���� ��ư ������ ��
	//   - �簢�� ��������?           --> Rect inside = true, Backg inside = true
	//   - �簢�� �ܺ�����? (�������) --> Rect inside = false, Backg inside = true

	// 2. ���콺 ������ ��ư ������ ��
	//   - �簢�� ��������?           --> Rect inside = true, Backg inside = true
	//   - �簢�� �ܺ�����? (�������) --> Rect inside = false, Backg inside = true


	int rect_inside_num = -1;
	int backg_inside_num = -1;



	// 1 - ���� ���콺 ��ư (�� �ٲٱ�)
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 4; i++) {
			// ------ �� �簢�� ���� �Ǻ� ------
			if (mouse_x >= rec[i].left && mouse_x <= rec[i].right && mouse_y >= rec[i].bottom && mouse_y <= rec[i].top) {
				rect_inside_num = i;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			// ------ �� ���� ��� �簢�� �Ǻ� ------
			if (mouse_x >= backg[i].left && mouse_x <= backg[i].right && mouse_y >= backg[i].bottom && mouse_y <= backg[i].top) {
				backg_inside_num = i;
				break;
			}
		}


		// �� �簢�� ������ ��
		if (rect_inside_num == backg_inside_num) {
			rec[rect_inside_num].color.r = (float)rand() / (RAND_MAX);
			rec[rect_inside_num].color.g = (float)rand() / (RAND_MAX);
			rec[rect_inside_num].color.b = (float)rand() / (RAND_MAX);
		}
		// �� �簢�� �ܺ��� �� (��游)
		else {
			backg[backg_inside_num].color.r = (float)rand() / (RAND_MAX);
			backg[backg_inside_num].color.g = (float)rand() / (RAND_MAX);
			backg[backg_inside_num].color.b = (float)rand() / (RAND_MAX);
		}
	}

	rect_inside_num = -1;
	backg_inside_num = -1;

	// 2 - ������ ���콺 ��ư (ũ�� �ٲٱ�)
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 4; i++) {
			// ------ �� �簢�� ���� �Ǻ� ------
			if (mouse_x >= rec[i].left && mouse_x <= rec[i].right && mouse_y >= rec[i].bottom && mouse_y <= rec[i].top) {
				rect_inside_num = i;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			// ------ �� ���� ��� �簢�� �Ǻ� ------
			if (mouse_x >= backg[i].left && mouse_x <= backg[i].right && mouse_y >= backg[i].bottom && mouse_y <= backg[i].top) {
				backg_inside_num = i;
				break;
			}
		}

		// �� �簢�� ������ �� - ũ�� ���
		if (rect_inside_num == backg_inside_num) {
			rec[backg_inside_num].left += 0.05f;
			rec[backg_inside_num].bottom += 0.05f;
			rec[backg_inside_num].right -= 0.05f;
			rec[backg_inside_num].top -= 0.05f;
		}
		// �� �簢�� �ܺ��� �� (��游) - ũ�� Ȯ��
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