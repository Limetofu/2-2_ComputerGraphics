#include "stdafx.h"
#include "objRead.h"

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define DRILL 18

#if DRILL == 16

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)800


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
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
void InitBuffer();
void InitShader();
void InitTexture();

void draw_cube(glm::mat4 TR, unsigned int modelLocation);
void draw_pyramid(glm::mat4 TR, unsigned int modelLocation);
void DrawCircle(glm::mat4 TR, unsigned int modelLocation);

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo;

unsigned int texture[6];

void SetCamera();
void SetProjection();

GLfloat cameraPosZ = 5.0f;
GLfloat cameraPosX = -1.5f;
GLfloat cameraPosY = 1.5f;

// 은면 제거 설정
int enable_depth = true;

// y축 자전
GLfloat y_rotate_degree = 0.0f;
int y_rotate_anime = false;

GLfloat x_rotate_degree = 0.0f;
int x_rotate_anime = false;

// 다른 도형 출력
int show_cube = true;

// 육면체 윗면 애니메이션
GLfloat upside_degree = 0.0f;
int upside_anime = false;


// 육면체 앞면 애니메이션
GLfloat front_degree = 0.0f;
int front_anime_up = false;
int front_anime_down = false;

// 육면체 옆면 애니메이션
GLfloat side_move_value = 0.0f;
int side_anime_up = false;
int side_anime_down = false;

// 사각뿔 애니메이션
GLfloat pyramid_degree = 0.0f;
int pyramid_anime_up = false;
int pyramid_anime_down = false;

// 직각, 원근 투영
int perspective_view = true;

GLfloat light_radian = 2.0f;
GLfloat lightX = 0.0f, lightY = 0.0f, lightZ = 2.0f;
int light_rotate = false;
GLfloat light_degree = 0.0f;

int light_on = true;

float pos[3000] =
{
	// x, y, z축
	//-1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0,  0.0, 1.0, 0.0, 0.0, 0.0, -1.0,  0.0, 0.0, 1.0,

	// 아래 0 -> 0.0, -1.0, 0.0,
	 -0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 1.0,	  -0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 0.0, 0.0,	  0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 1.0, 0.0,
	 0.5, -0.5, 0.5,   0.0, -1.0, 0.0, 1.0, 0.0, 	  0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 1.0, 1.0,     -0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 1.0,

	// 좌측 12
	 -0.5, 0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0, 	 -0.5, 0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 1.0,    	-0.5, -0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 0.0,
	 -0.5, 0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0,	     -0.5, -0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 0.0, 	-0.5, -0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 0.0,

	// right 18
	 0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,	 0.5, -0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 0.0,		0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 1.0,
	 0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,	 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 1.0,		0.5, 0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 1.0,

	// behind 24
	 -0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,	 -0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 0.0,	0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 0.0,
	 -0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,	 0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 0.0,	   0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 1.0,

	// front 30
	 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 		 -0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0,	  0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0,
	 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0,		0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0,		0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 1.0,

	// top 36
	 -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 	  -0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 		0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0,
	 0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, 		0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 1.0,	-0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,

	 // 사각뿔,
	 // front
	 -0.5, -0.5, 0.5, 0.0, 0.5, 1.0, 0.0, 0.0,      0.5, -0.5, 0.5, 0.0, 0.5, 1.0, 1.0, 0.0,       0.0, 0.5, 0.0, 0.0, 0.5, 1.0, 0.5, 1.0,
	 // left
	 -0.5, -0.5, 0.5, -1.0, 0.5, 0.0, 1.0, 0.0,    -0.5, -0.5, -0.5, -1.0, 0.5, 0.0, 0.0, 0.0,    0.0, 0.5, 0.0, -1.0, 0.5, 0.0, 0.5, 1.0,
	 // right
	 0.5, -0.5, 0.5, 1.0, 0.5, 0.0, 0.0, 0.0,       0.5, -0.5, -0.5, 1.0, 0.5, 0.0, 1.0, 0.0,       0.0, 0.5, 0.0, 1.0, 0.5, 0.0, 0.5, 1.0,
	 // back
	 -0.5, -0.5, -0.5, 0.0, 0.5, -1.0, 0.0, 0.0,   0.5, -0.5, -0.5, 0.0, 0.5, -1.0, 1.0, 0.0,    0.0, 0.5, 0.0, 0.0, 0.5, -1.0, 0.5, 1.0,
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

	InitTexture();

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

void InitLight() {
	lightX = cos(glm::radians(light_degree + 90.0f)) * light_radian;
	lightZ = sin(glm::radians(light_degree + 90.0f)) * light_radian;

	glUseProgram(s_program);
	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, lightX, lightY, lightZ);
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	if (light_on) glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	else glUniform3f(lightColorLocation, 0.05, 0.05, 0.05);
	//unsigned int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	//glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, cameraPosX, cameraPosY, cameraPosZ);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	if (enable_depth) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);



	InitLight();

	//unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	//glUniform3f(viewPosLocation, cameraPosX, 0.0, cameraPosZ);

	glm::mat4 TR = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");

	/* ------------------------------ 직각 / 원근 투영 ------------------------------ */
	SetProjection();
	SetCamera();
	/* ---------------------------------------------------------------------------- */

	
	//if (perspective_view) {
	//	TR = glm::translate(TR, glm::vec3(0.0, 0.0, -3.0f));
	//}

	
	glBindVertexArray(vao);
	
	unsigned int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	InitLight();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	/* ----------------------------------- y축 자전 ----------------------------------- */
	TR = glm::rotate(TR, glm::radians(y_rotate_degree), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(x_rotate_degree), glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	// 아래쪽 바닥은 공유
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	if (show_cube) {
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 18, 3);
		glDrawArrays(GL_TRIANGLES, 21, 3);
	}
	/* ---------------------------------------------------------------------------- */

	if (show_cube) {
		draw_cube(TR, modelLocation);
	}

	else {
		draw_pyramid(TR, modelLocation);
	}

	glBindVertexArray(vao);


	glutSwapBuffers(); // 화면에 출력하기
}

void draw_cube(glm::mat4 TR, unsigned int modelLocation) {
	/* ------------------------------ 윗면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawArrays(GL_TRIANGLES, 30, 3);
	glDrawArrays(GL_TRIANGLES, 33, 3);
	/* ---------------------------------------------------------------------------- */

	/* ------------------------------ 옆면 애니메이션, 뒤/아래 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glDrawArrays(GL_TRIANGLES, 12, 3);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	/* ---------------------------------------------------------------------------- */

	/* ------------------------------ 앞면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glDrawArrays(GL_TRIANGLES, 30 - 6, 3);
	glDrawArrays(GL_TRIANGLES, 33 - 6, 3);
	/* ---------------------------------------------------------------------------- */
}

void draw_pyramid(glm::mat4 TR, unsigned int modelLocation) {
	/* ------------------------------ 사각뿔 앞면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawArrays(GL_TRIANGLES, 42 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDrawArrays(GL_TRIANGLES, 45 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glDrawArrays(GL_TRIANGLES, 48 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glDrawArrays(GL_TRIANGLES, 51 - 6, 3);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void InitTexture() {
	BITMAPINFO* bmp;
	unsigned char* data;

	glGenTextures(6, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("a.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[1]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("b.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[2]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("c.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[3]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("d.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[4]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("e.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[5]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("f.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
}

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * (*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}
	fclose(fp);
	return bits;
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

	//if (upside_anime) {
	//	upside_degree += 0.8f;
	//}
	
	
	if (y_rotate_anime) {
		y_rotate_degree += 0.5f;
	}
	if (x_rotate_anime) {
		x_rotate_degree += 0.5f;
	}



	//if (side_anime_up) {
	//	if (side_move_value < 1.0f)
	//		side_move_value += 0.003f;
	//}
	//if (side_anime_down) {
	//	if (side_move_value > 0.0f)
	//		side_move_value -= 0.003f;
	//}

	//if (front_anime_down) {
	//	if (front_degree < 90.0f)
	//		front_degree += 0.5f;
	//}
	//if (front_anime_up) {
	//	if (front_degree > 0.0f)
	//		front_degree -= 0.5f;
	//}

	//if (pyramid_anime_down) {
	//	if (pyramid_degree < 233.0f)
	//		pyramid_degree += 0.5f;
	//}
	//if (pyramid_anime_up) {
	//	if (pyramid_degree > 0.0f) {
	//		pyramid_degree -= 0.5f;
	//	}
	//}

	// lightZ = 2.0f
	if (light_rotate) {
		lightX = cos(glm::radians(light_degree + 90.0f)) * light_radian;
		lightZ = sin(glm::radians(light_degree + 90.0f)) * light_radian;

		light_degree += 1.0f;
		if (light_degree >= 360.0f) light_degree = 0.0f;
	}



	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}


GLvoid Keyboard(unsigned char key, int x, int y) {
	
	switch(key) {

	case 'c': case 'C': // 육면체
		show_cube = true;
		break;

	case 'p': case 'P': // 사각뿔
		show_cube = false;
		break;

	case 'y': case 'Y': // y 축 기준 자전, 멈추기
		if (y_rotate_anime) y_rotate_anime = false;
		else y_rotate_anime = true;
		break;

	case 'x': case 'X': // x축 회전
		if (x_rotate_anime) x_rotate_anime = false;
		else x_rotate_anime = true;
		break;

	case 's': case 'S': // 초기화
		show_cube = true;
		y_rotate_anime = false, y_rotate_degree = 0.0f;
		x_rotate_anime = false, x_rotate_degree = 0.0f;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void SetCamera() {
	glm::mat4 VR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, cameraPosY, cameraPosZ); //--- 카메라 위치
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
}

void SetProjection() {
	glm::mat4 PR = glm::mat4(1.0f);
	unsigned int projLocation = glGetUniformLocation(s_program, "projectionTransform");

	/* ------------------------------ 원근 투영 ------------------------------ */
	if (perspective_view)
		PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	else
		PR = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -2.0f, 100.0f);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	/* ---------------------------------------------------------------------------- */
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


#elif DRILL == 17

#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)800


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
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
void InitBuffer();
void InitShader();
void InitTexture();

void draw_cube(glm::mat4 TR, unsigned int modelLocation);
void draw_pyramid(glm::mat4 TR, unsigned int modelLocation);
void DrawCircle(glm::mat4 TR, unsigned int modelLocation);

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo;

unsigned int texture[7];

void SetCamera();
void SetProjection();

GLfloat cameraPosZ = 5.0f;
GLfloat cameraPosX = -1.5f;
GLfloat cameraPosY = 1.5f;

// 은면 제거 설정
int enable_depth = true;

// y축 자전
GLfloat y_rotate_degree = 0.0f;
int y_rotate_anime = false;

GLfloat x_rotate_degree = 0.0f;
int x_rotate_anime = false;

// 다른 도형 출력
int show_cube = true;

// 육면체 윗면 애니메이션
GLfloat upside_degree = 0.0f;
int upside_anime = false;


// 육면체 앞면 애니메이션
GLfloat front_degree = 0.0f;
int front_anime_up = false;
int front_anime_down = false;

// 육면체 옆면 애니메이션
GLfloat side_move_value = 0.0f;
int side_anime_up = false;
int side_anime_down = false;

// 사각뿔 애니메이션
GLfloat pyramid_degree = 0.0f;
int pyramid_anime_up = false;
int pyramid_anime_down = false;

// 직각, 원근 투영
int perspective_view = true;

GLfloat light_radian = 2.0f;
GLfloat lightX = 0.0f, lightY = 0.0f, lightZ = 2.0f;
int light_rotate = false;
GLfloat light_degree = 0.0f;

int light_on = true;

float pos[3000] =
{
	// x, y, z축
	//-1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0,  0.0, 1.0, 0.0, 0.0, 0.0, -1.0,  0.0, 0.0, 1.0,

	// 아래 0 -> 0.0, -1.0, 0.0,
	 -0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 1.0,	  -0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 0.0, 0.0,	  0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 1.0, 0.0,
	 0.5, -0.5, 0.5,   0.0, -1.0, 0.0, 1.0, 0.0, 	  0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 1.0, 1.0,     -0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 1.0,

	 // 좌측 12
	  -0.5, 0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0, 	 -0.5, 0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 1.0,    	-0.5, -0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 0.0,
	  -0.5, 0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0,	     -0.5, -0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 0.0, 	-0.5, -0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 0.0,

	  // right 18
	   0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,	 0.5, -0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 0.0,		0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 1.0,
	   0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,	 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 1.0,		0.5, 0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 1.0,

	   // behind 24
		-0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,	 -0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 0.0,	0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 0.0,
		-0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,	 0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 0.0,	   0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 1.0,

		// front 30
		 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 		 -0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0,	  0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0,
		 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0,		0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0,		0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 1.0,

		 // top 36
		  -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 	  -0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 		0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0,
		  0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, 		0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 1.0,	-0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,

		  // 사각뿔,
		  // front
		  -0.5, -0.5, 0.5, 0.0, 0.5, 1.0, 0.0, 0.0,      0.5, -0.5, 0.5, 0.0, 0.5, 1.0, 1.0, 0.0,       0.0, 0.5, 0.0, 0.0, 0.5, 1.0, 0.5, 1.0,
		  // left
		  -0.5, -0.5, 0.5, -1.0, 0.5, 0.0, 1.0, 0.0,    -0.5, -0.5, -0.5, -1.0, 0.5, 0.0, 0.0, 0.0,    0.0, 0.5, 0.0, -1.0, 0.5, 0.0, 0.5, 1.0,
		  // right
		  0.5, -0.5, 0.5, 1.0, 0.5, 0.0, 0.0, 0.0,       0.5, -0.5, -0.5, 1.0, 0.5, 0.0, 1.0, 0.0,       0.0, 0.5, 0.0, 1.0, 0.5, 0.0, 0.5, 1.0,
		  // back
		  -0.5, -0.5, -0.5, 0.0, 0.5, -1.0, 0.0, 0.0,   0.5, -0.5, -0.5, 0.0, 0.5, -1.0, 1.0, 0.0,    0.0, 0.5, 0.0, 0.0, 0.5, -1.0, 0.5, 1.0,
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

	InitTexture();

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

void InitLight() {
	lightX = cos(glm::radians(light_degree + 90.0f)) * light_radian;
	lightZ = sin(glm::radians(light_degree + 90.0f)) * light_radian;

	glUseProgram(s_program);
	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, lightX, lightY, lightZ);
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	if (light_on) glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	else glUniform3f(lightColorLocation, 0.05, 0.05, 0.05);
	//unsigned int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	//glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, cameraPosX, cameraPosY, cameraPosZ);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	if (enable_depth) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);



	InitLight();
	glBindVertexArray(vao);
	//unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	//glUniform3f(viewPosLocation, cameraPosX, 0.0, cameraPosZ);

	glm::mat4 TR = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");

	glUniformMatrix4fv(glGetUniformLocation(s_program, "viewTransform"), 1, GL_FALSE, glm::value_ptr(TR));
	glUniformMatrix4fv(glGetUniformLocation(s_program, "projectionTransform"), 1, GL_FALSE, glm::value_ptr(TR));

	glBindTexture(GL_TEXTURE_2D, texture[6]);

	TR = glm::scale(TR, glm::vec3(2.0f, 2.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glDrawArrays(GL_TRIANGLES, 30, 6);


	/* ------------------------------ 직각 / 원근 투영 ------------------------------ */
	SetProjection();
	SetCamera();
	/* ---------------------------------------------------------------------------- */


	//if (perspective_view) {
	//	TR = glm::translsate(TR, glm::vec3(0.0, 0.0, -3.0f));
	//}

	TR = glm::mat4(1.0f);
	

	unsigned int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	InitLight();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	/* ----------------------------------- y축 자전 ----------------------------------- */
	TR = glm::rotate(TR, glm::radians(y_rotate_degree), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(x_rotate_degree), glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	// 아래쪽 바닥은 공유
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	if (show_cube) {
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 18, 3);
		glDrawArrays(GL_TRIANGLES, 21, 3);
	}
	/* ---------------------------------------------------------------------------- */

	if (show_cube) {
		draw_cube(TR, modelLocation);
	}

	else {
		draw_pyramid(TR, modelLocation);
	}

	glBindVertexArray(vao);


	glutSwapBuffers(); // 화면에 출력하기
}

void draw_cube(glm::mat4 TR, unsigned int modelLocation) {
	/* ------------------------------ 윗면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawArrays(GL_TRIANGLES, 30, 3);
	glDrawArrays(GL_TRIANGLES, 33, 3);
	/* ---------------------------------------------------------------------------- */

	/* ------------------------------ 옆면 애니메이션, 뒤/아래 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glDrawArrays(GL_TRIANGLES, 12, 3);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	/* ---------------------------------------------------------------------------- */

	/* ------------------------------ 앞면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glDrawArrays(GL_TRIANGLES, 30 - 6, 3);
	glDrawArrays(GL_TRIANGLES, 33 - 6, 3);
	/* ---------------------------------------------------------------------------- */
}

void draw_pyramid(glm::mat4 TR, unsigned int modelLocation) {
	/* ------------------------------ 사각뿔 앞면 애니메이션 ------------------------------ */
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawArrays(GL_TRIANGLES, 42 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDrawArrays(GL_TRIANGLES, 45 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glDrawArrays(GL_TRIANGLES, 48 - 6, 3);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glDrawArrays(GL_TRIANGLES, 51 - 6, 3);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void InitTexture() {
	BITMAPINFO* bmp;
	unsigned char* data;

	glGenTextures(7, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("a.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[1]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("b.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[2]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("c.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[3]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("d.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[4]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("e.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[5]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("f.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 258, 258, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glBindTexture(GL_TEXTURE_2D, texture[6]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = LoadDIBitmap("background.bmp", &bmp); //--- 텍스처로 사용할 비트맵 이미지 로드하기
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1000, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
}

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * (*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}
	fclose(fp);
	return bits;
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

	//if (upside_anime) {
	//	upside_degree += 0.8f;
	//}


	if (y_rotate_anime) {
		y_rotate_degree += 0.5f;
	}
	if (x_rotate_anime) {
		x_rotate_degree += 0.5f;
	}



	//if (side_anime_up) {
	//	if (side_move_value < 1.0f)
	//		side_move_value += 0.003f;
	//}
	//if (side_anime_down) {
	//	if (side_move_value > 0.0f)
	//		side_move_value -= 0.003f;
	//}

	//if (front_anime_down) {
	//	if (front_degree < 90.0f)
	//		front_degree += 0.5f;
	//}
	//if (front_anime_up) {
	//	if (front_degree > 0.0f)
	//		front_degree -= 0.5f;
	//}

	//if (pyramid_anime_down) {
	//	if (pyramid_degree < 233.0f)
	//		pyramid_degree += 0.5f;
	//}
	//if (pyramid_anime_up) {
	//	if (pyramid_degree > 0.0f) {
	//		pyramid_degree -= 0.5f;
	//	}
	//}

	// lightZ = 2.0f
	if (light_rotate) {
		lightX = cos(glm::radians(light_degree + 90.0f)) * light_radian;
		lightZ = sin(glm::radians(light_degree + 90.0f)) * light_radian;

		light_degree += 1.0f;
		if (light_degree >= 360.0f) light_degree = 0.0f;
	}



	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}


GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {

	case 'c': case 'C': // 육면체
		show_cube = true;
		break;

	case 'p': case 'P': // 사각뿔
		show_cube = false;
		break;

	case 'y': case 'Y': // y 축 기준 자전, 멈추기
		if (y_rotate_anime) y_rotate_anime = false;
		else y_rotate_anime = true;
		break;

	case 'x': case 'X': // x축 회전
		if (x_rotate_anime) x_rotate_anime = false;
		else x_rotate_anime = true;
		break;

	case 's': case 'S': // 초기화
		show_cube = true;
		y_rotate_anime = false, y_rotate_degree = 0.0f;
		x_rotate_anime = false, x_rotate_degree = 0.0f;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void SetCamera() {
	glm::mat4 VR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, cameraPosY, cameraPosZ); //--- 카메라 위치
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
}

void SetProjection() {
	glm::mat4 PR = glm::mat4(1.0f);
	unsigned int projLocation = glGetUniformLocation(s_program, "projectionTransform");

	/* ------------------------------ 원근 투영 ------------------------------ */
	if (perspective_view)
		PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	else
		PR = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -2.0f, 100.0f);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	/* ---------------------------------------------------------------------------- */
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







#elif DRILL == 18


#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)1000

#define TRI_COUNT 500

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
GLvoid KeyboardUp(unsigned char key, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void DrawLeg(glm::mat4 TR, unsigned int modelLocation, int legnum_1, int legnum_2);
void DrawLeg_reverse(glm::mat4 TR, unsigned int modelLocation, int legnum_1, int legnum_2);


void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

void DrawShapeOri();

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[2];

// 은면 제거 설정
int enable_depth = true;

GLfloat DEGREES = 0.0f;
GLfloat cameraPosZ = 2.0f;
GLfloat cameraPosX = 0.0f;

GLfloat cameraY = 0.0f;

int animeFrontOpen = false;
GLfloat degreeFront = 89.0f;

int RobotMoveDir = 0;
GLfloat moveRobotX = 0.0f;
GLfloat moveRobotY = -1.0f;
GLfloat moveRobotZ = -1.0f;

int armRotateAnime = false;
GLfloat armDegree = 0.0f;

int RobotFaceDir = 1;

int RobotJumpAnime = false;

GLfloat obstacleX = 0.0f;
GLfloat obstacleZ = 0.0f;

int RobotOnObstacle = false;

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x, y축
	{-3.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, {0.0, 3.0, 0.0},

	// z축
	{0.0, 0.0, -3.0}, {0.0, 0.0, 3.0},

	// 아래 6
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 12
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 18
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 24
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 30
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 36
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},



	///// body
	// 아래 42
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// 좌측 48
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	// right 54
	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	// behind 60
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	// front 66
	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	// top 72
	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},


	///// left arm, leg
	// 78 ~
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},


	///// right arm, leg
	// 114 ~ 
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
	{-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5},

	{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},

	{-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},

	{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5},

	{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},

	///// obstacle
	// 150 ~
	{ -0.5, -0.5, -0.5 }, { -0.5, -0.5, 0.5 }, { 0.5, -0.5, 0.5 },
	{ 0.5, -0.5, 0.5 }, { 0.5, -0.5, -0.5 }, { -0.5, -0.5, -0.5 },

	{ -0.5, 0.5, -0.5 }, { -0.5, 0.5, 0.5 }, { -0.5, -0.5, 0.5 },
	{ -0.5, 0.5, -0.5 }, { -0.5, -0.5, 0.5 }, { -0.5, -0.5, -0.5 },

	{ 0.5, -0.5, -0.5 }, { 0.5, -0.5, 0.5 }, { 0.5, 0.5, 0.5 },
	{ 0.5, -0.5, -0.5 }, { 0.5, 0.5, 0.5 }, { 0.5, 0.5, -0.5 },

	{ -0.5, 0.5, -0.5 }, { -0.5, -0.5, -0.5 }, { 0.5, -0.5, -0.5 },
	{ -0.5, 0.5, -0.5 }, { 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5 },

	{ -0.5, 0.5, 0.5 }, { -0.5, -0.5, 0.5 }, { 0.5, -0.5, 0.5 },
	{ -0.5, 0.5, 0.5 }, { 0.5, -0.5, 0.5 }, { 0.5, 0.5, 0.5 },

	{ -0.5, 0.5, -0.5 }, { -0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },
	{ 0.5, 0.5, 0.5 }, { 0.5, 0.5, -0.5 }, { -0.5, 0.5, -0.5 },
};

GLfloat col[TRI_COUNT * 3][3] =
{
	// line -> triangle
	{1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f },
	{1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f },

	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, // down
	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f},

	{0.5f, 0.0f, 1.0f}, {0.5f, 0.0f, 1.0f}, {0.5f, 0.0f, 1.0f}, // left
	{0.5f, 0.0f, 1.0f}, {0.5f, 0.0f, 1.0f}, {0.5f, 0.0f, 1.0f},

	{0.5f, 0.3f, 0.0f}, {0.5f, 0.3f, 0.0f}, {0.5f, 0.3f, 0.0f}, // right
	{0.5f, 0.3f, 0.0f}, {0.5f, 0.3f, 0.0f}, {0.5f, 0.3f, 0.0f},

	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, // behind
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},

	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // front
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},

	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // top
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},


	// body
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // down
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},

	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // left
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},

	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // right
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},

	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // behind
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},

	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // front
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},

	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, // top
	{0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f}, {0.3f, 0.8f, 0.9f},


	// left arm, leg
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // down
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},

	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // left
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},

	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // right
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},

	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // behind
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},

	{0.8f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f}, // front
	{0.8f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f},

	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // top
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},


	// right arm, leg
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // down
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // left
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // right
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // behind
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // front
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, // top
	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},

	// obstacle
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},

	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},

	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},

	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},

	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},

	{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
	{ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f },
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
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	int Xsign, Zsign;

	if (rand() % 2 == 0) Xsign = 1;
	else Xsign = -1;

	if (rand() % 2 == 0) Zsign = 1;
	else Zsign = -1;

	obstacleX = Xsign * ((float)rand() / (RAND_MAX));
	obstacleZ = Zsign * ((float)rand() / (RAND_MAX));

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	if (enable_depth) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);


	DrawShapeOri();


	glBindVertexArray(vao);

	glutSwapBuffers(); // 화면에 출력하기
}


void DrawShapeOri() {


	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 VR = glm::mat4(1.0f);
	glm::mat4 PR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, 0.0f, cameraPosZ); //--- 카메라 위치
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	unsigned int projLocation = glGetUniformLocation(s_program, "projectionTransform");


	/* ------------------------------ 직각 / 원근 투영 ------------------------------ */
	PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	TR = glm::translate(TR, glm::vec3(0.0, 0.0, -5.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ y축 대하여 회전 ------------------------------ */
	TR = glm::translate(TR, glm::vec3(-cameraPosX * 2, 0.0f, 0.0));
	TR = glm::rotate(TR, glm::radians(cameraY), glm::vec3(0.0f, 1.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(cameraPosX * 2, 0.0f, 0.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ 기본 회전, 선 ------------------------------ */
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	// glDrawArrays(GL_LINES, 0, 2); glDrawArrays(GL_LINES, 2, 2); glDrawArrays(GL_LINES, 4, 2);
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ 바닥 ------------------------------ */
	TR = glm::scale(TR, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	for (int i = 6; i < 39 + 1; i += 3)
		if (i != 30 && i != 33) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::translate(TR, glm::vec3(0.0f, -0.5f, 0.5f));
	TR = glm::rotate(TR, glm::radians(degreeFront), glm::vec3(1.0f, 0.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.5f, -0.5f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glDrawArrays(GL_TRIANGLES, 30, 3);
	glDrawArrays(GL_TRIANGLES, 33, 3);

	TR = glm::translate(TR, glm::vec3(0.0f, -0.5f, 0.5f));
	TR = glm::rotate(TR, glm::radians(-degreeFront), glm::vec3(1.0f, 0.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.5f, -0.5f));
	TR = glm::scale(TR, glm::vec3(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f));
	/* ---------------------------------------------------------------------------- */




	/* ------------------------------ 장애물 ------------------------------ */
	TR = glm::translate(TR, glm::vec3(obstacleX, -1.5f, obstacleZ));
	TR = glm::scale(TR, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 150; i < 150 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::scale(TR, glm::vec3(1.0f / 0.5f, 1.0f / 0.5f, 1.0f / 0.5f));
	TR = glm::translate(TR, glm::vec3(-obstacleX, 1.5f, -obstacleZ));
	/* ---------------------------------------------------------------------------- */


	/* ----------------------------------- 몸통 육면체 ----------------------------------- */
	TR = glm::translate(TR, glm::vec3(moveRobotX, moveRobotY, moveRobotZ));
	TR = glm::scale(TR, glm::vec3(0.3f, 0.4f, 0.3f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 42; i < 42 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::scale(TR, glm::vec3(1.0f / 0.3f, 1.0f / 0.4f, 1.0f / 0.3f));
	/* ---------------------------------------------------------------------------- */



	// arm scale = 0.1, 0.4, 0.1
	if (RobotFaceDir == 1) {
		/* ----------------------------------- 왼쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(-3.5f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 78; i < 78 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(3.5f, 0.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */

		/* ----------------------------------- 오른쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(3.5f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 114; i < 114 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(-3.5f, 0.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */
	}
	else if (RobotFaceDir == -1) {
		/* ----------------------------------- 왼쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(-3.5f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 114; i < 114 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(3.5f, 0.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */


		/* ----------------------------------- 오른쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(3.5f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 78; i < 78 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(-3.5f, 0.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */
	}

	else if (RobotFaceDir == -2) {
		/* ----------------------------------- 뒤쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -3.5f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 78; i < 78 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 3.5f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */

		/* ----------------------------------- 앞쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 3.5f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 114; i < 114 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -3.5f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */
	}
	else {
		/* ----------------------------------- 뒤쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -3.5f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 114; i < 114 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 3.5f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */

		/* ----------------------------------- 앞쪽 팔 ----------------------------------- */
		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));

		TR = glm::scale(TR, glm::vec3(0.05f, 0.4f, 0.05f));
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 3.5f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 78; i < 78 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -3.5f));
		TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.4f, 1.0f / 0.05f));

		TR = glm::translate(TR, glm::vec3(0.0f, 0.2f, 0.0f));
		TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(0.0f, -0.2f, 0.0f));
		/* ---------------------------------------------------------------------------- */
	}


	if (RobotFaceDir == 1) {
		DrawLeg(TR, modelLocation, 78, 114);
	}
	else if (RobotFaceDir == -1) {
		DrawLeg(TR, modelLocation, 114, 78);
	}
	else if (RobotFaceDir == -2) {
		DrawLeg_reverse(TR, modelLocation, 78, 114);
	}
	else {
		DrawLeg_reverse(TR, modelLocation, 114, 78);
	}




	/* ----------------------------------- 머리 ----------------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, 0.3f, 0.0f));
	TR = glm::scale(TR, glm::vec3(0.23f, 0.23f, 0.23f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 6; i < 39 + 1; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);
	/* ---------------------------------------------------------------------------- */

	/* ----------------------------------- 코 ----------------------------------- */
	switch (RobotFaceDir) {
	case 1: // front
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 0.5f));
		break;
	case -1: // back
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -0.5f));
		break;
	case 2: // right
		TR = glm::translate(TR, glm::vec3(0.5f, 0.0f, 0.0f));
		break;
	case -2: // left
		TR = glm::translate(TR, glm::vec3(-0.5f, 0.0f, 0.0f));
		break;
	}

	TR = glm::scale(TR, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	for (int i = 6; i < 39 + 1; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);
	/* ---------------------------------------------------------------------------- */
}

void DrawLeg(glm::mat4 TR, unsigned int modelLocation, int legnum_1, int legnum_2) {
	/* --------------------------------- 왼쪽 다리 --------------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));

	TR = glm::scale(TR, glm::vec3(0.05f, 0.3f, 0.05f));
	TR = glm::translate(TR, glm::vec3(-1.5f, -1.2f, 0.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = legnum_1; i < legnum_1 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::translate(TR, glm::vec3(1.5f, 1.2f, 0.0f));
	TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.3f, 1.0f / 0.05f));

	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));
	/* ---------------------------------------------------------------------------- */

	/* ---------------------------- 오른쪽 다리 / 앞뒤 ---------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));

	TR = glm::scale(TR, glm::vec3(0.05f, 0.3f, 0.05f));
	TR = glm::translate(TR, glm::vec3(1.5f, -1.2f, 0.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = legnum_2; i < legnum_2 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::translate(TR, glm::vec3(-1.5f, 1.2f, 0.0f));
	TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.3f, 1.0f / 0.05f));

	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));
	/* ---------------------------------------------------------------------------- */
}

void DrawLeg_reverse(glm::mat4 TR, unsigned int modelLocation, int legnum_1, int legnum_2) {


	/* --------------------------------- 왼(뒷)쪽 다리 --------------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));

	TR = glm::scale(TR, glm::vec3(0.05f, 0.3f, 0.05f));
	TR = glm::translate(TR, glm::vec3(0.0f, -1.2f, -1.5f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = legnum_1; i < legnum_1 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::translate(TR, glm::vec3(0.0f, 1.2f, 1.5f));
	TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.3f, 1.0f / 0.05f));

	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));
	/* ---------------------------------------------------------------------------- */

	/* --------------------------------- 오른(잎)쪽 다리 --------------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(armDegree), glm::vec3(0.0, 0.0, 1.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));

	TR = glm::scale(TR, glm::vec3(0.05f, 0.3f, 0.05f));
	TR = glm::translate(TR, glm::vec3(0.0f, -1.2f, 1.5f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = legnum_2; i < legnum_2 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::translate(TR, glm::vec3(0.0f, 1.2f, -1.5f));
	TR = glm::scale(TR, glm::vec3(1.0f / 0.05f, 1.0f / 0.3f, 1.0f / 0.05f));

	TR = glm::translate(TR, glm::vec3(0.0f, -0.1f, 0.0f));
	TR = glm::rotate(TR, glm::radians(-armDegree), glm::vec3(0.0, 0.0, 1.0));
	TR = glm::translate(TR, glm::vec3(0.0f, 0.1f, 0.0f));
	/* ---------------------------------------------------------------------------- */
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

int openDir = 1;
int armRotateDir = 1;
int jumpDir = 1;
int jumpLimit = 1;
int blockJump = false;

void TimerFunction(int value) {

	if (animeFrontOpen) {
		if (degreeFront >= 90 || degreeFront < 0) {
			openDir *= -1;
			animeFrontOpen = false;
		}
		if (openDir == 1) degreeFront += 0.5f;
		else if (openDir == -1) degreeFront -= 0.5f;
	}

	if (armRotateAnime) {
		if (armDegree >= 45 || armDegree <= -45) {
			armRotateDir *= -1;
		}
		if (armRotateDir == 1) {
			armDegree += 0.5f;
		}
		else if (armRotateDir == -1) {
			armDegree -= 0.5f;
		}
	}


	switch (RobotMoveDir) {
	case 1:
		//printf("%d", jumpDir);
		// 로봇이 장애물 위에 올라가있지 않고, / 위로 올라가고 있을 때(가만히 있을 때도 포함)
		if (!RobotOnObstacle && jumpDir == 1) {
			// 움직인 이후의 값이 앞뒤 사이에 있으면?
			if (moveRobotZ + 0.005f >= obstacleZ - 0.25f && moveRobotZ + 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				; // 움직이지 않기
			}
			else moveRobotZ += 0.005f;
		}
		else if (jumpDir == -1) { // 내려오고 있을 때
			if (moveRobotZ + 0.005f >= obstacleZ - 0.25f && moveRobotZ + 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit 풀기
				moveRobotZ += 0.005f;
			}
			else {
				moveRobotZ += 0.005f;
			}
		}
		else if (RobotOnObstacle) { // 블럭 위에 올라가 있을 때
			if (!(moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f)) {
				RobotOnObstacle = false;
				jumpLimit = 1;
				jumpDir = -1;
				RobotJumpAnime = true;
			}
			moveRobotZ += 0.005f;
		}


		if (moveRobotZ > 1.65f) {
			moveRobotZ = -1.64f;
		}
		break;
	case -1:
		if (!RobotOnObstacle && jumpDir == 1) {
			// 움직인 이후의 값이 앞뒤 사이에 있으면?
			if (moveRobotZ - 0.005f >= obstacleZ - 0.25f && moveRobotZ - 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				; // 움직이지 않기
			}
			else moveRobotZ -= 0.005f;
		}
		else if (jumpDir == -1) { // 내려오고 있을 때
			if (moveRobotZ - 0.005f >= obstacleZ - 0.25f && moveRobotZ - 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit 풀기
				moveRobotZ -= 0.005f;
			}
			else {
				moveRobotZ -= 0.005f;
			}
		}
		else if (RobotOnObstacle) { // 블럭 위에 올라가 있을 때
			if (!(moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f)) {
				RobotOnObstacle = false;
				jumpLimit = 1;
				jumpDir = -1;
				RobotJumpAnime = true;
			}
			moveRobotZ -= 0.005f;
		}

		if (moveRobotZ > 1.65f) {
			moveRobotZ = -1.64f;
		}
		break;
	case 2:
		if (!RobotOnObstacle && jumpDir == 1) {
			// 움직인 이후의 값이 앞뒤 사이에 있으면?
			if (moveRobotX + 0.005f >= obstacleX - 0.25f && moveRobotX + 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				; // 움직이지 않기
			}
			else moveRobotX += 0.005f;
		}
		else if (jumpDir == -1) { // 내려오고 있을 때
			if (moveRobotX + 0.005f >= obstacleX - 0.25f && moveRobotX + 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit 풀기
				moveRobotX += 0.005f;
			}
			else {
				moveRobotX += 0.005f;
			}
		}
		else if (RobotOnObstacle) { // 블럭 위에 올라가 있을 때
			if (!(moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f)) {
				RobotOnObstacle = false;
				jumpLimit = 1;
				jumpDir = -1;
				RobotJumpAnime = true;
			}
			moveRobotX += 0.005f;
		}

		if (moveRobotX > 1.65f) {
			moveRobotX = -1.64;
		}
		break;
	case -2:
		if (!RobotOnObstacle && jumpDir == 1) {
			// 움직인 이후의 값이 앞뒤 사이에 있으면?
			if (moveRobotX - 0.005f >= obstacleX - 0.25f && moveRobotX - 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				; // 움직이지 않기
			}
			else moveRobotX -= 0.005f;
		}
		else if (jumpDir == -1) { // 내려오고 있을 때
			if (moveRobotX - 0.005f >= obstacleX - 0.25f && moveRobotX - 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit 풀기
				moveRobotX -= 0.005f;
			}
			else {
				moveRobotX -= 0.005f;
			}
		}
		else if (RobotOnObstacle) { // 블럭 위에 올라가 있을 때
			if (!(moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f)) {
				RobotOnObstacle = false;
				jumpLimit = 1;
				jumpDir = -1;
				RobotJumpAnime = true;
			}
			moveRobotX -= 0.005f;
		}

		if (moveRobotX < -1.65f) {
			moveRobotX = 1.64f;
		}
		break;
	}

	if (RobotJumpAnime) {
		if (jumpLimit == 1) {
			if (blockJump) {
				if (jumpDir && moveRobotY > -0.15f) {
					jumpDir *= -1;
					blockJump = false;
				}
			}
			else {
				if (jumpDir && moveRobotY > -0.4f) {
					jumpDir *= -1;
				}

			}

			if (jumpDir == 1) {
				// up
				moveRobotY += 0.01f;
			}
			else {
				// down
				moveRobotY -= 0.01f;
				if (moveRobotY < -1.0f) {
					moveRobotY = -1.0f;
					RobotJumpAnime = false;
					jumpDir = 1;
				}
			}
		}
		else {
			if (jumpDir && moveRobotY > -0.15f) {
				jumpDir *= -1;
			}

			if (jumpDir == 1) {
				// up
				moveRobotY += 0.01f;
			}
			else {
				// down
				moveRobotY -= 0.01f;
				if (moveRobotY < -0.75f) {
					moveRobotY = -0.75f;
					RobotJumpAnime = false;
					jumpDir = 1;
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

	case 'o': case 'O':
		if (animeFrontOpen) animeFrontOpen = false;
		else animeFrontOpen = true;
		break;

	case 't': case 'T':
		if (armRotateAnime) armRotateAnime = false;
		else armRotateAnime = true;
		break;

	case 'w': case 'W':
		RobotFaceDir = -1;
		RobotMoveDir = -1;
		armRotateAnime = true;
		break;
	case 'a': case 'A':
		RobotFaceDir = -2;
		RobotMoveDir = -2;
		armRotateAnime = true;
		break;
	case 's': case 'S':
		RobotFaceDir = 1;
		RobotMoveDir = 1;
		armRotateAnime = true;
		break;
	case 'd': case 'D':
		RobotFaceDir = 2;
		RobotMoveDir = 2;
		armRotateAnime = true;
		break;

	case 'j': case 'J': // jump
		RobotJumpAnime = true;
		break;
	case 'i': case 'I': // reset

		break;

	case 'z':
		cameraPosZ += 0.1f;
		break;
	case 'Z':
		cameraPosZ -= 0.1f;
		break;
	case 'x':
		cameraPosX += 0.1f;
		break;
	case 'X':
		cameraPosX -= 0.1f;
		break;
	case 'y':
		cameraY += 0.5f;
		break;
	case 'Y':
		cameraY -= 0.5f;
		break;

	case 'r': case 'R':

		break;



	case 'c': case 'C': // 모든 움직임 초기화
		animeFrontOpen = false;
		armRotateAnime = false;
		RobotMoveDir = 0;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		if (RobotMoveDir == -1) {
			// 애니메이션 끄기
			armRotateAnime = false;
			RobotMoveDir = 0;
			armDegree = 0.0f;
			armRotateDir = 1;
		}
		break;
	case 'a': case 'A':
		if (RobotMoveDir == -2) {
			armRotateAnime = false;
			RobotMoveDir = 0;
			armDegree = 0.0f;
			armRotateDir = 1;
		}
		break;
	case 's': case 'S':
		if (RobotMoveDir == 1) {
			armRotateAnime = false;
			RobotMoveDir = 0;
			armDegree = 0.0f;
			armRotateDir = 1;
		}
		break;
	case 'd': case 'D':
		if (RobotMoveDir == 2) {
			armRotateAnime = false;
			RobotMoveDir = 0;
			armDegree = 0.0f;
			armRotateDir = 1;
		}
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

	vertexSource = filetobuf("vertex2.glsl");
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
	fragmentSource = filetobuf("fragment2.glsl"); // 프래그세이더 읽어오기
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