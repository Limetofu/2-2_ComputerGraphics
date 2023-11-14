#include "stdafx.h"
#include "objRead.h"

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define TRI_COUNT 500
#define SCREEN_W (GLint)1000
#define SCREEN_H (GLint)1000
#define GWIDTH (GLint)1000
#define GHEIGHT (GLint)1000
#define PI 3.141592

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
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);


void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitLight();
void InitShader();

void DrawShapeOri();


GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo;


// ���� ���� ����
int enable_depth = true;

GLfloat DEGREES = 0.0f;
GLfloat cameraPosZ = 3.0f;
GLfloat cameraPosX = 0.0f;

GLfloat cameraY = 0.0f;

glm::vec3 lightPos = { 0.f, 0.f, 5.f };
glm::vec3 lightColor = { 1.f, 1.f, 1.f };
glm::vec3 objColor = { 1.f, 0.5f, 0.3f };

GLfloat light_rotate_degree = 0.f;
GLfloat light_distance = 1.f;
GLfloat mid_rotate_degree = 0.f;

bool show_rectangle = true;

GLfloat pos[TRI_COUNT * 3][3] =
{
	// x��
	{-1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0},
	
	// y��
	{0.0, -1.0, 0.0},{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0},{0.0, 0.0, 1.0},

	// z��
	{0.0, 0.0, -1.0},{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},{0.0, 0.0, 1.0},

	// �Ʒ� 6
	{-0.5, -0.5, -0.5},{0.0, -1.0, 0.0}, {-0.5, -0.5, 0.5},{0.0, -1.0, 0.0},
	{0.5, -0.5, 0.5},{0.0, -1.0, 0.0},{0.5, -0.5, 0.5}, {0.0, -1.0, 0.0},
	{0.5, -0.5, -0.5},{0.0, -1.0, 0.0}, {-0.5, -0.5, -0.5},{0.0, -1.0, 0.0},

	// ���� 12
	{-0.5, 0.5, -0.5},{-1.0, 0.0, 0.0}, {-0.5, 0.5, 0.5},{-1.0, 0.0, 0.0},
	{-0.5, -0.5, 0.5},{-1.0, 0.0, 0.0},{-0.5, 0.5, -0.5}, {-1.0, 0.0, 0.0},
	{-0.5, -0.5, 0.5},{-1.0, 0.0, 0.0}, {-0.5, -0.5, -0.5},{-1.0, 0.0, 0.0},

	// right 18
	{0.5, -0.5, -0.5}, {1.0, 0.0, 0.0},{0.5, -0.5, 0.5},{1.0, 0.0, 0.0}, 
	{0.5, 0.5, 0.5},{1.0, 0.0, 0.0}, {0.5, -0.5, -0.5}, {1.0, 0.0, 0.0},
	{0.5, 0.5, 0.5}, {1.0, 0.0, 0.0},{0.5, 0.5, -0.5},{1.0, 0.0, 0.0},

	// behind 24
	{-0.5, 0.5, -0.5}, {0.0, 0.0, -1.0},{-0.5, -0.5, -0.5},{0.0, 0.0, -1.0},
	{0.5, -0.5, -0.5},{0.0, 0.0, -1.0}, {-0.5, 0.5, -0.5}, {0.0, 0.0, -1.0},
	{0.5, -0.5, -0.5}, {0.0, 0.0, -1.0},{0.5, 0.5, -0.5},{0.0, 0.0, -1.0},

	// front 30
	{-0.5, 0.5, 0.5},{0.0, 0.0, 1.0}, {-0.5, -0.5, 0.5},{0.0, 0.0, 1.0},
	{0.5, -0.5, 0.5},{0.0, 0.0, 1.0}, {-0.5, 0.5, 0.5},{0.0, 0.0, 1.0},
	{0.5, -0.5, 0.5},{0.0, 0.0, 1.0}, {0.5, 0.5, 0.5}, {0.0, 0.0, 1.0},

	// top 36
	{-0.5, 0.5, -0.5}, {0.0, 1.0, 0.0},{-0.5, 0.5, 0.5},{0.0, 1.0, 0.0},
	{0.5, 0.5, 0.5},{0.0, 1.0, 0.0},{0.5, 0.5, 0.5},{0.0, 1.0, 0.0},
	{0.5, 0.5, -0.5},{0.0, 1.0, 0.0}, {-0.5, 0.5, -0.5},{0.0, 1.0, 0.0},


	// �簢��
	// �Ʒ� 42
	{-0.5, -0.5, -0.5},{0.0, -1.0, 0.0}, {-0.5, -0.5, 0.5},{0.0, -1.0, 0.0},
	{0.5, -0.5, 0.5},{0.0, -1.0, 0.0},{0.5, -0.5, 0.5}, {0.0, -1.0, 0.0},
	{0.5, -0.5, -0.5},{0.0, -1.0, 0.0}, {-0.5, -0.5, -0.5},{0.0, -1.0, 0.0},

	// �� 48
	{0.5, -0.5, 0.5}, {0.0, 0.0, 1.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 1.0},
	{-0.5, -0.5, 0.5},{0.0, 0.0, 1.0},

	// �� 51
	{-0.5, -0.5, 0.5}, {-1.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {-1.0, 0.0, 0.0},
	{-0.5, -0.5, -0.5},{-1.0, 0.0, 0.0},

	// �� 54
	{0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {1.0, 0.0, 0.0},
	{0.5, -0.5, 0.5},{1.0, 0.0, 0.0},

	// �� 57
	{-0.5, -0.5, -0.5}, {0.0, 0.0, -1.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, -1.0},
	{0.5, -0.5, -0.5},{0.0, 0.0, -1.0},
};

GLfloat val = 0.f;


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
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));

	int Xsign, Zsign;

	if (rand() % 2 == 0) Xsign = 1;
	else Xsign = -1;

	if (rand() % 2 == 0) Zsign = 1;
	else Zsign = -1;

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}



GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	glEnable(GL_DEPTH_TEST);


	DrawShapeOri();


	glBindVertexArray(vao);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}


void DrawShapeOri() {


	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 VR = glm::mat4(1.0f);
	glm::mat4 PR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, 0.0f, cameraPosZ); //--- ī�޶� ��ġ
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	unsigned int projLocation = glGetUniformLocation(s_program, "projectionTransform");


	/* ------------------------------ ���� / ���� ���� ------------------------------ */
	PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	//TR = glm::translate(TR, glm::vec3(0.0, 0.0, -5.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ y�� ���Ͽ� ȸ�� ------------------------------ */
	//TR = glm::translate(TR, glm::vec3(-cameraPosX * 2, 0.0f, 0.0));
	//TR = glm::rotate(TR, glm::radians(cameraY), glm::vec3(0.0f, 1.0f, 0.0f));
	//TR = glm::translate(TR, glm::vec3(cameraPosX * 2, 0.0f, 0.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ �⺻ ȸ��, �� ------------------------------ */
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	/* ---------------------------------------------------------------------------- */

	// �⺻ ��
	TR = glm::rotate(TR, glm::radians(20.f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR = glm::rotate(TR, glm::radians(20.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	objColor = {0.f, 0.f, 0.f};
	InitLight();

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);

	/* ------------------------------ ���� �簢�� ------------------------------ */
	TR = glm::rotate(TR, glm::radians(light_rotate_degree), glm::vec3(0.f, 1.f, 0.f));
	TR = glm::translate(TR, glm::vec3(0.f, 0.f, light_distance));
	TR = glm::scale(TR, glm::vec3(1.f / 5.f, 1.f / 5.f, 1.f / 5.f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));


	glm::vec3 lightOffset = glm::vec3(0.f, 0.f, -0.5f);
	glm::vec4 lightPosInModelSpace = TR * VR * PR * glm::vec4(lightOffset, 1.0f);
	lightPos = glm::vec3(lightPosInModelSpace);


	objColor = { 0.5f, 0.5f, 0.5f };
	InitLight();

	for (int i = 6; i < 39 + 1; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::scale(TR, glm::vec3(5.f, 5.f, 5.f));
	TR = glm::translate(TR, glm::vec3(0.f, 0.f, -light_distance));
	TR = glm::rotate(TR, glm::radians(-light_rotate_degree), glm::vec3(0.f, 1.f, 0.f));
	/* ------------------------------------------------------------------------ */



	/* ------------------------------ �߾� �簢�� ------------------------------ */
	TR = glm::rotate(TR, glm::radians(mid_rotate_degree), glm::vec3(0.f, 1.f, 0.f));
	TR = glm::scale(TR, glm::vec3(1.f / 3.f, 1.f / 3.f, 1.f / 3.f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	objColor = { 0.f, 1.f, 0.f };
	InitLight();

	if (show_rectangle) {
		for (int i = 6; i < 39 + 1; i += 3)
			glDrawArrays(GL_TRIANGLES, i, 3);
	}
	else {
		for (int i = 42; i < 57 + 1; i += 3)
			glDrawArrays(GL_TRIANGLES, i, 3);
	}



	TR = glm::scale(TR, glm::vec3(3.f, 3.f, 3.f));
	TR = glm::rotate(TR, glm::radians(-mid_rotate_degree), glm::vec3(0.f, 1.f, 0.f));

	/* ------------------------------------------------------------------------ */
}



void InitBuffer()
{
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	InitLight();
}

void InitLight()
{
	glUseProgram(s_program);
	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	unsigned int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	glUniform3f(objColorLocation, objColor.x, objColor.y, objColor.z);
	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, cameraPosX, 0.f, cameraPosZ);
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

int i_light_rotate = 0;
bool b_mid_rotate_y = false;
bool off_light = false;

void TimerFunction(int value) {
	switch (i_light_rotate)
	{
	case 1: // ��
		light_rotate_degree += 0.6f;
		break;
	case -1: // ��
		light_rotate_degree -= 0.6f;
		break;
	}

	if (b_mid_rotate_y) {
		mid_rotate_degree += 1.f;
	}

	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}


GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {

	case 'n': case 'N': // ����ü / �簢��
		if (show_rectangle) show_rectangle = false;
		else show_rectangle = true;
		break;

	case 'm': case 'M': // ���� �Ѱ� ����
		if (off_light) 
		{ 
			off_light = false;
			lightColor = { 1.f, 1.f, 1.f };
		}
		else 
		{ 
			off_light = true; 
			lightColor = { 0.f, 0.f, 0.f };
		}
		break;

	case 'y': case 'Y': // ��ü y�� ���� ȸ��
		if (b_mid_rotate_y) b_mid_rotate_y = false;
		else b_mid_rotate_y = true;
		break;

	case 'r': // ���� y�� ��/�� ���� ����
		if (i_light_rotate == 1) i_light_rotate = 0;
		else i_light_rotate = 1;
		break;

	case 'R': // ��
		if (i_light_rotate == -1) i_light_rotate = 0;
		else i_light_rotate = -1;
		break;

	case 'z': // �ְ�
		light_distance += 0.1f;
		break;

	case 'Z': // ������
		light_distance -= 0.1f;
		break;

	case 'Q': case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {

	}
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
