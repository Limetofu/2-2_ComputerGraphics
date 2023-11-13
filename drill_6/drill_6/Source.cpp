#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define GWIDTH (GLint)800
#define GHEIGHT (GLint)800

#define DRILL 12

#if DRILL == 6

#define TRI_COUNT 4

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.2f, 0.2f, 0.2f };

static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);
void Motion(int x, int y);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

void change_tri(float ox, float oy);

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[8];

bool draw_line = 0;
bool draw_plane = 1;

int delete_count = 0;

bool now_bigger = 1;
float grow_size = 0.0f;

GLfloat pos[TRI_COUNT * 3][3] = {
	{ -0.7, 0.4, 0.0 }, { -0.3, 0.4, 0.0 }, { -0.5, 0.9, 0.0 },
	{0.4, 0.3, 0.0}, {0.8, 0.3, 0.0}, {0.6, 0.8, 0.0}, 
	{-0.4, -0.8, 0.0}, {-0.8, -0.8, 0.0}, {-0.6, -0.3, 0.0},
	{0.3, -0.7, 0.0}, {0.7, -0.7, 0.0}, {0.5, -0.2, 0.0}
};

GLfloat col[TRI_COUNT * 3][3] = {
	{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
	{0.5, 0.5, 1.0}, {0.5, 0.5, 1.0}, {0.5, 0.5, 1.0}
};




void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(GWIDTH, GHEIGHT);
	glutCreateWindow("Exam 1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0f;
	bColor = 1.0f;
	
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	
	glBindVertexArray(vao);

	glPointSize(5.0);

	if (draw_plane)
		glDrawArrays(GL_TRIANGLES, 0, 12);
	else {
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 3, 3);
		glDrawArrays(GL_LINE_LOOP, 6, 3);
		glDrawArrays(GL_LINE_LOOP, 9, 3);
	}
	


	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void InitBuffer()
{  
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(8, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
		//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
		//--- triShape �迭�� ������: 9 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glEnableVertexAttribArray(0);
		//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3 * float
	glEnableVertexAttribArray(1);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	*/
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	float ox = 0, oy = 0;
	con_D_to_Ogl(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;

		change_tri(ox, oy);

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
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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


GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A': case 'a': // ������
		draw_line = 0;
		draw_plane = 1;
		break;
	case 'B': case 'b': // ������
		draw_line = 1;
		draw_plane = 0;
		break;
	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void change_tri(float ox, float oy) {
	switch (delete_count) {
	case 0:
		pos[0][0] = ox - 0.2f - grow_size;
		pos[0][1] = oy - 0.2f - grow_size;
		pos[1][0] = ox + 0.2f + grow_size;
		pos[1][1] = oy - 0.2f - grow_size;
		pos[2][0] = ox;
		pos[2][1] = oy + 0.3f + grow_size;
		delete_count++;
		break;
	case 1:
		pos[3][0] = ox - 0.2f - grow_size;
		pos[3][1] = oy - 0.2f - grow_size;
		pos[4][0] = ox + 0.2f + grow_size;
		pos[4][1] = oy - 0.2f - grow_size;
		pos[5][0] = ox;
		pos[5][1] = oy + 0.3f + grow_size;
		delete_count++;
		break;
	case 2:
		pos[6][0] = ox - 0.2f - grow_size;
		pos[6][1] = oy - 0.2f - grow_size;
		pos[7][0] = ox + 0.2f + grow_size;
		pos[7][1] = oy - 0.2f - grow_size;
		pos[8][0] = ox;
		pos[8][1] = oy + 0.3f + grow_size;
		delete_count++;
		break;
	case 3:
		pos[9][0] = ox - 0.2f - grow_size;
		pos[9][1] = oy - 0.2f - grow_size;
		pos[10][0] = ox + 0.2f + grow_size;
		pos[10][1] = oy - 0.2f - grow_size;
		pos[11][0] = ox;
		pos[11][1] = oy + 0.3f + grow_size;
		delete_count = 0;
		break;
	}

	if (now_bigger == 1) {
		if (grow_size < 0.16f)
			grow_size += 0.04f;
		else {
			now_bigger = 0;
			grow_size -= 0.04f;
		}
	}
	else {
		if (grow_size > -0.16f)
			grow_size -= 0.04f;
		else {
			now_bigger = 1;
			grow_size += 0.04f;
		}
	}



	InitBuffer();
}


void print_rect(int i) {
	//glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	//glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}

#elif DRILL == 7

#define TRI_COUNT 4

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.2f, 0.2f, 0.2f };

static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);
void Motion(int x, int y);
void TimerFunction(int value);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

void change_tri(float ox, float oy);

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[8];

bool draw_line = 0;
bool draw_plane = 1;
int anime_flag = false;

int delete_count = 0;

float grow_size = 0.0f;

GLfloat pos[TRI_COUNT * 3][3] = {
	{ -0.7, 0.4, 0.0 }, { -0.4, 0.4, 0.0 }, { -0.55, 0.8, 0.0 },
	{0.5, 0.3, 0.0}, {0.8, 0.3, 0.0}, {0.65, 0.7, 0.0}, 
	{-0.8, -0.8, 0.0}, {-0.5, -0.8, 0.0}, {-0.65, -0.4, 0.0},
	{0.4, -0.7, 0.0}, {0.7, -0.7, 0.0}, {0.55, -0.3, 0.0}
};

GLfloat col[TRI_COUNT * 3][3] = {
	{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
	{0.5, 0.5, 1.0}, {0.5, 0.5, 1.0}, {0.5, 0.5, 1.0}
};

typedef struct INT_RECT {
	int left;
	int top;
	int right;
	int bottom;
};

typedef struct TRIANGLE {
	INT_RECT int_pos;
	int show;
	int to;
	int shape;
};

TRIANGLE tri[4] = {0, };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(GWIDTH, GHEIGHT);
	glutCreateWindow("Exam 1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);



	con_Ogl_to_D(&tri[0].int_pos.left, &tri[0].int_pos.top, pos[0][0], pos[2][1]);
	con_Ogl_to_D(&tri[0].int_pos.right, &tri[0].int_pos.bottom, pos[1][0], pos[0][1]);

	con_Ogl_to_D(&tri[1].int_pos.left, &tri[1].int_pos.top, pos[3][0], pos[5][1]);
	con_Ogl_to_D(&tri[1].int_pos.right, &tri[1].int_pos.bottom, pos[4][0], pos[3][1]);

	con_Ogl_to_D(&tri[2].int_pos.left, &tri[2].int_pos.top, pos[6][0], pos[8][1]);
	con_Ogl_to_D(&tri[2].int_pos.right, &tri[2].int_pos.bottom, pos[7][0], pos[6][1]);

	con_Ogl_to_D(&tri[3].int_pos.left, &tri[3].int_pos.top, pos[9][0], pos[11][1]);
	con_Ogl_to_D(&tri[3].int_pos.right, &tri[3].int_pos.bottom, pos[10][0], pos[9][1]);

	for (int i = 0; i < 4; i++) {
		tri[i].shape = 0;
		tri[i].show = 0;
		tri[i].to = 1;
	}

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0f;
	bColor = 1.0f;
	
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	
	glBindVertexArray(vao);

	glPointSize(5.0);

	if (draw_plane) {
		for (int i = 0; i < 4; i++) {
			if (tri[i].show)
				glDrawArrays(GL_TRIANGLES, i * 3, 3);
		}
	}
	else {
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 3, 3);
		glDrawArrays(GL_LINE_LOOP, 6, 3);
		glDrawArrays(GL_LINE_LOOP, 9, 3);
	}
	


	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void InitBuffer()
{  
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(8, vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	float ox = 0, oy = 0;
	con_D_to_Ogl(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;

		change_tri(ox, oy);

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
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

void move_LT(int i) {
	pos[i * 3][0] -= (0.003f + (0.002f * i));
	pos[i * 3][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] += (0.003f + (0.002f * i));
}

void move_LB(int i) {
	pos[i * 3][0] -= (0.003f + (0.002f * i));
	pos[i * 3][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] -= (0.003f + (0.002f * i));
}

void move_RB(int i) {
	pos[i * 3][0] += (0.003f + (0.002f * i));
	pos[i * 3][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] -= (0.003f + (0.002f * i));
}

void move_RT(int i) {
	pos[i * 3][0] += (0.003f + (0.002f * i));
	pos[i * 3][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] += (0.003f + (0.002f * i));
}

// 3��° ���Ҹ�
// ���� ���������� ����.

void change_up(int i) {
	GLfloat ytemp = pos[i * 3][1];
	switch (tri[i].shape) {

	case 1:
		//pos[i * 3][0]
		pos[i * 3][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 1][0]
		pos[i * 3 + 1][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 2][0]
		pos[i * 3 + 2][1] = ytemp;
		break;

	case 2:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3 + 1][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.4f;
		break;
	case 3:
		pos[i * 3 + 1][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3][1];
		pos[i * 3][0] = pos[i * 3][0] - 0.15f;
		//pos[i * 3][1] = pos[i * 3][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.4f;
		break;
	}

	tri[i].shape = 0;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3 + 2][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3][1]);
}

void change_down(int i) {
	GLfloat ytemp = pos[i * 3][1];
	switch (tri[i].shape) {
	case 0:
		//pos[i * 3][0]
		pos[i * 3][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 1][0]
		pos[i * 3 + 1][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 2][0]
		pos[i * 3 + 2][1] = ytemp;

		break;
	case 2:
		pos[i * 3 + 1][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3][1];
		pos[i * 3][0] = pos[i * 3][0] - 0.15f;
		//pos[i * 3][1] = pos[i * 3][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.4f;
		break;
	case 3:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3 + 1][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.4f;
		break;
	}

	tri[i].shape = 1;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3 + 2][1]);
}

void change_left(int i) {
	
	switch (tri[i].shape) {
	case 0:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1] + 0.3f;
		pos[i * 3 + 1][0] = pos[i * 3][0];
		//pos[i * 3 + 1][1] = pos[i * 3 + 1][1]
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.15f;
		break;
	case 1:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		//pos[i * 3][1] = pos[i * 3]
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.15f;
		break;
	case 3:
		pos[i * 3][0] = pos[i * 3 + 2][0];
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		//pos[i * 3 + 2][1]
		break;
	}
	
	tri[i].shape = 2;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 2][0], pos[i * 3 + 1][0]);
}

void change_right(int i) {
	
	switch (tri[i].shape) {
	case 0:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		//pos[i * 3][1] = pos[i * 3]
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] + 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	case 1:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 1][0] = pos[i * 3][0];
		//pos[i * 3 + 1][1] = pos[i * 3][1] + 0.4f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	case 2:
		pos[i * 3][0] = pos[i * 3 + 2][0];
		pos[i * 3][1] = pos[i * 3 + 2][1];
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] + 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	}
	


	tri[i].shape = 3;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3][0], pos[i * 3][1]);
}

void TimerFunction(int value) {
	RECT temp;
	RECT wout_left = { -100, 0, 0, GHEIGHT };
	RECT wout_right = { GWIDTH, 0, GWIDTH + 100, GHEIGHT };
	RECT wout_top = { 0, -100, GWIDTH, 0 };
	RECT wout_bottom = { 0, GHEIGHT, GWIDTH, GHEIGHT + 100 };
	RECT t_rect[4] = { 0, };

	if (anime_flag == 1) {
		for (int i = 0; i < 4; i++) {
			switch (tri[i].shape) {
			case 0:
				con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3 + 2][1]);
				con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3][1]);
				break;
			case 1:
				con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
				con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3 + 2][1]);
				break;
			case 2:
				con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
				con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
				break;
			case 3:
				con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
				con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3][0], pos[i * 3][1]);
				break;
			}

			t_rect[i].left = tri[i].int_pos.left;
			t_rect[i].top = tri[i].int_pos.top;
			t_rect[i].right = tri[i].int_pos.right;
			t_rect[i].bottom = tri[i].int_pos.bottom;

			if (tri[i].show) {
				switch (tri[i].to) {
				case 0:
					move_RT(i);
					if (IntersectRect(&temp, &t_rect[i], &wout_right)) {
						change_right(i);
						tri[i].to = 3;
					}
					else if (IntersectRect(&temp, &t_rect[i], &wout_top)) {
						change_down(i);
						tri[i].to = 1;
					}
					break;
				case 1:
					move_RB(i);
					if (IntersectRect(&temp, &t_rect[i], &wout_right)) {
						change_right(i);
						tri[i].to = 2;
					}
					else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) {
						change_up(i);
						tri[i].to = 0;
					}
					break;
				case 2:
					move_LB(i);
					if (IntersectRect(&temp, &t_rect[i], &wout_left)) {
						change_left(i);
						tri[i].to = 1;
					}
					else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) {
						change_up(i);
						tri[i].to = 3;
					}
					break;
				case 3:
					move_LT(i);
					if (IntersectRect(&temp, &t_rect[i], &wout_left)) {
						change_left(i);
						tri[i].to = 0;
					}
					else if (IntersectRect(&temp, &t_rect[i], &wout_top)) {
						change_down(i);
						tri[i].to = 2;
					}
					break;
				}



			}
		}
	}
	


	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		if (anime_flag == 1) {
			anime_flag = 0;
		}
		else {
			anime_flag = 1;
		}
		
		break;	

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void change_tri(float ox, float oy) {
	switch (delete_count) {

	case 0:
		pos[0][0] = ox - 0.15f - grow_size;
		pos[0][1] = oy - 0.2f - grow_size;
		pos[1][0] = ox + 0.15f + grow_size;
		pos[1][1] = oy - 0.2f - grow_size;
		pos[2][0] = ox;
		pos[2][1] = oy + 0.2f + grow_size;
		delete_count++;
		tri[0].show = 1;
		tri[0].shape = 0;
		tri[0].to = 1;
		break;

	case 1:
		pos[3][0] = ox - 0.15f - grow_size;
		pos[3][1] = oy - 0.2f - grow_size;
		pos[4][0] = ox + 0.15f + grow_size;
		pos[4][1] = oy - 0.2f - grow_size;
		pos[5][0] = ox;
		pos[5][1] = oy + 0.2f + grow_size;
		delete_count++;
		tri[1].show = 1;
		tri[1].shape = 0;
		tri[1].to = 1;
		break;

	case 2:
		pos[6][0] = ox - 0.15f - grow_size;
		pos[6][1] = oy - 0.2f - grow_size;
		pos[7][0] = ox + 0.15f + grow_size;
		pos[7][1] = oy - 0.2f - grow_size;
		pos[8][0] = ox;
		pos[8][1] = oy + 0.2f + grow_size;
		delete_count++;
		tri[2].show = 1;
		tri[2].shape = 0;
		tri[2].to = 1;
		break;

	case 3:
		pos[9][0] = ox - 0.15f - grow_size;
		pos[9][1] = oy - 0.2f - grow_size;
		pos[10][0] = ox + 0.15f + grow_size;
		pos[10][1] = oy - 0.2f - grow_size;
		pos[11][0] = ox;
		pos[11][1] = oy + 0.2f + grow_size;
		delete_count = 0;
		tri[3].show = 1;
		tri[3].shape = 0;
		tri[3].to = 1;
		break;
	}
	InitBuffer();
}


void print_rect(int i) {
	//glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	//glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}


#elif DRILL == 8

#define TRI_COUNT 5

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.2f, 0.2f, 0.2f };

static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);
void Motion(int x, int y);
void TimerFunction(int value);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

void change_tri(float ox, float oy);

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[8];

bool draw_line = 0;
bool draw_plane = 1;

int delete_count = 0;
bool middle_heading = 1;

float grow_size = 0.0f;

GLfloat pos[TRI_COUNT * 3][3] = {
	// out triangle
	{ -0.7, 0.4, 0.0 }, { -0.4, 0.4, 0.0 }, { -0.55, 0.8, 0.0 },

	// middle triangle
	{-0.4, 0.4, 0.0}, {-0.2, 0.4, 0.0}, {-0.3, 0.1, 0.0},
	{0.2, -0.4, 0.0}, {0.4, -0.4, 0.0}, {0.3, -0.1, 0.0},

	// middle rect
	{-0.4, 0.4, 0.0}, {0.4, 0.4, 0.0}, {0.4, -0.4, 0.0},
	{-0.4, -0.4, 0.0} , {-0.4, 0.4, 0}, {0, 0, 0}
};

GLfloat col[TRI_COUNT * 3][3] = {
	// out triangle
	{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
	
	// middle triangle
	{1.0, 0.8, 0.0}, {1.0, 0.8, 0.0}, {1.0, 0.8, 0.0},
	{1.0, 0.8, 0.0}, {1.0, 0.8, 0.0}, {1.0, 0.8, 0.0},

	// middle rect
	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}
};

typedef struct INT_RECT {
	int left;
	int top;
	int right;
	int bottom;
};

typedef struct TRIANGLE {
	INT_RECT int_pos;
	int show;
	int to;
	int shape;
};

TRIANGLE tri[4] = { 0, };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(GWIDTH, GHEIGHT);
	glutCreateWindow("Exam 1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	con_Ogl_to_D(&tri[0].int_pos.left, &tri[0].int_pos.top, pos[0][0], pos[2][1]);
	con_Ogl_to_D(&tri[0].int_pos.right, &tri[0].int_pos.bottom, pos[1][0], pos[0][1]);

	con_Ogl_to_D(&tri[1].int_pos.left, &tri[1].int_pos.top, pos[3][0], pos[5][1]);
	con_Ogl_to_D(&tri[1].int_pos.right, &tri[1].int_pos.bottom, pos[4][0], pos[3][1]);

	con_Ogl_to_D(&tri[2].int_pos.left, &tri[2].int_pos.top, pos[6][0], pos[8][1]);
	con_Ogl_to_D(&tri[2].int_pos.right, &tri[2].int_pos.bottom, pos[7][0], pos[6][1]);

	con_Ogl_to_D(&tri[3].int_pos.left, &tri[3].int_pos.top, pos[9][0], pos[11][1]);
	con_Ogl_to_D(&tri[3].int_pos.right, &tri[3].int_pos.bottom, pos[10][0], pos[9][1]);

	tri[0].show = 1;
	tri[0].shape = 0;
	tri[0].to = 1;

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0f;
	bColor = 1.0f;

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(5.0);


	glDrawArrays(GL_LINES, 9, 2);
	glDrawArrays(GL_LINES, 10, 2);
	glDrawArrays(GL_LINES, 11, 2);
	glDrawArrays(GL_LINES, 12, 3);

	glDrawArrays(GL_TRIANGLES, 0, 9);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(8, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	float ox = 0, oy = 0;
	con_D_to_Ogl(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;

		// change_tri(ox, oy);

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
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

void move_LT(int i) {
	pos[i * 3][0] -= (0.003f + (0.002f * i));
	pos[i * 3][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] += (0.003f + (0.002f * i));
}

void move_LB(int i) {
	pos[i * 3][0] -= (0.003f + (0.002f * i));
	pos[i * 3][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] -= (0.003f + (0.002f * i));
}

void move_RB(int i) {
	pos[i * 3][0] += (0.003f + (0.002f * i));
	pos[i * 3][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] -= (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] -= (0.003f + (0.002f * i));
}

void move_RT(int i) {
	pos[i * 3][0] += (0.003f + (0.002f * i));
	pos[i * 3][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 1][1] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][0] += (0.003f + (0.002f * i));
	pos[i * 3 + 2][1] += (0.003f + (0.002f * i));
}

// 3��° ���Ҹ�
// ���� ���������� ����.

void change_up(int i) {
	GLfloat ytemp = pos[i * 3][1];
	switch (tri[i].shape) {

	case 1:
		//pos[i * 3][0]
		pos[i * 3][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 1][0]
		pos[i * 3 + 1][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 2][0]
		pos[i * 3 + 2][1] = ytemp;
		break;

	case 2:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3 + 1][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.4f;
		break;
	case 3:
		pos[i * 3 + 1][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3][1];
		pos[i * 3][0] = pos[i * 3][0] - 0.15f;
		//pos[i * 3][1] = pos[i * 3][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.4f;
		break;
	}

	tri[i].shape = 0;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3 + 2][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3][1]);
}

void change_down(int i) {
	GLfloat ytemp = pos[i * 3][1];
	switch (tri[i].shape) {
	case 0:
		//pos[i * 3][0]
		pos[i * 3][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 1][0]
		pos[i * 3 + 1][1] = pos[i * 3 + 2][1];
		//pos[i * 3 + 2][0]
		pos[i * 3 + 2][1] = ytemp;

		break;
	case 2:
		pos[i * 3 + 1][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3][1];
		pos[i * 3][0] = pos[i * 3][0] - 0.15f;
		//pos[i * 3][1] = pos[i * 3][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.4f;
		break;
	case 3:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3 + 1][0] + 0.15f;
		pos[i * 3 + 1][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.4f;
		break;
	}

	tri[i].shape = 1;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3 + 2][1]);
}

void change_left(int i) {

	switch (tri[i].shape) {
	case 0:
		pos[i * 3][0] = pos[i * 3 + 1][0] - 0.15f;
		pos[i * 3][1] = pos[i * 3 + 1][1] + 0.3f;
		pos[i * 3 + 1][0] = pos[i * 3][0];
		//pos[i * 3 + 1][1] = pos[i * 3 + 1][1]
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.15f;
		break;
	case 1:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		//pos[i * 3][1] = pos[i * 3]
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] - 0.15f;
		break;
	case 3:
		pos[i * 3][0] = pos[i * 3 + 2][0];
		pos[i * 3][1] = pos[i * 3 + 1][1];
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] + 0.4f;
		//pos[i * 3 + 2][1]
		break;
	}

	tri[i].shape = 2;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 2][0], pos[i * 3 + 1][0]);
}

void change_right(int i) {

	switch (tri[i].shape) {
	case 0:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		//pos[i * 3][1] = pos[i * 3]
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] + 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	case 1:
		pos[i * 3][0] = pos[i * 3][0] + 0.15f;
		pos[i * 3][1] = pos[i * 3][1] - 0.3f;
		pos[i * 3 + 1][0] = pos[i * 3][0];
		//pos[i * 3 + 1][1] = pos[i * 3][1] + 0.4f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	case 2:
		pos[i * 3][0] = pos[i * 3 + 2][0];
		pos[i * 3][1] = pos[i * 3 + 2][1];
		pos[i * 3 + 1][0] = pos[i * 3][0];
		pos[i * 3 + 1][1] = pos[i * 3][1] + 0.3f;
		pos[i * 3 + 2][0] = pos[i * 3][0] - 0.4f;
		pos[i * 3 + 2][1] = pos[i * 3][1] + 0.15f;
		break;
	}

	tri[i].shape = 3;

	con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
	con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3][0], pos[i * 3][1]);
}

void TimerFunction(int value) {
	RECT temp;
	RECT wout_left = { -100, 0, 0, GHEIGHT };
	RECT wout_right = { GWIDTH, 0, GWIDTH + 100, GHEIGHT };
	RECT wout_top = { 0, -100, GWIDTH, 0 };
	RECT wout_bottom = { 0, GHEIGHT, GWIDTH, GHEIGHT + 100 };

	RECT mid_left = { 240, 260, 260, 540 };
	RECT mid_right = { 540, 260, 560, 540 };
	RECT mid_top = { 260, 240, 540, 260 };
	RECT mid_bottom = { 260, 540, 540, 560 };

	int tempx = 0, tempy = 0;

	con_Ogl_to_D(&tempx, &tempy, -0.4f, -0.4f);

	RECT t_rect[4] = { 0, };

	if (!tri[1].show || !tri[2].show)
	if (middle_heading == 1) {
		// ������
		if (pos[4][0] >= 0.4f) {
			middle_heading = 0;
			GLfloat tcol[3] = { (float)rand() / (RAND_MAX), };
			for (int i = 3; i < 9; i++) {
				for (int k = 0; k < 3; k++)
					col[i][k] = tcol[k];
			}
		}
		else {
			for (int i = 3; i < 9; i++) {
				if (i <= 5) pos[i][0] += 0.003f;
				else pos[i][0] -= 0.003f;
			}
		}
	}
	else {
		if (pos[3][0] <= -0.4f) {
			middle_heading = 1;
			GLfloat tcol[3] = { (float)rand() / (RAND_MAX), };
			for (int i = 3; i < 9; i++) {
				for (int k = 0; k < 3; k++)
					col[i][k] = tcol[k];
			}
		}
		else {
			for (int i = 3; i < 9; i++) {
				if (i <= 5) pos[i][0] -= 0.003f;
				else pos[i][0] += 0.003f;
			}
		}
	}

	for (int i = 0; i < 1; i++) {
		switch (tri[i].shape) {
		case 0:
			con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3 + 2][1]);
			con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3][1]);
			break;
		case 1:
			con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
			con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 1][0], pos[i * 3 + 2][1]);
			break;
		case 2:
			con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3][0], pos[i * 3][1]);
			con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
			break;
		case 3:
			con_Ogl_to_D(&tri[i].int_pos.left, &tri[i].int_pos.top, pos[i * 3 + 2][0], pos[i * 3 + 1][1]);
			con_Ogl_to_D(&tri[i].int_pos.right, &tri[i].int_pos.bottom, pos[i * 3][0], pos[i * 3][1]);
			break;
		}

		t_rect[i].left = tri[i].int_pos.left;
		t_rect[i].top = tri[i].int_pos.top;
		t_rect[i].right = tri[i].int_pos.right;
		t_rect[i].bottom = tri[i].int_pos.bottom;

		if (tri[i].show) {
			switch (tri[i].to) {
			case 0:
				move_RT(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) {
					change_right(i);
					tri[i].to = 3;
				}
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) {
					change_down(i);
					tri[i].to = 1;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_left)) {
					change_right(i);
					tri[i].to = 3;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_bottom)) {
					change_down(i);
					tri[i].to = 1;
				}
				break;
			case 1:
				move_RB(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_right)) {
					change_right(i);
					tri[i].to = 2;
				}
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) {
					change_up(i);
					tri[i].to = 0;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_left)) {
					change_right(i);
					tri[i].to = 2;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_top)) {
					change_up(i);
					tri[i].to = 0;
				}
				break;
			case 2:
				move_LB(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) {
					change_left(i);
					tri[i].to = 1;
				}
				else if (IntersectRect(&temp, &t_rect[i], &wout_bottom)) {
					change_up(i);
					tri[i].to = 3;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_right)) {
					change_left(i);
					tri[i].to = 1;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_top)) {
					change_up(i);
					tri[i].to = 3;
				}
				break;
			case 3:
				move_LT(i);
				if (IntersectRect(&temp, &t_rect[i], &wout_left)) {
					change_left(i);
					tri[i].to = 0;
				}
				else if (IntersectRect(&temp, &t_rect[i], &wout_top)) {
					change_down(i);
					tri[i].to = 2;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_right)) {
					change_left(i);
					tri[i].to = 0;
				}
				else if (IntersectRect(&temp, &t_rect[i], &mid_bottom)) {
					change_down(i);
					tri[i].to = 2;
				}
				break;
			}
		}
	}


	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A': case 'a': // ������
		draw_line = 0;
		draw_plane = 1;
		break;
	case 'B': case 'b': // ������
		draw_line = 1;
		draw_plane = 0;
		break;
	case 'O': case 'o': // ���� �ﰢ���� ƨ��� ���� / ����
		if (tri[0].show == 0)
			tri[0].show = 1;
		else
			tri[0].show = 0;
		break;
	case 'I': case 'i': // ���� �ﰢ�� �̵� / ����
		if (tri[1].show == 0 || tri[2].show == 0)
			tri[1].show = tri[2].show = 1;
		else
			tri[1].show = tri[2].show = 0;
		break;
	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}


void print_rect(int i) {
	//glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	//glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}







#elif DRILL == 9

#define TRI_COUNT 1000

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.2f, 0.2f, 0.2f };

static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void print_rect(int i);
void Motion(int x, int y);
void TimerFunction(int value);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();


GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[2];

GLfloat rColor = 0.0f;
GLfloat gColor = 0.0f;
GLfloat bColor = 0.0f;

int start_x_, start_y_;
float radian = 0;
GLfloat degree = 0.0f;

bool be_small = 1;

GLfloat pos[TRI_COUNT * 3][3] = { 0.0f, };

int show[TRI_COUNT * 3] = { 0, };

GLfloat col[TRI_COUNT * 3][3] = { 1.0f, };


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(GWIDTH, GHEIGHT);
	glutCreateWindow("Exam 1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	for (int i = 0; i < TRI_COUNT * 3; i++)
		for (int k = 0; k < 3; k++)
			col[i][k] = 1.0f;

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(2.0);


	for (int i = 0; i < TRI_COUNT * 3; i++) {
		if (show[i] == 1) {
			glDrawArrays(GL_POINTS, i, 1);
		}
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	//GLfloat ox = 0, oy = 0;
	//con_D_to_Ogl(x, y, &ox, &oy);

	if (!left_button && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;

		start_x_ = x;
		start_y_ = y;

		rColor = (float)rand() / (RAND_MAX);
		gColor = (float)rand() / (RAND_MAX);
		bColor = (float)rand() / (RAND_MAX);

		if (show[1] == 1) {
			for (int i = 0; i < TRI_COUNT * 3; i++) {
				show[i] = 0;
			}
			degree = radian = 0.0f;
		}

		be_small = 1;
		left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// left_button = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button = true;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		right_button = false;
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

void TimerFunction(int value) {
	
	if (left_button) {

		int ix = start_x_ + (radian * cos(degree));
		int iy = start_y_ + (radian * sin(degree));

		GLfloat xpos = 0.0f;
		GLfloat ypos = 0.0f;

		con_D_to_Ogl(ix, iy, &xpos, &ypos);

		for (int i = 0; i < TRI_COUNT * 3; i++) {
			if (show[i] == 0) {
				pos[i][0] = xpos;
				pos[i][1] = ypos;
				show[i] = 1;
				break;
			}
		}

		if (be_small) {
			// �۾��� ��
			if (degree >= 18.0f) {
				be_small = 0;
				start_x_ += 56;
				start_y_ -= 67;
				degree += 179.0f;
			}
			else {
				radian += 0.12f;
				degree += 0.05f;
			}
		}
		else {
			// Ŀ�� ��
			if (radian <= 0.0f) {
				left_button = 0;
			}
			else {
				radian -= 0.12f;
				degree -= 0.05f;
			}
		}
	}

	


	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A': case 'a': // ������

		break;
	case 'B': case 'b': // ������

		break;
	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}



void print_rect(int i) {
	//glColor3f(rect[i].color.r, rect[i].color.g, rect[i].color.b);
	//glRectf(rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
}



#elif DRILL == 12


#define DOT_COUNT 60

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

COLORVALUE backg_color = { 0.0f, 0.0f, 0.0f };

static int showed_rect_count = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

int insert_shape(int count);

float mouseX = 0.f, mouseY = 0.f;

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[8];

int shape_count[10];



GLfloat pos[DOT_COUNT * 3][3] = {
    -2.f,
};

GLfloat DotPos[DOT_COUNT * 3][3] = {
	-2.f,
};




GLfloat col[DOT_COUNT * 3][3] = {
   0,
};




void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(GWIDTH, GHEIGHT);
	glutCreateWindow("Exam 1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	for (int i = 0; i < DOT_COUNT * 3; i++) {
		for (int k = 0; k < 3; k++) {
			pos[i][k] = -2.f;
		}
	}

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);


	srand(unsigned int(time(NULL)));

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0f;
	bColor = 1.0f;

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(5.0);

	int first = 0;
	for (int i = 0; i < 10; i++) {
		if (shape_count[i]) { // 0�� �ƴϸ�! ���� �ִ°���!!!!
			switch (shape_count[i]) {
			case 1:
				glDrawArrays(GL_POINTS, first, shape_count[i]);
				break;
			case 2:
				glDrawArrays(GL_LINE_LOOP, first, shape_count[i]);
				break;
			case 3:
				glDrawArrays(GL_TRIANGLES, first, shape_count[i]);
				break;
			case 6:
				glDrawArrays(GL_TRIANGLES, first, 3);
				glDrawArrays(GL_TRIANGLES, first + 3, 3);
				break;
			}

			first += shape_count[i];
		}
		else {
			break;
		}
	}

	InitBuffer();
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(8, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, DOT_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, DOT_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	float ox = 0, oy = 0;
	con_D_to_Ogl(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;


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
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {
	con_D_to_Ogl(x, y, &mouseX, &mouseY);
	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;

		// ���콺 ������ ������ ��.

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

void TimerFunction(int value) {
	// ���� ��ġ ����


	glutPostRedisplay();
	InitBuffer();
	//glutTimerFunc(10, TimerFunction, 1);
}

int insert_shape(int count) {
	for (int i = 0; i < 10; ++i) {
		if (!shape_count[i]) { // �ƹ��͵� ����! ���� X
			shape_count[i] = count;
			// ����� �ڵ�!
			// mouseX, mouseY <-- float
			// pos�� ������ ��!
			// count��ŭ!!

			float r = (float)rand() / (RAND_MAX);
			float g = (float)rand() / (RAND_MAX);
			float b = (float)rand() / (RAND_MAX);

			switch (count) {
			case 1:
				for (int i = 0; i < DOT_COUNT; i++) {
					if (pos[i][0] < -1.5f) { // �ʱ�ȭ �ȵ�. float�ϱ� �̷��� ���ؾ�.
						pos[i][0] = mouseX;
						pos[i][1] = mouseY;
						pos[i][2] = 0.f;

						col[i][0] = r, col[i][1] = g, col[i][2] = b;

						break;
					}
				}
				break;
			case 2:
				for (int i = 0; i < DOT_COUNT; i++) {
					if (pos[i][0] < -1.1f) { // �ʱ�ȭ �ȵ�. float�ϱ� �̷��� ���ؾ�.
						pos[i][0] = mouseX - 0.15f;
						pos[i][1] = mouseY;
						pos[i][2] = 0.f;

						pos[i + 1][0] = mouseX + 0.15f;
						pos[i + 1][1] = mouseY;
						pos[i + 1][2] = 0.f;

						col[i][0] = r, col[i][1] = g, col[i][2] = b;
						col[i + 1][0] = r, col[i + 1][1] = g, col[i + 1][2] = b;

						break;
					}
				}
				break;
			case 3:
				for (int i = 0; i < DOT_COUNT; i++) {
					if (pos[i][0] < -1.1f) { // �ʱ�ȭ �ȵ�. float�ϱ� �̷��� ���ؾ�.
						pos[i][0] = mouseX - 0.1f;
						pos[i][1] = mouseY - 0.1f;
						pos[i][2] = 0.f;

						pos[i + 1][0] = mouseX + 0.1f;
						pos[i + 1][1] = mouseY - 0.1f;
						pos[i + 1][2] = 0.f;

						pos[i + 2][0] = mouseX;
						pos[i + 2][1] = mouseY + 0.1f;
						pos[i + 2][2] = 0.f;

						col[i][0] = r, col[i][1] = g, col[i][2] = b;
						col[i + 1][0] = r, col[i + 1][1] = g, col[i + 1][2] = b;
						col[i + 2][0] = r, col[i + 2][1] = g, col[i + 2][2] = b;
						break;
					}
				}
				break;
			case 6:
				for (int i = 0; i < DOT_COUNT; i++) {
					if (pos[i][0] < -1.1f) { // �ʱ�ȭ �ȵ�. float�ϱ� �̷��� ���ؾ�.
						// �Ʒ� - ���� ġ��ħ

						pos[i][0] = mouseX - 0.1f;
						pos[i][1] = mouseY - 0.1f;
						pos[i][2] = 0.f;

						pos[i + 1][0] = mouseX + 0.1f;
						pos[i + 1][1] = mouseY - 0.1f;
						pos[i + 1][2] = 0.f;

						pos[i + 2][0] = mouseX - 0.1f;
						pos[i + 2][1] = mouseY + 0.1f;
						pos[i + 2][2] = 0.f;


						// �� - ������ ġ��ħ
						pos[i + 3][0] = mouseX - 0.1f;
						pos[i + 3][1] = mouseY + 0.1f;
						pos[i + 3][2] = 0.f;

						pos[i + 4][0] = mouseX + 0.1f;
						pos[i + 4][1] = mouseY + 0.1f;
						pos[i + 4][2] = 0.f;

						pos[i + 5][0] = mouseX + 0.1f;
						pos[i + 5][1] = mouseY - 0.1f;
						pos[i + 5][2] = 0.f;

						for (int k = i; k < i + 6; ++k) {
							col[k][0] = r, col[k][1] = g, col[k][2] = b;
						}

						break;
					}
				}
				break;
			default:

				break;
			}
			break;
		}
		else if (i == 9) {
			// �� á�� ��. ������ ������ ��!
			return false;
		}
	}

	return true;
}



GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {

	case 'Q': case 'q':
		exit(0);
		break;
	}
	InitBuffer();
	glutPostRedisplay();
}


#elif DRILL == 13

#endif


GLchar* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void make_vertexShaders() {

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return ShaderProgramID;
}
