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
#include <vector>
#include <algorithm>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define DRILL 12

#if DRILL == 12

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)800

#define TRI_COUNT 100

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

GLfloat rColor = 1.0f;
GLfloat gColor = 1.0f;
GLfloat bColor = 1.0f;

GLfloat pos[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{-0.8, 0.2, 0.0}, {-0.3, 0.7, 0}, {-0.55, 0.45, 0},

};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{}, {}, {},
	{}, {}, {},
	{}, {}, {},
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

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(2.0);

	glDrawArrays(GL_LINES, 0, 2);

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

void TimerFunction(int value) {



	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A': case 'a':

		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void random_color(COLORVALUE* color) {
	color->r = (float)rand() / (RAND_MAX);
	color->g = (float)rand() / (RAND_MAX);
	color->b = (float)rand() / (RAND_MAX);
}


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


#elif DRILL == 13


#endif