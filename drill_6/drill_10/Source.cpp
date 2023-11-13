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

#define DRILL 11

#if DRILL == 10

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)800

#define DOT_COUNT 100

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

char anime_key = 0;

float radian = 0;
GLfloat degree = 0.0f;

bool be_small = 1;

GLfloat pos[DOT_COUNT * 3][3] = 
{ 
	// line -> triangle
	{-0.8, 0.2, 0.0}, {-0.3, 0.7, 0}, {-0.55, 0.45, 0},

	// triangle -> rectangle
	{0.2, 0.2, 0.0}, {0.8, 0.2, 0.0}, {0.5, 0.7, 0.0},
	{0.5, 0.7, 0.0}, {0.8, 0.2, 0.0}, {0.5, 0.7, 0.0},

	// rectangle -> pentagon
	{-0.8, -0.2, 0.0}, {-0.55, -0.2, 0.0}, {-0.8, -0.7, 0.0},
	{-0.55, -0.2, 0.0}, {-0.3, -0.2, 0.0}, {-0.3, -0.7, 0.0},
	{-0.8, -0.7, 0.0}, {-0.3, -0.7, 0.0}, {-0.55f, -0.2, 0.0},
	
	// pentagon -> dot

	// 18	19	20
	// 21	22	23
	// 24	25	26

	{0.2, -0.27, 0.0}, {0.45, -0.1, 0.0}, {0.3, -0.7, 0.0},
	{0.45, -0.1, 0.0}, {0.7, -0.27, 0.0}, {0.6, -0.7, 0.0},
	{0.3, -0.7, 0.0}, {0.6, -0.7, 0.0}, {0.45, -0.1, 0.0}
};

GLfloat ori_pos[27][3] =
{
	// line -> triangle
	{-0.8, 0.2, 0.0}, {-0.3, 0.7, 0}, {-0.55, 0.45, 0},

	// triangle -> rectangle
	{0.2, 0.2, 0.0}, {0.8, 0.2, 0.0}, {0.5, 0.7, 0.0},
	{0.5, 0.7, 0.0}, {0.8, 0.2, 0.0}, {0.5, 0.7, 0.0},

	// rectangle -> pentagon
	{-0.8, -0.2, 0.0}, {-0.55, -0.2, 0.0}, {-0.8, -0.7, 0.0},
	{-0.55, -0.2, 0.0}, {-0.3, -0.2, 0.0}, {-0.3, -0.7, 0.0},
	{-0.8, -0.7, 0.0}, {-0.3, -0.7, 0.0}, {-0.55f, -0.2, 0.0},

	// pentagon -> dot

	// 18	19	20
	// 21	22	23
	// 24	25	26

	{0.2, -0.27, 0.0}, {0.45, -0.1, 0.0}, {0.3, -0.7, 0.0},
	{0.45, -0.1, 0.0}, {0.7, -0.27, 0.0}, {0.6, -0.7, 0.0},
	{0.3, -0.7, 0.0}, {0.6, -0.7, 0.0}, {0.45, -0.1, 0.0}
};

GLfloat col[DOT_COUNT * 3][3] = 
{
	// line -> triangle
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },

	// triangle -> rectangle
	{1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},

	// rectangle -> pentagon
	{0.6, 0.8, 0.4}, {0.6, 0.8, 0.4}, {0.6, 0.8, 0.4},
	{0.6, 0.8, 0.4}, {0.6, 0.8, 0.4}, {0.6, 0.8, 0.4},
	{0.6, 0.8, 0.4}, {0.6, 0.8, 0.4}, {0.6, 0.8, 0.4},

	// pentagon -> dot
	{}, {}, {},
	{}, {}, {},
	{}, {}, {}

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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(2.0);

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 3, 6);

	glDrawArrays(GL_TRIANGLE_STRIP, 9, 9);

	glDrawArrays(GL_TRIANGLE_STRIP, 18, 9);

	glutSwapBuffers(); // 화면에 출력하기
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(2, vbo);
	
	
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

	if (anime_key == 'l') {
		if (pos[2][1] >= 0.2f) { // L
			pos[2][0] += 0.0027f;
			pos[2][1] -= 0.002f;
		}
	}
	if (anime_key == 't') {
		if (pos[5][0] >= 0.2f) { // T
			pos[5][0] -= 0.002f;
			pos[8][0] += 0.002f;
		}
	}
	if (anime_key == 'r') {
		if (pos[10][1] <= -0.1f) { // R
			pos[10][1] += 0.002f;
			pos[12][1] += 0.002f;
			pos[17][1] += 0.002f;

			pos[11][0] += 0.002f;
			pos[15][0] += 0.002f;

			pos[14][0] -= 0.002f;
			pos[16][0] -= 0.002f;

			pos[9][1] -= 0.0015f;
			pos[13][1] -= 0.0015f;
		}
	}
	if (anime_key == 'p') {
		if (pos[19][1] >= -0.3214f) { // P
			pos[19][1] -= 0.0018f;
			pos[21][1] -= 0.0018f;
			pos[26][1] -= 0.0018f;

			pos[18][0] += 0.002f;
			pos[18][1] -= 0.0005f;

			pos[22][0] -= 0.002f;
			pos[22][1] -= 0.0005f;

			pos[20][0] += 0.0013f;
			pos[24][0] += 0.0013f;
			pos[20][1] += 0.003f;
			pos[24][1] += 0.003f;

			pos[23][0] -= 0.0013f;
			pos[25][0] -= 0.0013f;
			pos[23][1] += 0.003f;
			pos[25][1] += 0.003f;
		}
	}

	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'L': case 'l':
		if (anime_key == 'l')
			anime_key = 0;
		else anime_key = 'l';
		break;
	case 'T': case 't':
		if (anime_key == 't')
			anime_key = 0;
		else anime_key = 't';
		break;
	case 'R': case 'r':
		if (anime_key == 'r')
			anime_key = 0;
		else anime_key = 'r';
		break;
	case 'P': case 'p':
		if (anime_key == 'p')
			anime_key = 0;
		else anime_key = 'p';
		break;

	case 'A': case 'a': // 초기화
		for (int i = 0; i < 27; i++) {
			for (int k = 0; k < 3; k++) {
				pos[i][k] = ori_pos[i][k];
			}
		}
		anime_key = 0;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}


#elif DRILL == 11

#define GWIDTH (GLint)800
#define GHEIGHT (GLint)600

#define TRI_COUNT 100

bool left_button = 0;
bool right_button = 0;

typedef struct COLORVALUE {
	float r;
	float g;
	float b;
};

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} INT_RECT;

typedef struct {
	int x, y;
} Vector;

typedef long long int ll;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
void Motion(int x, int y);
//void TimerFunction(int value);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();
bool my_pt_in_Rect(INT_RECT rt, int x, int y);


GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[2];

GLfloat rColor = 1.0f;
GLfloat gColor = 1.0f;
GLfloat bColor = 1.0f;

int move_edge = -1;



GLfloat pos[TRI_COUNT * 3][3] =
{
	{-0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, 
	{0.5, -0.5, 0.0}, {0.5, 0.5, 0.0},

	{0.0, -1.0, 0.0}, {0.0, 1.0, 0.0},
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f }, {0.3f, 0.6f, 0.8f },
};

ll crossProduct(Vector a, Vector b) {
	return (((ll)a.x * b.y) - ((ll)a.y * b.x));
}

int direction(Vector a, Vector b, Vector c) {
	Vector ca, cb;
	ll value;
	ca.x = a.x - c.x, ca.y = a.y - c.y;
	cb.x = b.x - c.x, cb.y = b.y - c.y;
	value = crossProduct(ca, cb);
	return (value > 0) ? 1 : (value < 0? -1 : 0);
}

bool onSegment(Vector a, Vector b, Vector c) {
	if (c.x >= min(a.x, b.x) && c.x <= max(a.x, b.x) && 
		c.y >= min(a.y, b.y) && c.y <= max(a.y, b.y)) return true;
	return false;
}

bool segmentIntersect(Vector* p) {
	int d1, d2, d3, d4;
	d1 = direction(p[3], p[4], p[1]);
	d2 = direction(p[3], p[4], p[2]);
	d3 = direction(p[1], p[2], p[3]);
	d4 = direction(p[1], p[2], p[4]);
	if (d1 * d2 < 0 && d3 * d4 < 0) return true;
	else if (d1 == 0 && onSegment(p[3], p[4], p[1])) return true;
	else if (d2 == 0 && onSegment(p[3], p[4], p[2])) return true;
	else if (d3 == 0 && onSegment(p[1], p[2], p[3])) return true;
	else if (d4 == 0 && onSegment(p[1], p[2], p[4])) return true;
	return false;
}

Vector point[4][5];
INT_RECT rec_point[4] = { 0, };

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
	//glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	// 위 직선
	point[0][1].x = 200, point[0][1].y = 150;
	point[0][2].x = 600, point[0][2].y = 150;

	// 아래 직선
	point[1][1].x = 200, point[1][1].y = 450;
	point[1][2].x = 600, point[1][2].y = 450;

	// 왼쪽 직선
	point[2][1].x = 200, point[2][1].y = 150;
	point[2][2].x = 200, point[2][2].y = 450;

	// 오른쪽 직선
	point[3][1].x = 600, point[3][1].y = 150;
	point[3][2].x = 600, point[3][2].y = 450;

	for (int i = 0; i < 4; i++) {
		con_Ogl_to_D(&rec_point[i].left, &rec_point[i].top, pos[i][0], pos[i][1]);
		rec_point[i].right = rec_point[i].left + 20;
		rec_point[i].left -= 20;
		rec_point[i].bottom = rec_point[i].top + 20;
		rec_point[i].top -= 20;
	}

	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glBindVertexArray(vao);

	glPointSize(2.0);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glDrawArrays(GL_LINES, 4, 2);
	glDrawArrays(GL_LINES, 6, 2);

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

		for (int i = 0; i < 4; i++) {
			if (my_pt_in_Rect(rec_point[i], x, y)) {
				move_edge = i;
				break;
			}
		}
		
		if (move_edge == -1) {
			// 다각형 안쪽인지 검사.
			int intersect_count = 0;

			// 마우스 좌표 기준, 위랑 아래로 뻗는 직선 두개 만들기.
			for (int i = 0; i < 4; i++) {
				// 위로 뻗는 직선
				point[i][3].x = x, point[i][3].y = y;
				point[i][4].x = x, point[i][4].y = 0;

				if (segmentIntersect(point[i])) {
					intersect_count++;
				}
			}

			if (intersect_count == 0) {
				// 교차하는게 0이면, 다른 쪽 (아래쪽)도 검사

				for (int i = 0; i < 4; i++) {
					// 아래로 뻗는 직선
					point[i][3].x = x, point[i][3].y = y;
					point[i][4].x = x, point[i][4].y = 600;

					if (segmentIntersect(point[i])) {
						intersect_count++;
					}
				}

				if (intersect_count % 2 == 1) {
					// 홀수일때,
					// 사각형 내부
					left_button = true;
				}
			}
			else if (intersect_count % 2 == 0) {
				// 짝수일때, 
				// 사각형 외부
				;
			}

			else if (intersect_count % 2 == 1) {
				// 홀수일때,
				// 사각형 내부
				left_button = true;
			}
		}	
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		move_edge = -1;
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

	if (move_edge >= 0) {
		int afterX = x - beforeX; // x 변화값
		int afterY = y - beforeY; // y 변화값
		float f_afterX, f_afterY;

		switch (move_edge) {
		case 0:
			point[0][1].x += afterX;
			point[0][1].y += afterY;
			point[2][1].x += afterX;
			point[2][1].y += afterY;

			rec_point[0].left += afterX;
			rec_point[0].top += afterY;
			rec_point[0].right += afterX;
			rec_point[0].bottom += afterY;
			break;
		case 1:
			point[2][2].x += afterX;
			point[2][2].y += afterY;
			point[1][1].x += afterX;
			point[1][1].y += afterY;
			rec_point[1].left += afterX;
			rec_point[1].top += afterY;
			rec_point[1].right += afterX;
			rec_point[1].bottom += afterY;
			break;
		case 2:
			point[1][2].x += afterX;
			point[1][2].y += afterY;
			point[3][2].x += afterX;
			point[3][2].y += afterY;
			rec_point[2].left += afterX;
			rec_point[2].top += afterY;
			rec_point[2].right += afterX;
			rec_point[2].bottom += afterY;
			break;
		case 3:
			point[0][2].x += afterX;
			point[0][2].y += afterY;
			point[3][1].x += afterX;
			point[3][1].y += afterY;
			rec_point[3].left += afterX;
			rec_point[3].top += afterY;
			rec_point[3].right += afterX;
			rec_point[3].bottom += afterY;
			break;
		}



		con_D_to_Ogl(point[0][1].x, point[0][1].y, &pos[0][0], &pos[0][1]);
		con_D_to_Ogl(point[0][2].x, point[0][2].y, &pos[3][0], &pos[3][1]);

		con_D_to_Ogl(point[1][1].x, point[1][1].y, &pos[1][0], &pos[1][1]);
		con_D_to_Ogl(point[1][2].x, point[1][2].y, &pos[2][0], &pos[2][1]);

		beforeX = x, beforeY = y;
	}

	else if (left_button == true) {

		int afterX = x - beforeX; // x 변화값
		int afterY = y - beforeY; // y 변화값
		float f_afterX, f_afterY;

		// point vector에 대입. (점 원소)
		for (int i = 0; i < 4; i++) {
			point[i][1].x += afterX;
			point[i][1].y += afterY;

			point[i][2].x += afterX;
			point[i][2].y += afterY;

			rec_point[i].left += afterX;
			rec_point[i].top += afterY;
			rec_point[i].right += afterX;
			rec_point[i].bottom += afterY;
		}



		con_D_to_Ogl(point[0][1].x, point[0][1].y, &pos[0][0], &pos[0][1]);
		con_D_to_Ogl(point[0][2].x, point[0][2].y, &pos[3][0], &pos[3][1]);

		con_D_to_Ogl(point[1][1].x, point[1][1].y, &pos[1][0], &pos[1][1]);
		con_D_to_Ogl(point[1][2].x, point[1][2].y, &pos[2][0], &pos[2][1]);

		beforeX = x, beforeY = y;
	}

	// 오른쪽 마우스가 눌렸을 때
	if (right_button == true) {
		;
	}
	else {
		;
	}

	InitBuffer();
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
/*
void TimerFunction(int value) {

	if (left_button) {
		;
	}

	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}*/

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}


bool my_pt_in_Rect(INT_RECT rt, int x, int y) {
	if (rt.bottom >= y && rt.top <= y && rt.left <= x && rt.right >= x)
		return true;
	return false;
}







#endif




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
