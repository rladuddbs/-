#include "CHeader.h"
#include "initialSet.h"
#include "CObj.h"
#include "MatrixFunc.h"


GLvoid drawScene();
void Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
void GetMouseDir();
void processMouse(int button, int state, int x, int y);
void processDragMouse(int x, int y);

bool SphereRayIntersection(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& rayStart, const glm::vec3& rayDirection);


GLint width, height;

CObj a;
double mx, my;

glm::mat4 ModelMatrix(1.0f);

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 20.0); //--- ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
glm::mat4 view = glm::mat4(1.0f);

glm::mat4 projection = glm::mat4(1.0f);

bool selected;

glm::vec3 objPos = glm::vec3(2.0, 0.0, 0.0);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 800;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	glewExperimental = GL_TRUE;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glewInit();


	

	a.LoadObj("tank.obj");
	a.initBuffer();

	make_vertexShaders(); 
	make_fragmentShaders();
	make_shaderProgram();
	glutKeyboardFunc(Keyboard); 
	glutMouseFunc(processMouse);
	glutMotionFunc(processDragMouse);
	glutTimerFunc(10, Timer, 1);
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "projectionTransform"), 1, GL_FALSE, glm::value_ptr(projection)
	);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "viewTransform"), 1, GL_FALSE, glm::value_ptr(view)
	);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);


	glUniformMatrix3fv
	(
		glGetUniformLocation(shaderProgramID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr
		(
			glm::mat3(glm::transpose(glm::inverse(ModelMatrix)))
		)
	);

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);

	glUniform3fv
	(
		lightPosLocation, 1, glm::value_ptr(glm::vec3(0.0,5.0,0.0))
	);

	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���

	glUniform3fv
	(
		lightColorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0))
	);

	My_rotate(shaderProgramID, ModelMatrix, glm::vec3(0.0, 1.0, 0.0), 90.f);

	My_translate(shaderProgramID, ModelMatrix, objPos);

	GLUquadric* planet_obj = gluNewQuadric();
	gluQuadricDrawStyle(planet_obj, GLU_FILL);
	gluQuadricNormals(planet_obj, GLU_SMOOTH);
	gluQuadricOrientation(planet_obj, GLU_OUTSIDE);
	gluSphere(planet_obj, 1.0, 50, 50);


	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}


void Timer(int value)
{
	

	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(1, Timer, 1); // Ÿ�̸��Լ� �� ����
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		
	}
	glutPostRedisplay(); // ȭ�� �� ���
}



void GetMouseDir() {

}



void processMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx = x, my = y;

		glm::vec3 rayStart = cameraPos;

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		glm::vec3 windowPos = glm::vec3(mx, viewport[3] - my, 0.0);
		glm::vec3 rayDir = glm::unProject(windowPos, view, projection, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

		glm::vec3 pickRay = glm::normalize(rayDir - rayStart);

		if (SphereRayIntersection(glm::vec3(2.0f, 0.0f, 0.0f), 1.f, rayStart, pickRay)) {
			std::cout << "���� ������" << std::endl;
			selected = true;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		selected = false;
	}
}
void processDragMouse(int x, int y) {
	if (selected) {
		// ���� ���콺 ��ġ�� ���ο� ������Ʈ ��ġ�� ����
		// ������ ��ǥ�� OpenGL ��ǥ�� ��ȯ
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		glm::vec3 windowPos = glm::vec3(x, viewport[3] - y, 0.0);
		glm::vec3 newObjectPos = glm::unProject(windowPos, view, projection, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

		// ���õ� ������Ʈ�� ��ġ�� ���ο� ���콺 ��ġ�� ������Ʈ
		objPos = newObjectPos;
		objPos.z = 0.0;
	}
}

bool SphereRayIntersection(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& rayStart, const glm::vec3& rayDirection) {
	glm::vec3 rayToSphere = sphereCenter - rayStart;
	float t = glm::dot(rayToSphere, rayDirection);
	glm::vec3 closestPoint;

	if (t < 0.0f) {
		closestPoint = rayStart;
	}
	else {
		closestPoint = rayStart + t * rayDirection;
	}

	glm::vec3 sphereToClosest = closestPoint - sphereCenter;
	float distanceSquared = glm::dot(sphereToClosest, sphereToClosest);

	return distanceSquared <= (sphereRadius * sphereRadius);
}