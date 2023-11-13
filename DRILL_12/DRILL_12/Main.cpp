#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

#define DRILL 15

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

bool show[10] = { 0, };

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x, y축
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 1.0, 0.0},

	// 아래 4
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 10
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 16
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 22
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 28
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 34
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},


	{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.0, -0.5, -0.5}, 
	{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.0, 0.5, 0.0},
	{0.5, -0.5, 0.5}, {0.0, -0.5, -0.5}, {0.0, 0.5, 0.0},
	{-0.5, -0.5, 0.5}, {0.0, -0.5, -0.5}, {0.0, 0.5, 0.0}
};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f },
	
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }
};


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLfloat DEGREES = 0.0f;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);


	



	// fill here
	glm::mat4 TR = glm::mat4(1.0f);
	//TR = glm::translate(TR, glm::vec3(0.5, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (show[1]) {
		glDrawArrays(GL_TRIANGLES, 34, 3);
		glDrawArrays(GL_TRIANGLES, 37, 3);
	}
	if (show[2]) {
		glDrawArrays(GL_TRIANGLES, 4, 3);
		glDrawArrays(GL_TRIANGLES, 7, 3);
	}
	if (show[3]) {
		glDrawArrays(GL_TRIANGLES, 10, 3);
		glDrawArrays(GL_TRIANGLES, 13, 3);
	}
	if (show[4]) {
		glDrawArrays(GL_TRIANGLES, 16, 3);
		glDrawArrays(GL_TRIANGLES, 19, 3);
	}
	if (show[5]) {
		glDrawArrays(GL_TRIANGLES, 22, 3);
		glDrawArrays(GL_TRIANGLES, 25, 3);
	}
	if (show[6]) {
		glDrawArrays(GL_TRIANGLES, 28, 3);
		glDrawArrays(GL_TRIANGLES, 31, 3);
	}

	if (show[7]) {
		glDrawArrays(GL_TRIANGLES, 40, 3);
	}
	if (show[8]) {
		glDrawArrays(GL_TRIANGLES, 43, 3);
	}
	if (show[9]) {
		glDrawArrays(GL_TRIANGLES, 46, 3);	
	}
	if (show[0]) {
		glDrawArrays(GL_TRIANGLES, 49, 3);
	}

	TR = glm::rotate(TR, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	// glPointSize(2.0);

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);

	

	

	glBindVertexArray(vao);
	glutSwapBuffers(); // 화면에 출력하기
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
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

	// 왼쪽 마우스가 눌렸을 때
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

void del_show(int exc) {
	for (int i = 0; i < 10; i++) {
		if (i != exc) {
			show[i] = false;
		}
	}
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	int first_rand = 0;
	int second_rand = 0;
	int nums[] = { 7, 8, 9, 0 };

	switch (key) {
	case '1': // 상
		show[1] = true; del_show(1);
		break;
	case '2': // 하
		show[2] = true; del_show(2);
		break;
	case '3': // 좌
		show[3] = true; del_show(3);
		break;
	case '4': // 우
		show[4] = true; del_show(4);
		break;
	case '5': // 뒤
		show[5] = true; del_show(5);
		break;
	case '6': // 앞
		show[6] = true; del_show(6);
		break;


	case '7':
		show[7] = true; del_show(7);
		break;
	case '8':
		show[8] = true; del_show(8);
		break;
	case '9':
		show[9] = true; del_show(9);
		break;
	case '0':
		show[0] = true; del_show(0);
		break;


	case 'c': case 'C':
		first_rand = (rand() % 6) + 1;

		do {
			second_rand = (rand() % 6) + 1;
		} while (first_rand == second_rand);

		del_show(first_rand);
		show[first_rand] = true;
		show[second_rand] = true;

		break;
	case 't': case 'T':
		first_rand = rand() % 4;

		do {
			second_rand = rand() % 4;
		} while (first_rand == second_rand);

		del_show(nums[first_rand]);
		show[nums[first_rand]] = true;
		show[nums[second_rand]] = true;

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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}


#elif DRILL == 13

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)1000

#define TRI_COUNT 100

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
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

bool show[11] = { 0, };
bool del_cull_face = false;
GLfloat rotate_degree_x = 30.0f;
GLfloat rotate_degree_y = -30.0f;
bool run_x_anime = false, run_y_anime = false;

GLfloat move_x = 0.0f, move_y = 0.0f;
#define MOVE_VALUE 0.01f

bool show_wire = false;
int show_six = -1;

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x, y축
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 1.0, 0.0},

	// 아래 4
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 10
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 16
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 22
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 28
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 34
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},



	// 사각뿔 아랫면 40
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 46
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.0, 0.5, 0.0},
	{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.0, 0.5, 0.0},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {0.0, 0.5, 0.0},
	{0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.0, 0.5, 0.0},

};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f },

	{0.93f, 0.93f, 0.87f}, {0.43f, 0.54f, 0.56f}, {0.80f, 0.52f, 0.24f}, // down
	{0.80f, 0.52f, 0.24f}, {0.60f, 0.80f, 0.19f}, {0.93f, 0.93f, 0.87f},

	{0.87f, 1.0f, 1.0f},  {0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, // left
	{0.87f, 1.0f, 1.0f}, {0.43f, 0.54f, 0.56f}, {0.93f, 0.93f, 0.87f},

	{0.60f, 0.80f, 0.19f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f}, // right
	{0.60f, 0.80f, 0.19f}, {0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f},

	{0.87f, 1.0f, 1.0f}, {0.93f, 0.93f, 0.87f}, {0.60f, 0.80f, 0.19f}, // behind
	{0.87f, 1.0f, 1.0f}, {0.60f, 0.80f, 0.19f}, {0.80f, 0.52f, 0.24f},

	{0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, {1.0f, 1.0f, 0.87f}, // front
	{0.4f, 0.8f, 0.66f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f},

	{0.87f, 1.0f, 1.0f}, {0.4f, 0.8f, 0.66f}, {0.91f, 0.58f, 0.47f}, // top
	{0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f}, {0.87f, 1.0f, 1.0f},



	{0.93f, 0.93f, 0.87f}, {0.43f, 0.54f, 0.56f}, {1.0f, 1.0f, 0.87f},
	{1.0f, 1.0f, 0.87f}, {0.60f, 0.80f, 0.19f}, {0.93f, 0.93f, 0.87f},

	{0.93f, 0.93f, 0.87f}, {0.43f, 0.54f, 0.56f}, {0.91f, 0.58f, 0.47f},
	{0.43f, 0.54f, 0.56f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f},
	{1.0f, 1.0f, 0.87f}, {0.60f, 0.80f, 0.19f}, {0.91f, 0.58f, 0.47f},
	{0.60f, 0.80f, 0.19f}, {0.93f, 0.93f, 0.87f}, {0.91f, 0.58f, 0.47f}
};


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLfloat DEGREES = 0.0f;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	if (del_cull_face) {
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
	}
	
	glm::mat4 TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(move_x, move_y, 0.0));
	TR = glm::rotate(TR, glm::radians(rotate_degree_x), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(rotate_degree_y), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (show_wire) {
		if (show_six == true) {
			for (int i = 4; i < 39; i++) {
				glDrawArrays(GL_LINES, i, 2);
			}
		}
		else if (show_six == 0) {
			for (int i = 40; i < 57; i++) { // 57까지
				glDrawArrays(GL_LINES, i, 2);
			}
		}
	}
	else {
		if (show_six == true) {
			glDrawArrays(GL_TRIANGLES, 34, 3);
			glDrawArrays(GL_TRIANGLES, 37, 3);

			glDrawArrays(GL_TRIANGLES, 4, 3);
			glDrawArrays(GL_TRIANGLES, 7, 3);

			glDrawArrays(GL_TRIANGLES, 10, 3);
			glDrawArrays(GL_TRIANGLES, 13, 3);

			glDrawArrays(GL_TRIANGLES, 16, 3);
			glDrawArrays(GL_TRIANGLES, 19, 3);

			glDrawArrays(GL_TRIANGLES, 22, 3);
			glDrawArrays(GL_TRIANGLES, 25, 3);

			glDrawArrays(GL_TRIANGLES, 28, 3);
			glDrawArrays(GL_TRIANGLES, 31, 3);
		}
		else if (show_six == 0) {
			glDrawArrays(GL_TRIANGLES, 40, 3);
			glDrawArrays(GL_TRIANGLES, 43, 3);

			glDrawArrays(GL_TRIANGLES, 46, 3);
			glDrawArrays(GL_TRIANGLES, 49, 3);
			glDrawArrays(GL_TRIANGLES, 52, 3);
			glDrawArrays(GL_TRIANGLES, 55, 3);

		}
	}

	
	TR = glm::rotate(TR, glm::radians(-rotate_degree_y), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-rotate_degree_x), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-move_x, -move_y, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	// glPointSize(2.0);

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);





	glBindVertexArray(vao);
	glutSwapBuffers(); // 화면에 출력하기
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
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

	// 왼쪽 마우스가 눌렸을 때
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

	if (run_x_anime)
		rotate_degree_x += 0.5f;
	if (run_y_anime)
		rotate_degree_y += 0.5f;

	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

void del_show(int exc) {
	for (int i = 0; i < 11; i++) {
		if (i != exc) {
			show[i] = false;
		}
	}
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'C': case 'c': // 육면체 출력 
		/*del_show(0);
		for (int i = 0; i < 6; i++) {
			show[i] = true;
		}*/
		show_six = true;
		break;
	case 'P': case 'p': // 사각뿔 (오면체) 출력 - 6 7 8 9 10
		/*del_show(7);
		for (int i = 6; i < 11; i++) {
			show[i] = true;
		}*/
		show_six = false;
		break;
	case 'H': case 'h': // 은면제거 적용, 해제
		if (del_cull_face)
			del_cull_face = false;
		else
			del_cull_face = true;
		break;


	case 'X': case 'x':
		if (run_x_anime)
			run_x_anime = false;
		else
			run_x_anime = true;
		break;
	case 'Y': case 'y':
		if (run_y_anime)
			run_y_anime = false;
		else
			run_y_anime = true;
		break;
	case 'W': case 'w':
		if (show_wire)
			show_wire = false;
		else
			show_wire = true;
		break;



	case 'S': case 's': // 초기 위치로 리셋
		rotate_degree_x = 30.0f;
		rotate_degree_y = 30.0f;
		move_x = 0.0f;
		move_y = 0.0f;
		run_x_anime = false;
		run_y_anime = false;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		move_y += MOVE_VALUE;
		break;
	case GLUT_KEY_DOWN:
		move_y -= MOVE_VALUE;
		break;
	case GLUT_KEY_LEFT:
		move_x -= MOVE_VALUE;
		break;
	case GLUT_KEY_RIGHT:
		move_x += MOVE_VALUE;
		break;
	}
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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}




#elif DRILL == 14

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)1000

#define TRI_COUNT 100

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
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


#define MOVE_VALUE 0.01f

GLUquadric* qobj;

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x, y축
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 1.0, 0.0},

	// 아래 4
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 10
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 16
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 22
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 28
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 34
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},



	{0.0, 0.0, -1.0}, {0.0, 0.0, 1.0} // z line
};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},

	{0.93f, 0.93f, 0.87f}, {0.43f, 0.54f, 0.56f}, {0.80f, 0.52f, 0.24f}, // down
	{0.80f, 0.52f, 0.24f}, {0.60f, 0.80f, 0.19f}, {0.93f, 0.93f, 0.87f},

	{0.87f, 1.0f, 1.0f},  {0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, // left
	{0.87f, 1.0f, 1.0f}, {0.43f, 0.54f, 0.56f}, {0.93f, 0.93f, 0.87f},

	{0.60f, 0.80f, 0.19f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f}, // right
	{0.60f, 0.80f, 0.19f}, {0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f},

	{0.87f, 1.0f, 1.0f}, {0.93f, 0.93f, 0.87f}, {0.60f, 0.80f, 0.19f}, // behind
	{0.87f, 1.0f, 1.0f}, {0.60f, 0.80f, 0.19f}, {0.80f, 0.52f, 0.24f},

	{0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, {1.0f, 1.0f, 0.87f}, // front
	{0.4f, 0.8f, 0.66f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f},

	{0.87f, 1.0f, 1.0f}, {0.4f, 0.8f, 0.66f}, {0.91f, 0.58f, 0.47f}, // top
	{0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f}, {0.87f, 1.0f, 1.0f},



	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, // z line
};


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLfloat left_degree = 0.0f, left_degree_self_y = 0.0f, right_degree_self_y = 0.0f;
GLfloat left_degree_self_x = 0.0f, right_degree_self_x = 0.0f;

int rotate_ = 0;
int left_rotate_self_y = 0;
int right_rotate_self_y = 0;
int left_rotate_self_x = 0;
int right_rotate_self_x = 0;
int show_which_obj = 1;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);


		glEnable(GL_DEPTH_TEST);


	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);

	glm::mat4 TR = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");


	//////////////////////////////////////////////////////////////////////////////

	// 선 먼저 그려주기
	TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(left_degree/*공전각*/), glm::vec3(0.0, 1.0, 0.0));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 40, 2);

	//TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
	//TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));

	// rotate, translate -> 공전함.

	// TR = glm::rotate(TR, glm::radians(left_degree), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::translate(TR, glm::vec3(0.7, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(right_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(right_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (show_which_obj == 0) {
		gluSphere(qobj, 0.2, 50, 50);
	}
	else { // == 1
		gluCylinder(qobj, 0.15, 0.15, 0.4, 20, 8);
	}
	
	
	TR = glm::rotate(TR, glm::radians(-right_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-right_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-1.4, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(left_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(left_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	
	if (show_which_obj == 0) {
		gluCylinder(qobj, 0.15, 0.0, 0.4, 20, 8);
	}
	else { // == 1
		TR = glm::scale(TR, glm::vec3(0.3, 0.3, 0.3));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 4; i <= 38; i ++) {
			glDrawArrays(GL_LINES, i, 2);
		}
	}
	

	
	
	
	glPopMatrix();

	glBindVertexArray(vao);
	glutSwapBuffers(); // 화면에 출력하기
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
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

	// 왼쪽 마우스가 눌렸을 때
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

	if (rotate_ == 1) {
		left_degree += 0.3f; // 공전
	}
	else if (rotate_ == -1) {
		left_degree -= 0.3f; // 공전
	}
	
	if (left_rotate_self_y == 1) {
		left_degree_self_y += 0.3f; // 자전
	}
	else if (left_rotate_self_y == -1) {
		left_degree_self_y -= 0.3f; // 자전
	}
	if (left_rotate_self_x == 1) {
		left_degree_self_x += 0.3f; // 자전
	}
	else if (left_rotate_self_x == -1) {
		left_degree_self_x -= 0.3f; // 자전
	}


	if (right_rotate_self_y == 1) {
		right_degree_self_y += 0.3f; // 자전
	}
	else if (right_rotate_self_y == -1) {
		right_degree_self_y -= 0.3f; // 자전
	}
	if (right_rotate_self_x == 1) {
		right_degree_self_x += 0.3f; // 자전
	}
	else if (right_rotate_self_x == -1) {
		right_degree_self_x -= 0.3f; // 자전
	}


	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'C': case 'c': // 다른 도형으로 바꾸기
		if (show_which_obj == 0) {
			show_which_obj = 1;
		}
		else {
			show_which_obj = 0;
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'x': // + 방향 우측 자전, x축
		if (right_rotate_self_x == 1 || right_rotate_self_x == 0)
			right_rotate_self_x = -1;
		else if (right_rotate_self_x == -1)
			right_rotate_self_x = 0;
		break;
	case 'X': // - 방향 우측 자전, x축
		if (right_rotate_self_x == -1 || right_rotate_self_x == 0)
			right_rotate_self_x = 1;
		else if (right_rotate_self_x == 1)
			right_rotate_self_x = 0;
		break;

	case 'y': // + 방향 우측 자전, y축
		if (right_rotate_self_y == 1 || right_rotate_self_y == 0)
			right_rotate_self_y = -1;
		else if (right_rotate_self_y == -1)
			right_rotate_self_y = 0;
		break;
	case 'Y': // - 방향 우측 자전, y축
		if (right_rotate_self_y == -1 || right_rotate_self_y == 0)
			right_rotate_self_y = 1;
		else if (right_rotate_self_y == 1)
			right_rotate_self_y = 0;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'A':
		if (left_rotate_self_x == -1 || left_rotate_self_x == 0)
			left_rotate_self_x = 1;
		else if (left_rotate_self_x == 1)
			left_rotate_self_x = 0;
		break;
	case 'a':
		if (left_rotate_self_x == 1 || left_rotate_self_x == 0)
			left_rotate_self_x = -1;
		else if (left_rotate_self_x == -1)
			left_rotate_self_x = 0;
		break;

	case 'B':
		if (left_rotate_self_y == -1 || left_rotate_self_y == 0)
			left_rotate_self_y = 1;
		else if (left_rotate_self_y == 1)
			left_rotate_self_y = 0;
		break;
	case 'b':
		if (left_rotate_self_y == 1 || left_rotate_self_y == 0)
			left_rotate_self_y = -1;
		else if (left_rotate_self_y == -1)
			left_rotate_self_y = 0;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'r': // 두 객체 전부 공전, y축 기준 +
		if (rotate_ == 1 || rotate_ == 0)
			rotate_ = -1;
		else if (rotate_ == -1)
			rotate_ = 0;
		break;
	case 'R':
		if (rotate_ == 0 || rotate_ == -1)
			rotate_ = 1;
		else if (rotate_ == 1)
			rotate_ = 0;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'S': case 's': // 초기화
		rotate_ = 0;
		left_degree = 0.0f;
		left_degree_self_x = 0.0f;
		left_degree_self_y = 0.0f;
		left_rotate_self_x = 0;
		left_rotate_self_y = 0;

		right_degree_self_x = 0.0f;
		right_degree_self_y = 0.0f;
		right_rotate_self_x = 0;
		right_rotate_self_y = 0;

		show_which_obj = 1;
		break;

	case 'Q': case 'q':
		exit(0);
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	
	}
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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}


#elif DRILL == 15

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)1000

#define TRI_COUNT 3000

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
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


#define MOVE_VALUE 0.01f

GLUquadric* qobj;

int show[TRI_COUNT * 3] = { 0, };

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x, y축
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 1.0, 0.0},

	// 아래 4
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 10
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 16
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 22
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 28
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 34
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},


	// z line 40 ~ 41
	{0.0, 0.0, -1.0}, {0.0, 0.0, 1.0},
	
};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},

	{0.93f, 0.93f, 0.87f}, {0.43f, 0.54f, 0.56f}, {0.80f, 0.52f, 0.24f}, // down
	{0.80f, 0.52f, 0.24f}, {0.60f, 0.80f, 0.19f}, {0.93f, 0.93f, 0.87f},

	{0.87f, 1.0f, 1.0f},  {0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, // left
	{0.87f, 1.0f, 1.0f}, {0.43f, 0.54f, 0.56f}, {0.93f, 0.93f, 0.87f},

	{0.60f, 0.80f, 0.19f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f}, // right
	{0.60f, 0.80f, 0.19f}, {0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f},

	{0.87f, 1.0f, 1.0f}, {0.93f, 0.93f, 0.87f}, {0.60f, 0.80f, 0.19f}, // behind
	{0.87f, 1.0f, 1.0f}, {0.60f, 0.80f, 0.19f}, {0.80f, 0.52f, 0.24f},

	{0.4f, 0.8f, 0.66f}, {0.43f, 0.54f, 0.56f}, {1.0f, 1.0f, 0.87f}, // front
	{0.4f, 0.8f, 0.66f}, {1.0f, 1.0f, 0.87f}, {0.91f, 0.58f, 0.47f},

	{0.87f, 1.0f, 1.0f}, {0.4f, 0.8f, 0.66f}, {0.91f, 0.58f, 0.47f}, // top
	{0.91f, 0.58f, 0.47f}, {0.80f, 0.52f, 0.24f}, {0.87f, 1.0f, 1.0f},



	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, // z line
};

GLfloat left_degree = 0.0f, left_degree_self_y = 0.0f, right_degree_self_y = 0.0f;
GLfloat left_degree_self_x = 0.0f, right_degree_self_x = 0.0f;

int rotate_ = 0;
int left_rotate_self_y = 0;
int right_rotate_self_y = 0;
int left_rotate_self_x = 0;
int right_rotate_self_x = 0;
int show_which_obj = 1;

GLfloat left_move_x = 0.04f, left_move_y = 0.0f, left_move_z = 0.0f;
GLfloat right_move_x = 0.1f, right_move_y = 0.0f, right_move_z = 0.0f;
GLfloat left_self_scale = 0.0f, right_self_scale = 0.0f;
GLfloat line_move = 0.0f;
bool anime_R = false, anime_T = false, anime_S = false;
bool was_S = false;
int left_anime_T = false, right_anime_T = false;
int left_anime_S = false, right_anime_S = false;
GLfloat all_scale = 0.0f;
GLfloat cyclone_degree = 0.0f;
GLfloat cyclone_radian = 0.0f;
GLfloat cyclone_move_left = 0.0f;
GLfloat cyclone_move_right = 0.0f;
GLfloat t_anime_move_left = 0.0f;
GLfloat t_anime_move_right = 0.0f;

GLfloat tempx_left_ori = 0.0f;
GLfloat tempx_right_ori = 0.0f;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	for (int i = 42; i < TRI_COUNT * 3; i++)
		for (int k = 0; k < 3; k++)
			col[i][k] = 0.0f;

	for (int k = 0; k < 504; k++) {
		int ix = (GWIDTH / 2) + (cyclone_radian * cos(cyclone_degree));
		int iy = (GHEIGHT / 2) + (cyclone_radian * sin(cyclone_degree));

		GLfloat xpos = 0.0f;
		GLfloat ypos = 0.0f;

		con_D_to_Ogl(ix, iy, &xpos, &ypos);

		for (int i = 42; i < TRI_COUNT * 3; i++) {
			if (show[i] == 0) {
				pos[i][0] = xpos;
				pos[i][2] = ypos;
				show[i] = 1;
				break;
			}
		}

		cyclone_radian += 1.005f;
		cyclone_degree -= 0.05f;
	}


	srand(unsigned int(time(NULL)));

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);


	glEnable(GL_DEPTH_TEST);


	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);

	glm::mat4 TR = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	//////////////////////////////////////////////////////////////////////////////

	TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (anime_R) {
		// 회오리 그리기
		for (int i = 42; i < 544; i++) {
				glDrawArrays(GL_LINES, i, 2);
		}
	}
	TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));

	// 선 먼저 그려주기
	TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(left_degree/*공전각*/), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glDrawArrays(GL_LINES, 2, 2); // y

	TR = glm::translate(TR, glm::vec3(0.0, line_move, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glDrawArrays(GL_LINES, 0, 2); // x
	glDrawArrays(GL_LINES, 40, 2); // z
	TR = glm::translate(TR, glm::vec3(0.0, -line_move, 0.0));

	// rotate, translate -> 공전함.
	TR = glm::scale(TR, glm::vec3(1.0 + all_scale, 1.0 + all_scale, 1.0 + all_scale));

	TR = glm::translate(TR, glm::vec3(0.7, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(cyclone_move_left, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(t_anime_move_left, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(right_move_x, right_move_y, right_move_z));
	TR = glm::rotate(TR, glm::radians(right_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(right_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::scale(TR, glm::vec3(1.0 + right_self_scale, 1.0 + right_self_scale, 1.0 + right_self_scale));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (show_which_obj == 0) {
		gluSphere(qobj, 0.2, 50, 50);
	}
	else { // == 1
		gluCylinder(qobj, 0.15, 0.15, 0.4, 20, 8);
	}

	// init
	GLfloat temp_self_scale = 1.0 / (1.0 + right_self_scale);
	TR = glm::scale(TR, glm::vec3(temp_self_scale, temp_self_scale, temp_self_scale));
	TR = glm::rotate(TR, glm::radians(-right_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-right_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-right_move_x, -right_move_y, -right_move_z));

	//TR = glm::scale(TR, glm::vec3(1.0 + right_self_scale, 1.0 + right_self_scale, 1.0 + right_self_scale));

	TR = glm::translate(TR, glm::vec3(-cyclone_move_left, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-t_anime_move_left, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-1.4, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-t_anime_move_right, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(-cyclone_move_right, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(left_move_x, left_move_y, left_move_z));
	TR = glm::rotate(TR, glm::radians(left_degree_self_y/*자전각*/), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(left_degree_self_x/*자전각*/), glm::vec3(1.0, 0.0, 0.0));

	TR = glm::scale(TR, glm::vec3(1.0 + left_self_scale, 1.0 + left_self_scale, 1.0 + left_self_scale));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	if (show_which_obj == 0) {
		gluCylinder(qobj, 0.15, 0.0, 0.4, 20, 8);
	}
	else { // == 1
		TR = glm::scale(TR, glm::vec3(0.3, 0.3, 0.3)); // 육면체 크기 조정
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 4; i <= 38; i++) {
			glDrawArrays(GL_LINES, i, 2);
		}
	}





	glPopMatrix();

	glBindVertexArray(vao);
	glutSwapBuffers(); // 화면에 출력하기
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
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

	// 왼쪽 마우스가 눌렸을 때
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
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

	if (rotate_ == 1) {
		left_degree += 0.3f; // 공전
	}
	else if (rotate_ == -1) {
		left_degree -= 0.3f; // 공전
	}

	if (left_rotate_self_y == 1) {
		left_degree_self_y += 0.3f; // 자전
	}
	else if (left_rotate_self_y == -1) {
		left_degree_self_y -= 0.3f; // 자전
	}
	if (left_rotate_self_x == 1) {
		left_degree_self_x += 0.3f; // 자전
	}
	else if (left_rotate_self_x == -1) {
		left_degree_self_x -= 0.3f; // 자전
	}


	if (right_rotate_self_y == 1) {
		right_degree_self_y += 0.3f; // 자전
	}
	else if (right_rotate_self_y == -1) {
		right_degree_self_y -= 0.3f; // 자전
	}
	if (right_rotate_self_x == 1) {
		right_degree_self_x += 0.3f; // 자전
	}
	else if (right_rotate_self_x == -1) {
		right_degree_self_x -= 0.3f; // 자전
	}

	// 딱 500번.
	

	if (anime_R) {
		/*int ix = (GWIDTH / 2) + (cyclone_radian * cos(cyclone_degree));
		int iy = (GHEIGHT / 2) + (cyclone_radian * sin(cyclone_degree));

		GLfloat xpos = 0.0f;
		GLfloat ypos = 0.0f;

		con_D_to_Ogl(ix, iy, &xpos, &ypos);

		for (int i = 42; i < TRI_COUNT * 3; i++) {
			if (show[i] == 0) {
				pos[i][0] = xpos;
				pos[i][2] = ypos;
				show[i] = 1;
				break;
			}

		}

		cyclone_radian += 1.0f;
		cyclone_degree -= 0.05f;*/
		if (was_S) {
			cyclone_move_left += 0.0004f;
			cyclone_move_right += 0.0004f;
		}
		else {
			cyclone_move_left -= 0.0004f;
			cyclone_move_right -= 0.0004f;
		}

		left_degree += 0.5f; // 공전
	}
	if (anime_T) {
		
		GLfloat tempx_left = left_move_x + cyclone_move_left + t_anime_move_left + 0.04;
		GLfloat tempx_right = right_move_x + cyclone_move_right + t_anime_move_right + 0.04;
		if (tempx_left <= -0.7f && left_anime_T == 1) left_anime_T = -1;
		if (tempx_right <= -0.6f && right_anime_T == 1) right_anime_T = -1;

		if (tempx_left >= tempx_left_ori && left_anime_T == -1) {
			left_anime_T = 0;
		}
		if (tempx_right >= tempx_right_ori && right_anime_T == -1) {
			right_anime_T = 0;
		}

		if (right_anime_T == 0 && left_anime_T == 0) anime_T = false;

		if (left_anime_T == 1) {
			t_anime_move_left -= 0.0015f;
		}
		else if (left_anime_T == -1) {
			t_anime_move_left += 0.0015f;
		}

		if (right_anime_T == 1) {
			t_anime_move_right -= 0.0015f;
		}
		else if (right_anime_T == -1) {
			t_anime_move_right += 0.0015f;
		}
		


	}
	if (anime_S) {
		GLfloat tempx_left = left_move_x + cyclone_move_left + t_anime_move_left + 0.04;
		GLfloat tempx_right = right_move_x + cyclone_move_right + t_anime_move_right + 0.04;


		if (tempx_left <= -1.3f) {
			left_anime_S = 0;
		}
		if (tempx_right <= -1.4f) {
			right_anime_S = 0;
		}

		if (right_anime_S == 0 && left_anime_S == 0) anime_S = false;
		
		if (left_anime_S == 1) {
			t_anime_move_left -= 0.0015f;
		}


		if (right_anime_S == 1) {
			t_anime_move_right -= 0.0015f;
		}

	}



	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'C': case 'c': // 다른 도형으로 바꾸기
		if (show_which_obj == 0) {
			show_which_obj = 1;
		}
		else {
			show_which_obj = 0;
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'x': // + 방향 우측 자전, x축
		if (right_rotate_self_x == 1 || right_rotate_self_x == 0)
			right_rotate_self_x = -1;
		else if (right_rotate_self_x == -1)
			right_rotate_self_x = 0;
		break;
	case 'X': // - 방향 우측 자전, x축
		if (right_rotate_self_x == -1 || right_rotate_self_x == 0)
			right_rotate_self_x = 1;
		else if (right_rotate_self_x == 1)
			right_rotate_self_x = 0;
		break;

	case 'y': // + 방향 우측 자전, y축
		if (right_rotate_self_y == 1 || right_rotate_self_y == 0)
			right_rotate_self_y = -1;
		else if (right_rotate_self_y == -1)
			right_rotate_self_y = 0;
		break;
	case 'Y': // - 방향 우측 자전, y축
		if (right_rotate_self_y == -1 || right_rotate_self_y == 0)
			right_rotate_self_y = 1;
		else if (right_rotate_self_y == 1)
			right_rotate_self_y = 0;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'A':
		if (left_rotate_self_x == -1 || left_rotate_self_x == 0)
			left_rotate_self_x = 1;
		else if (left_rotate_self_x == 1)
			left_rotate_self_x = 0;
		break;
	case 'a':
		if (left_rotate_self_x == 1 || left_rotate_self_x == 0)
			left_rotate_self_x = -1;
		else if (left_rotate_self_x == -1)
			left_rotate_self_x = 0;
		break;

	case 'B':
		if (left_rotate_self_y == -1 || left_rotate_self_y == 0)
			left_rotate_self_y = 1;
		else if (left_rotate_self_y == 1)
			left_rotate_self_y = 0;
		break;
	case 'b':
		if (left_rotate_self_y == 1 || left_rotate_self_y == 0)
			left_rotate_self_y = -1;
		else if (left_rotate_self_y == -1)
			left_rotate_self_y = 0;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case '1': // left x +
		left_move_x += 0.05f;
		break;
	case '2': // left y +
		left_move_y += 0.05f;
		break;
	case '3': // left z +
		left_move_z += 0.05f;
		break;

	case '4': // left x -
		left_move_x -= 0.05f;
		break;
	case '5': // left y -
		left_move_y -= 0.05f;
		break;
	case '6': // left z -
		left_move_z -= 0.05f;
		break;



	case '7': // right x +
		right_move_x += 0.05f;
		break;
	case '8': // right y +
		right_move_y += 0.05f;
		break;
	case '9': // right z +
		right_move_z += 0.05f;
		break;

	case '0': // right x -
		right_move_x -= 0.05f;
		break;
	case '-': // right y -
		right_move_y -= 0.05f;
		break;
	case '=': // right z -
		right_move_z -= 0.05f;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case '_':
		all_scale -= 0.05f;
		break;
	case '+':
		all_scale += 0.05f;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case '[': // left self scale -
		left_self_scale -= 0.05f;
		break;
	case ']': // left self scale + 
		left_self_scale += 0.05f;
		break;

	case ';': // right self scale -
		right_self_scale -= 0.05f;
		break;
	case '\'': // right self scale 
		right_self_scale += 0.05f;
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'r': // xz평면 회오리 그리고, 회오리 위치에 따라 객체 이동 애니메이션
		if (anime_R) anime_R = false;
		else anime_R = true;
		break;
	case 'o':
		cyclone_degree += 0.5f;
		break;

	case 't': // 원점 이동, 다시 제자리로 이동 애니메이션
		if (anime_T || left_anime_T || right_anime_T) {
			tempx_left_ori = left_move_x + cyclone_move_left + t_anime_move_left + 0.04;
			tempx_right_ori = right_move_x + cyclone_move_left + t_anime_move_right + 0.04;
			anime_T = false;
			left_anime_T = false;
			right_anime_T = false;
		}
		else {
			anime_T = true;
			left_anime_T = true;
			right_anime_T = true;
		
			anime_S = false;
			left_anime_S = false;
			right_anime_S = false;		
		}
		break;

	case 'S': case 's': // 상대방의 자리로 이동하는 애니메이션
		if (anime_S) {
			tempx_left_ori = left_move_x + cyclone_move_left + t_anime_move_left + 0.04;
			tempx_right_ori = right_move_x + cyclone_move_left + t_anime_move_right + 0.04;
			anime_S = false;
			left_anime_S = false;
			right_anime_S = false;
		}
		else {
			anime_T = false;
			left_anime_T = false;
			right_anime_T = false;

			anime_S = true;
			left_anime_S = true;
			right_anime_S = true;

			was_S = true;
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////

	case 'R': // 두 객체 전부 공전, y축 기준 -
		//if (rotate_ == 0 || rotate_ == -1)
		//	rotate_ = 1;
		//else if (rotate_ == 1)
		//	rotate_ = 0;
		break;

	case 'Q': case 'q':
		exit(0);
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		line_move += 0.05f;
		left_move_y += 0.05f;
		right_move_y += 0.05f;
		break;
	case GLUT_KEY_DOWN:
		line_move -= 0.05f;
		left_move_y -= 0.05f;
		right_move_y -= 0.05f;
		break;
	}
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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}




#endif