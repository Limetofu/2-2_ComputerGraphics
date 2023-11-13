#include "stdafx.h"
#include "objRead.h"
#include "shader.h"


#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define DRILL 20

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
GLuint s_program;
GLuint fragmentShader;

GLuint vao, vbo[2];

// ���� ���� ����
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
	// x, y��
	{-3.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {0.0, -3.0, 0.0}, {0.0, 3.0, 0.0},

	// z��
	{0.0, 0.0, -3.0}, {0.0, 0.0, 3.0},

	// �Ʒ� 6
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// ���� 12
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
	// �Ʒ� 42
	{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5},

	// ���� 48
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

	obstacleX = Xsign * ((float)rand() / (RAND_MAX));
	obstacleZ = Zsign * ((float)rand() / (RAND_MAX));

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

	if (enable_depth) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);


	DrawShapeOri();


	glBindVertexArray(vao);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}


void DrawShapeOri() {


	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 VR = glm::mat4(1.0f);
	glm::mat4 PR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, 0.0f, cameraPosZ); //--- ī�޶� ��ġ
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

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
	TR = glm::translate(TR, glm::vec3(0.0, 0.0, -5.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ y�� ���Ͽ� ȸ�� ------------------------------ */
	TR = glm::translate(TR, glm::vec3(-cameraPosX * 2, 0.0f, 0.0));
	TR = glm::rotate(TR, glm::radians(cameraY), glm::vec3(0.0f, 1.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(cameraPosX * 2, 0.0f, 0.0));
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ �⺻ ȸ��, �� ------------------------------ */
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	// glDrawArrays(GL_LINES, 0, 2); glDrawArrays(GL_LINES, 2, 2); glDrawArrays(GL_LINES, 4, 2);
	/* ---------------------------------------------------------------------------- */


	/* ------------------------------ �ٴ� ------------------------------ */
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




	/* ------------------------------ ��ֹ� ------------------------------ */
	TR = glm::translate(TR, glm::vec3(obstacleX, -1.5f, obstacleZ));
	TR = glm::scale(TR, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 150; i < 150 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::scale(TR, glm::vec3(1.0f / 0.5f, 1.0f / 0.5f, 1.0f / 0.5f));
	TR = glm::translate(TR, glm::vec3(-obstacleX, 1.5f, -obstacleZ));
	/* ---------------------------------------------------------------------------- */


	/* ----------------------------------- ���� ����ü ----------------------------------- */
	TR = glm::translate(TR, glm::vec3(moveRobotX, moveRobotY, moveRobotZ));
	TR = glm::scale(TR, glm::vec3(0.3f, 0.4f, 0.3f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 42; i < 42 + 34; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);

	TR = glm::scale(TR, glm::vec3(1.0f / 0.3f, 1.0f / 0.4f, 1.0f / 0.3f));
	/* ---------------------------------------------------------------------------- */



	// arm scale = 0.1, 0.4, 0.1
	if (RobotFaceDir == 1) {
		/* ----------------------------------- ���� �� ----------------------------------- */
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

		/* ----------------------------------- ������ �� ----------------------------------- */
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
		/* ----------------------------------- ���� �� ----------------------------------- */
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


		/* ----------------------------------- ������ �� ----------------------------------- */
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
		/* ----------------------------------- ���� �� ----------------------------------- */
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

		/* ----------------------------------- ���� �� ----------------------------------- */
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
		/* ----------------------------------- ���� �� ----------------------------------- */
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

		/* ----------------------------------- ���� �� ----------------------------------- */
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




	/* ----------------------------------- �Ӹ� ----------------------------------- */
	TR = glm::translate(TR, glm::vec3(0.0f, 0.3f, 0.0f));
	TR = glm::scale(TR, glm::vec3(0.23f, 0.23f, 0.23f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 6; i < 39 + 1; i += 3) glDrawArrays(GL_TRIANGLES, i, 3);
	/* ---------------------------------------------------------------------------- */

	/* ----------------------------------- �� ----------------------------------- */
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
	/* --------------------------------- ���� �ٸ� --------------------------------- */
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

	/* ---------------------------- ������ �ٸ� / �յ� ---------------------------- */
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


	/* --------------------------------- ��(��)�� �ٸ� --------------------------------- */
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

	/* --------------------------------- ����(��)�� �ٸ� --------------------------------- */
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
		// �κ��� ��ֹ� ���� �ö����� �ʰ�, / ���� �ö󰡰� ���� ��(������ ���� ���� ����)
		if (!RobotOnObstacle && jumpDir == 1) {
			// ������ ������ ���� �յ� ���̿� ������?
			if (moveRobotZ + 0.005f >= obstacleZ - 0.25f && moveRobotZ + 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				; // �������� �ʱ�
			}
			else moveRobotZ += 0.005f;
		}
		else if (jumpDir == -1) { // �������� ���� ��
			if (moveRobotZ + 0.005f >= obstacleZ - 0.25f && moveRobotZ + 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit Ǯ��
				moveRobotZ += 0.005f;
			}
			else {
				moveRobotZ += 0.005f;
			}
		}
		else if (RobotOnObstacle) { // �� ���� �ö� ���� ��
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
			// ������ ������ ���� �յ� ���̿� ������?
			if (moveRobotZ - 0.005f >= obstacleZ - 0.25f && moveRobotZ - 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				; // �������� �ʱ�
			}
			else moveRobotZ -= 0.005f;
		}
		else if (jumpDir == -1) { // �������� ���� ��
			if (moveRobotZ - 0.005f >= obstacleZ - 0.25f && moveRobotZ - 0.005f <= obstacleZ + 0.25f &&
				moveRobotX >= obstacleX - 0.25f && moveRobotX <= obstacleX + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit Ǯ��
				moveRobotZ -= 0.005f;
			}
			else {
				moveRobotZ -= 0.005f;
			}
		}
		else if (RobotOnObstacle) { // �� ���� �ö� ���� ��
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
			// ������ ������ ���� �յ� ���̿� ������?
			if (moveRobotX + 0.005f >= obstacleX - 0.25f && moveRobotX + 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				; // �������� �ʱ�
			}
			else moveRobotX += 0.005f;
		}
		else if (jumpDir == -1) { // �������� ���� ��
			if (moveRobotX + 0.005f >= obstacleX - 0.25f && moveRobotX + 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit Ǯ��
				moveRobotX += 0.005f;
			}
			else {
				moveRobotX += 0.005f;
			}
		}
		else if (RobotOnObstacle) { // �� ���� �ö� ���� ��
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
			// ������ ������ ���� �յ� ���̿� ������?
			if (moveRobotX - 0.005f >= obstacleX - 0.25f && moveRobotX - 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				; // �������� �ʱ�
			}
			else moveRobotX -= 0.005f;
		}
		else if (jumpDir == -1) { // �������� ���� ��
			if (moveRobotX - 0.005f >= obstacleX - 0.25f && moveRobotX - 0.005f <= obstacleX + 0.25f &&
				moveRobotZ >= obstacleZ - 0.25f && moveRobotZ <= obstacleZ + 0.25f) {
				RobotOnObstacle = true;
				blockJump = true;
				jumpLimit = 0; // jumpLimit Ǯ��
				moveRobotX -= 0.005f;
			}
			else {
				moveRobotX -= 0.005f;
			}
		}
		else if (RobotOnObstacle) { // �� ���� �ö� ���� ��
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



	case 'c': case 'C': // ��� ������ �ʱ�ȭ
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
			// �ִϸ��̼� ����
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
