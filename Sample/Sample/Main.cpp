#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")

#include <iostream>
#include <random>
#include <stdlib.h>
#include <stdio.h>

#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/freeglut_std.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>


char* filetobuf(const char* file);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_vertexShader();
void make_FragmentShader();
void make_shaderProgram();
void InitBuffer();
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);

GLint width, height;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

GLuint VAO_cube, VBO_cube[2];
GLuint VAO_line, VBO_line[2];
GLuint VAO_board, VBO_board[2];

int projT = 1;

float vertexPosition[] = {
	// 뒷면: 위, 아래
	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,

	-0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,

	// 앞면: 위, 아래
	-0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,

	-0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5, 
	0.5, -0.5, 0.5,

	//  윗면: 위, 아래
	-0.5, 0.5, -0.5, 
	0.5, 0.5, 0.5,
	0.5, 0.5, -0.5,

	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,

	// 아랫면: 위, 아래
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,


	// 왼쪽면: 위, 아래
	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,

	-0.5, 0.5, 0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,

	// 오른쪽 면: 위, 아래
	0.5, 0.5, 0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,

	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, -0.5, -0.5
};

float vertexCol[] = {
	// 뒷면
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	// 앞면
	0.0, 1.0, 1.0, 
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,

	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,

	// 윗면	
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
		
	// 아랫면
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	// 왼쪽면
	1.0, 0.0, 0.0, 
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	// 오른쪽면
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,

	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0

};

float linePosition[] = {
	-1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, 1.0
};

float lineCol[] = {
	1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 0.0, 1.0
};

float boardPosition[] = {
	-3.0, -0.5, -3.0,
	3.0, -0.5, 3.0,
	3.0, -0.5, -3.0,

	-3.0, -0.5, -3.0, 
	-3.0, -0.5, 3.0, 
	3.0, -0.5, 3.0 
};

float boardCol[] = {
	0.8, 0.8, 0.8,
	0.8, 0.8, 0.8,
	0.8, 0.8, 0.8,

	0.8, 0.8, 0.8,
	0.8, 0.8, 0.8,
	0.8, 0.8, 0.8
};

float xTrans, yTrans, zTrans;
float yRot = 0.0;
float xRot = 0.0;
float ySelfRot = 0.0;
float yZeroRot = 0.0f;

glm::vec3 cameraPos;			//--- 카메라 위치
glm::vec3 cameraDirection;		//--- 카메라 바라보는 방향
glm::vec3 cameraUp;			//--- 카메라 위쪽 방향
glm::vec3 cameraFront;

float bigYRot;

void main(int argc, char** argv) 	//--- 윈도우 출력하고 콜백함수 설정
{
	int dummy[100];

	width = 800;
	height = 600;

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

	}
	else {
		std::cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		
	}

	make_vertexShader();
	make_FragmentShader();
	make_shaderProgram();

	InitBuffer();

	printf("p: projection\n");
	printf("w/s: x translation\n");
	printf("a/d: y translation\n");
	printf("z/Z: z translation\n");
	printf("y/Y: y rotation\n");
	printf("x/X: x rotation\n");
	printf("r: camera rotation - lookat fct");
	printf("1/2: camera z-translation\n");
	printf("3/4: camera x-translation\n");
	printf("5/6: camera가 공전\n");
	printf("left/right: 모델링 변환으로 카메라 자전\n");

	glutDisplayFunc(drawScene);	//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMainLoop();
}

GLvoid drawScene() 			//--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;
	GLfloat boardSize = 5.0;

	rColor = gColor = 0.0;
	bColor = 1.0;			//--- 배경색을 파랑색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	InitBuffer();

	glUseProgram(shaderProgram);



	//--- Modeling Transformation
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model_matrix");
	glm::mat4 transform = glm::mat4(1.0f);

	//transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	//transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));

	transform = glm::translate(transform, glm::vec3(xTrans, yTrans, zTrans));

	transform = glm::rotate(transform, glm::radians(yRot), glm::vec3(0.0, 1.0, 0.0));
	transform = glm::rotate(transform, glm::radians(xRot), glm::vec3(1.0, 0.0, 0.0));


	//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

	//--- 뷰잉 변환을 모델링 변환으로 사용하는 경우
	//glm::mat4 TR = glm::mat4(1.0f);				//--- 이동 행렬 선언
	transform = glm::translate(transform, cameraPos);			//--- x축으로 이동 행렬
	transform = glm::rotate(transform, glm::radians(bigYRot), glm::vec3(0.0, 1.0, 0.0));	//--- z축에 대하여 회전 행렬
	transform = glm::translate(transform, -cameraPos);			//--- x축으로 이동 행렬
	//---------------------------------

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));


	//--- Projjection Transformation
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "proj_matrix");
	glm::mat4 PM = glm::mat4(1.0f);
	if (projT == 0)
		PM = glm::ortho(-boardSize, boardSize, -boardSize, boardSize, -boardSize, 100.0f);
	else {
		PM = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 50.0f);
		//transform = glm::translate(transform, glm::vec3(0.0, 0.0, -2.0));
	}
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(PM));





	//--- Viewing Transformation
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view_matrix");

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraPos+ cameraDirection, cameraUp);
	//view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);


	transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 6);

	glBindVertexArray(VAO_board);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers(); 			// 화면에 출력하기
}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
		xTrans += 0.1;
	else if (key == 's')
		xTrans -= 0.1;
	else if (key == 'a')
		yTrans += 0.1;
	else if (key == 'd')
		yTrans -= 0.1;
	else if (key == 'z')
		zTrans += 0.1;
	else if (key == 'Z')
		zTrans -= 0.1;

	else if (key == 'y')
		yRot += 10.0;
	else if (key == 'Y')
		yRot -= 10.0;
	else if (key == 'x')
		xRot += 10.0;
	else if (key == 'X')
		xRot -= 10.0;
	else if (key == 'r') {
		ySelfRot += 10.0f;
		cameraDirection = glm::vec3(sin(glm::radians(ySelfRot)), 0.0, -cos(glm::radians(ySelfRot)));
		cameraDirection = glm::normalize(cameraDirection);
	}
	else if (key == 'R') {
		ySelfRot -= 10.0f;
		cameraDirection = glm::vec3(sin(glm::radians(ySelfRot)), 0.0, -cos(glm::radians(ySelfRot)));
		cameraDirection = glm::normalize(cameraDirection);
	}

	//--- camera position change
	else if (key == '1') {
		cameraPos.z += 0.1;

		std::cout << "cameraPos.z = " << cameraPos.z << std::endl;
	}
	else if (key == '2') {
		cameraPos.z -=  0.1;

		std::cout << "cameraPos.z = " << cameraPos.z << std::endl;
	}
	else if (key == '3') {
		cameraPos.x += 0.1;
		cameraDirection.x += 0.1;
	}
	else if (key == '4') {
		cameraPos.x -= 0.1;
		cameraDirection.x -= 0.1;
	}
	else if (key == '5') {
		yZeroRot += 0.1f;
		glm::vec3 front;
		front.x = cameraPos.x * cos(glm::radians(yZeroRot)) + cameraPos.z * sin(glm::radians(yZeroRot));
		front.y = cameraPos.y;
		front.z = cameraPos.x * (-sin(glm::radians(yZeroRot))) + cameraPos.z * cos(glm::radians(yZeroRot));

		cameraPos = front;

		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::normalize(cameraPos - target);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), direction));
		
		cameraDirection = -direction;

	}
	else if (key == '6') {
		yZeroRot -= 0.1f;
		glm::vec3 front;
		front.x = cameraPos.x * cos(glm::radians(yZeroRot)) + cameraPos.z * sin(glm::radians(yZeroRot));
		front.y = cameraPos.y;
		front.z = cameraPos.x * (-sin(glm::radians(yZeroRot))) + cameraPos.z * cos(glm::radians(yZeroRot));

		cameraPos = front;

		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::normalize(cameraPos - target);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), direction));
		
		cameraDirection = -direction;
	}

	
	else if (key == 'p') {
		projT++;
		projT %= 2;
	}
	else if (key == 'c') {
		xRot = yRot = 0.0;
		xTrans = yTrans = zTrans = 0;
		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);			//--- 카메라 위치
		cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);		//--- 카메라 바라보는 방향
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//--- 카메라 위쪽 방향
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		projT = 0;
	}
	else if (key == 'q') {
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}


void Special(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		bigYRot +=10.0f;

	}

	else if (key == GLUT_KEY_DOWN) {
		bigYRot -= 10.0f;
	}
	else if (key == GLUT_KEY_LEFT) {
	}
	else if (key == GLUT_KEY_RIGHT) {
	}
	glutPostRedisplay();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)				//--- 콜백 함수: 다시 그리기 콜백 함수
{
	//glViewport(0, 0, w, h);
}


//----------------------------------------------------------------------
// Make VAO, VBO buffer

GLvoid InitBuffer()
{
	glGenVertexArrays(1, &VAO_cube);
	glGenBuffers(2, VBO_cube);
	glBindVertexArray(VAO_cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCol), vertexCol, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &VAO_line);
	glGenBuffers(2, VBO_line);
	glBindVertexArray(VAO_line);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_line[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(linePosition), linePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_line[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineCol), lineCol, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &VAO_board);
	glGenBuffers(2, VBO_board);
	glBindVertexArray(VAO_board);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_board[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boardPosition), boardPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_board[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boardCol), boardCol, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);			//--- 카메라 위치
	cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);		//--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//--- 카메라 위쪽 방향
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
}


//--------------------------------------------------------------------------
// Make shader program
void make_vertexShader()
{
	GLchar* vertexSource;

	// 버텍스 셰이더 읽어와서 객체 만들고 컴파일하기
	vertexSource = filetobuf ("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShader == 0)
		std::cout << "vertex shader: 0" << std::endl;
	else
		std::cout << "vertex shader: 1" << std::endl;

	glShaderSource (vertexShader, 1, &vertexSource, NULL);
	glCompileShader (vertexShader);

	// error check
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

void make_FragmentShader()
{
	GLchar* fragmentSource;


	// 버텍스 셰이더 읽어와서 객체 만들고 컴파일하기
	fragmentSource = filetobuf ("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (fragmentShader == 0)
		std::cout << "fragment shader: 0" << std::endl;
	else
		std::cout << "fragment shader: 1" << std::endl;

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// error check
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_shaderProgram()
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);	// ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}

	glUseProgram(shaderProgram);

}


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb"); 		// Open file for reading 
	if (!fptr) 			// Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); 		// Seek to the end of the file 
	length = ftell(fptr); 			// Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); 		// Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); 		// Go back to the beginning of the file 
	fread(buf, length, 1, fptr); 		// Read the contents of the file in to the buffer 
	fclose(fptr); 			// Close the file 
	buf[length] = 0; 			// Null terminator 

	return buf; 			// Return the buffer 
}