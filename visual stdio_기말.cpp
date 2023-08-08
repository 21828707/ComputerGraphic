#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist) {
	glTranslatef(0.0, 0.0, -radius); // ������ ������ ����
	glRotatef(-twist, 0.0, 0.0, 1.0); // ī�޶� ����� ����
	glRotatef(-elevation, 1.0, 0.0, 0.0);// ���� 
	glRotatef(-azimuth, 0.0, 1.0, 0.0); // ������
} // �����̵��� ���� �޼ҵ�

float timerForCameraMove = 0; // ���� ���ư� �� ���.

// �˿��� ����ϴ� ����
int born_mode = 0;
int born = 0;
float falling = 0;
float move_section0 = 0;
float egg_R = 0.3;
float s_size = 0.3;

int section = 0; // ����� �̵�
int Time  = 0; // ���� �ð�
int Time_flow = 0; // �ð��� �帧�� ���� section���� <- Ÿ�̸� �ݹ�


// �ֹ��� ���
double body = 0.15;
double eye = 0.02;

// �ֹ����� �̵�
GLfloat bug_Delta = -0.5; // ������ ����, �̿� ǥ��
int move_type = 0; // ������ �̵��� ��� -> �̿Ͻ� �Ӹ��� ������ �̵�
float move_point = 0.000001; // ������ ����ϱ� ���� 0.0�� ���� �ȵ�
int moment = 0;

//�ֹ����� �Ļ翡 ���
GLboolean bug_bite = false; // ������ ���� ����
GLboolean bug_bite_action = true; // ������ �Դ� �ൿ
GLboolean bug_action = false; // ���� ���� ������

GLint bug_count = 0; //���� ���� ������ Ƚ��
GLfloat Bug_eat_Delta = 0.0; // ������ ������ ���
GLfloat background_r = 0.8; // ����_Red
GLfloat background_g = 0.86; // ����_Green
GLfloat background_b = 0.95; // ����_Blue

//������
double body_R = 0.4;
int delta = 0;

void make_branch(float R, float S, float X, float Y) {

	glRotatef(R, 0.0, 0.0, 1.0);
	glScalef(S, S, S);
	glTranslatef(X, Y, 0.0);
	glColor3f(0.5, 0.2, 0.0); // ����
	glBegin(GL_POLYGON);
	glVertex3f(0.05, 2.0, 0.0);
	glVertex3f(0.05, -2.0, 0.0);
	glVertex3f(-0.05, -2.0, 0.0);
	glVertex3f(-0.05, 2.0, 0.0);
	glEnd();
}

double leaf_rad = 0.499; // ������ ��翡 ���
void make_leaf(float R, float S, float X, float Y) {
	glColor3f(0.3, 0.8, 0.3);//�ʷ�
	glScalef(S, S, S);
	glTranslatef(X, Y, 0.0);
	glRotatef(R, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x = leaf_rad * cos(angle);
		double y = leaf_rad * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();

	glPushMatrix();
	glRotatef(260, 0.0, 0.0, 1.0);
	glTranslatef(0.41, -0.105, 0.0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(70, 0.0, 0.0, 1.0);
	glTranslatef(0.41, -0.105, 0.0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glEnd();
	glPopMatrix();
}

void bug_bite_motion(float R, float S, float X, float Y) {
	glPushMatrix();
		glColor3f(background_r, background_g, background_b);
		glScalef(S, S, S);
		glTranslatef(X, Y, 0.0);
		glRotatef(R, 0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			double angle = i * 3.141592 / 180;
			double x = 0.3 * cos(angle);
			double y = 0.3 * sin(angle);
			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();
}

// �ֹ��� => ������ �̵��� ���ϰ� �ϱ� ���� Rotation �� Translate
void make_bug(float R, float S, float X, float Y) {

	glScalef(S, S, S);
	glRotatef(R, 0.0, 0.0, 1.0);
	glTranslatef(X, Y, 0.0);

	glTranslatef(0.5, 0.0, 0.0);
	glColor3f(0.0, 0.5, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x = body * cos(angle);
		double y = body * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();

	glPushMatrix();
	glTranslatef(0.05, 0.06, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x = eye * cos(angle);
		double y = eye * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glColor3f(0.0, 0.5, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.5, 0.15 + Bug_eat_Delta, 0.0);
	glVertex3f(0.5, -0.15 - Bug_eat_Delta, 0.0);
	glVertex3f(bug_Delta, -0.15 - Bug_eat_Delta, 0.0);
	glVertex3f(bug_Delta, 0.15 + Bug_eat_Delta, 0.0);
	glEnd();

	glPushMatrix();
	glTranslatef(bug_Delta, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x = body * cos(angle);
		double y = body * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

float B_delta = 180;	// between 0 and 360, by time ������
float B_move = 0.0;
int wait_time = 0;
void makeWing() {
	// Big wing
	glVertex3f(0, 0, 0);
	glVertex3f(0.2, 0.8, 0);
	glVertex3f(0.4, 0.2, 0);
	// Small Wing
	glVertex3f(0, 0, 0);
	glVertex3f(0.32, -0.16, 0);
	glVertex3f(0.16, -0.4, 0);
}
void make_butterfly(int C, float S, float X, float Y, float Z) {

	glScalef(S, S, S);
	glTranslatef(X, Y, Z);


	glutSolidSphere(0.01, 10, 10);

	glPushMatrix();// one parts of wings
	if (C == 1) { glColor3f(0.5, 0, 0); }
	else if (C == 2) { glColor3f(0, 0.5, 0); }
	else { glColor3f(0, 0, 0.5); }
	glRotatef(sin(B_delta) * 60, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	makeWing();
	glEnd();
	glPopMatrix();

	glPushMatrix();// other wings
	glRotatef(180, 0, 1, 0);
	if (C == 1) { glColor3f(0.7, 0.3, 0.3); }
	else if (C == 2) { glColor3f(0.3, 0.7, 0.3); }
	else { glColor3f(0.3, 0.3, 0.7); }
	glRotatef(-sin(B_delta) * 60, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	makeWing();
	glEnd();
	glPopMatrix();
}

//------------------����---------------------------------
void make_branch_3D(float S, float X, float Y, float Z, float pitch, float yaw, float roll) {


	glPushMatrix();
	glScalef(S, S, S);
	glTranslatef(X, Y, Z);
	glRotatef(pitch, 1, 0, 0);
	glRotatef(yaw, 0, 1, 0);
	glRotatef(roll, 0, 0, 1);
	double rad = 0.2;
	float branchLength = 4;     // ���������� ������

	glBegin(GL_POLYGON);
	glPushMatrix();
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x1 = rad * cos(angle);
		double y1 = rad * sin(angle);
		glVertex3f(x1, y1, branchLength);
	}
	glPopMatrix();
	glEnd();

	glBegin(GL_POLYGON);
	glPushMatrix(); glColor3f(0.5, 0.2, 0.0);
	for (float k = 0; k < 360; k += 0.01) {
		double angle = k * 3.141592 / 180;
		double x3 = rad * cos(angle);
		double y3 = rad * sin(angle);
		glVertex3f(x3, y3, branchLength);
		glVertex3f(x3, y3, -branchLength);
	}
	glPopMatrix();
	glEnd();

	glBegin(GL_POLYGON);
	glPushMatrix(); glColor3f(0.2, 0.1, 0);
	for (int j = 0; j < 360; j++) {
		double angle = j * 3.141592 / 180;
		double x2 = rad * cos(angle);
		double y2 = rad * sin(angle);
		glVertex3f(-x2, -y2, -branchLength);
	}
	glPopMatrix();
	glEnd();

	glPopMatrix();
}

void make_leaf_3D(float S, float X, float Y, float Z, float pitch, float yaw, float roll) {

	glNormal3f(0.5, 0.3, 0.5);
	glColor3f(0.3, 0.8, 0.3);//�ʷ�
	glScalef(S, S, S);
	glTranslatef(X, Y, Z);
	glRotatef(pitch, 1, 0, 0);  // X�� ȸ��, pitch
	glRotatef(yaw, 0, 1, 0);  // Y�� ȸ��, yaw
	glRotatef(roll, 0, 0, 1);   // Z �� ȸ�� roll
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x = leaf_rad * cos(angle);
		double y = leaf_rad * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();

	glPushMatrix();
	glRotatef(260, 0.0, 0.0, 1.0);
	glTranslatef(0.41, -0.105, 0.0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(70, 0.0, 0.0, 1.0);
	glTranslatef(0.41, -0.105, 0.0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glEnd();
	glPopMatrix();


}
//-----------------����----------------------------------
GLboolean under_move = false;
GLboolean butt_move = true;
GLfloat flower_move = -4.7;
GLfloat camera_under_angle = -5.0;
GLfloat camera_under_earth = -3.0;
GLint flowerleaf_angle = 0; // ��������

float B_move_y = -6.0;
float B_move_z = -3.0;

void cylinder(float R, float S, float X, float Y) { // �ٱ�
	glScalef(S, S, S);
	glTranslatef(X, Y, 0.0);
	glRotatef(R, 0.0, 0.0, 1.0);

	glColor3f(0.0, 0.8, 0.0);
	double rad = 0.09;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		double angle = i * 3.141592 / 180;
		double x1 = rad * cos(angle);
		double y1 = rad * sin(angle);
		glVertex3f(x1, y1, 0.0);
	}
	glEnd();
	glPushMatrix();
	glBegin(GL_POLYGON);
	for (float k = 0; k < 360; k = k + 0.01) {
		double angle = k * 3.141592 / 180;
		double x3 = rad * cos(angle);
		double y3 = rad * sin(angle);
		glVertex3f(x3, y3, 0.0);
		glVertex3f(x3, y3, -5.0);
	}
	glEnd();

	glPushMatrix();
	glBegin(GL_POLYGON);
	for (int j = 0; j < 360; j++) {
		double angle = j * 3.141592 / 180;
		double x2 = rad * cos(angle);
		double y2 = rad * sin(angle);
		glVertex3f(-x2, -y2, -1.0);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void flowerleaf() { // ���� ����

	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, -0.2);
	glVertex3f(-0.4, -0.4, -0.2);
	glVertex3f(0, 0.4, -0.2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.7, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0, -0.4, 0.0);
	glVertex3f(-0.4, 0.4, 0.0);
	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.4, 0.4, -0.2);
	glVertex3f(-0.4, 0.4, -0.2);
	glVertex3f(0, -0.4, -0.2);
	glEnd();
	glPopMatrix();
}

void two_flowerleaf() { // ���� �ѽ� ����
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glRotatef(15, 0.0, 1.0, 0.0);
	flowerleaf();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	flowerleaf();
	glPopMatrix();
}


void flower() {
	glTranslatef(flower_move, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.1, 100, 100);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.05);
	cylinder(0.0, 0.5, 0.0, 0.0);
	glPopMatrix();

	for (flowerleaf_angle = 0; flowerleaf_angle <= 180; flowerleaf_angle += 45) {
		glRotatef(flowerleaf_angle, 0.0, 0.0, 1.0);
		glPushMatrix();
		two_flowerleaf();
		glPopMatrix();
	}

	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.2, 100, 100);
}

GLint count = 1;
GLfloat background_above_r = 0.21;
GLfloat background_above_g = 0.57;
GLfloat background_above_b = 0.69;

GLfloat background_down_r = 0.81;
GLfloat background_down_g = 0.92;
GLfloat background_down_b = 0.96;

void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (section) {
	case 0:
		gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glPushMatrix();
			make_leaf(0.0, 2.0, 0.0, 0.0);
			glPopMatrix();

		glPushMatrix();
			make_bug(45.0, 0.6, -1.1 + move_section0, 0.0);
			glPopMatrix();

		glPushMatrix();
			glColor3f(0.3, 0.8, 0.3);//�ʷ�
			glTranslatef(-0.8, -0.8, 0.0);
			glBegin(GL_POLYGON);
			glVertex3f(-s_size, s_size, 0.0);
			glVertex3f(s_size, s_size, 0.0);
			glVertex3f(s_size, -s_size, 0.0);
			glVertex3f(-s_size, -s_size, 0.0);
			glEnd();
			glPopMatrix();

		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(-0.4, -0.4 - falling, 0.0);

			if (born_mode == 0) {
				glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++) {
					double angle = i * 3.141592 / 180;
					double x = egg_R * cos(angle);
					double y = egg_R * sin(angle);
					glVertex3f(x, y, 0.0);

				}
				glEnd();
			}
			else
			{
				glRotatef(65, 0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);

				for (int i = 0; i < 180; i++) {
					double angle = i * 3.141592 / 180;
					double x = egg_R * cos(angle);
					double y = egg_R * sin(angle);
					glVertex3f(x, y, 0.0);

				}
				glEnd();

				glRotatef(135, 0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (int i = 0; i < 180; i++) {
					double angle = i * 3.141592 / 180;
					double x = egg_R * cos(angle);
					double y = egg_R * sin(angle);
					glVertex3f(x, y, 0.0);

				}
				glEnd();
			}
			glPopMatrix();

		break;
	case 1:
		gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glPushMatrix();
			make_branch(20.0, 1.0, 0.0, 0.0);// ��������
			glPushMatrix();
				make_branch(-20.0, 1.0, 0.0, 2.0);
				glPopMatrix();
			glPopMatrix();

		glPushMatrix();
			glRotatef(0, 0.0, 0.0, 1.0);
			glutWireSphere(0.01, 10, 10);//--------------------------------

			glPushMatrix();
				make_leaf(0.0, 0.7, -1.3, 0.7);// ������ 2 �»�
				glPopMatrix();
			glPopMatrix();

		glPushMatrix();
			make_leaf(-45.0, 0.7, 1.0, 0.7);// ������ 3 ���
			glPopMatrix();
	
		glPushMatrix();
			make_leaf(0.0, 0.7, -0.77, -0.7);// ������ 1 ����
				// ���� ���� 1 push ����!!! 1 �Ʒ���
				if (bug_count >= 3) {
					bug_bite_motion(0.0, 1.0, 0.0, 0.5);
				}
				// ���� ���� 2 push ����!!! 1 �Ʒ���
				if (bug_count >= 5) {
					bug_bite_motion(0.0, 0.7, 0.5, 0.9);
					bug_bite_action = false;
				}
			glPopMatrix();

		//�ֹ���
		glPushMatrix();
			if (moment == 0) {
				make_bug(45.0, 0.3, -2.5 + move_point, 0.2);
			}
			else if (moment == 1) {
				make_bug(90.0, 0.3, -2.5 + move_point, 0.2);
			}
			else if (moment == 2) {
				make_bug(0.0, 0.3, -3.5 + move_point, 1.5);
			}
			else { // ������� ����!!!!
				move_point = 0;
				make_bug(0.0, 0.3, 2.099, 1.5);
			}
			glPopMatrix();
		break;
	case 2:
		gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		
		glPushMatrix();
			make_branch(-10.0, 2.0, -0.4, 0.0);
			glPopMatrix();

		glPushMatrix();
			make_leaf(-80, 1.3, 0.3, -0.2);
			glPopMatrix();

		glPushMatrix();
			make_bug(-25.0, 0.8, 0.5, 0.0);
			//������
			glPushMatrix();
				glTranslatef(-0.5, 0.0, 0.0);
				glColor3f(0.0, 0.3, 0.0);
				glBegin(GL_POLYGON);
				for (int i = 0; i < delta; i++) {
					double angle = i * 3.141592 / 180;
					double x = body_R * cos(angle);
					double y = body_R * sin(angle);
					glVertex2f(x * 1.9, y * 0.8);
				}
				glEnd();
				glPopMatrix();
			glPopMatrix();

		break;

	case 3:
		gluLookAt(sin(timerForCameraMove) * 5, 0.3, cos(timerForCameraMove) * 5, 0, 0.3, 0, 0, 1, 0);	// ��, Ÿ��, õ��

		glPushMatrix();
		make_leaf_3D(1.2, -1.3, 0.7, -0.15, 0, 0, 0);// ������ 2 �»�
		glPopMatrix();

		glPushMatrix();
		make_leaf_3D(1.2, -0.7, -0.7, 0.275, 140, 180, 350);//������ 1 ����
		glPopMatrix();

		//material1();
		make_branch_3D(0.5, 0, 4, 0, 90, 0, 0);	// S X Y Z p y r
		make_branch_3D(0.5, 0, 0, 0, 90, 20, 0);

		glPushMatrix();
		make_leaf_3D(1.2, 1, 0.5, 0.55, 140, 0, 350);// ������ 3 ���

		glColor3f(0.0, 0.3, 0.0);

		glRotatef(40, 0, 0, 1);   // Z �� ȸ�� roll
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			double angle = i * 3.141592 / 180;
			double x = body_R * cos(angle);
			double y = body_R * sin(angle);
			glVertex2f(x * 0.95, y * 0.4);
		}
		glEnd();
		glPopMatrix();

		// ���� bufferfly
		glPushMatrix();
		glTranslatef(1, 0.6, 0.5);
		glRotatef(80, 0.0, 0.0, 1.0);
		glRotatef(80, 0.0, 1.0, 0.0);
		make_butterfly(1, 1.5, 0.0, B_move, 0.1);
		glPopMatrix();
		break;

	case 4:
		gluLookAt(0.0, camera_under_angle, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(-1.0, 6.0 + camera_under_angle, 0.0);
			glRotatef(90.0, 0.0, 0.0, 1.0);
			glRotatef(-150.0, 0.0, 1.0, 0.0);
			make_butterfly(2, 0.7, 0.0, 0.0, 0.0);
			glPopMatrix();

		glPushMatrix();
			glTranslatef(2.0, 7.0 + camera_under_angle, 0.0);
			glRotatef(90.0, 0.0, 0.0, 1.0);
			glRotatef(-150.0, 0.0, 1.0, 0.0);
			make_butterfly(0, 0.7, 0.0, 0.0, 0.0);
			glPopMatrix();
		
		glPushMatrix();
			glColor3f(0.5, 1.0, 0.3);
			glTranslatef(0.0, camera_under_earth, 0.0);
			glBegin(GL_POLYGON);
			glVertex3f(2.0, 1.5, 0.0);
			glVertex3f(-2.0, 1.5, 0.0);
			glVertex3f(-2.0, -1.5, 0.0);
			glVertex3f(2.0, -1.5, 0.0);
			glEnd();
			glPopMatrix();

		glTranslatef(0.0, 0.0, -0.35);
		flower();
		
		glTranslatef(0.0, 0.0, -0.35);
		glScalef(2.0, 2.0, 2.0);
		glTranslatef(0.0, 0.4, -0.7);
		make_butterfly(1, 0.5, 0.0, B_move_y, -B_move_z);
		break;

	default:
		gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glLoadIdentity();
		glColor3f(0.0, 0.0, 0.0);
		glutSolidCube(5.0);
		break;
	}

	

	glFlush();
}

void myreshape(int w, int h) {
	glViewport(0, 0, w, h);

	GLfloat widthfactor = (GLfloat)w / (GLfloat)300;
	GLfloat heightfactor = (GLfloat)h / (GLfloat)300;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	GLfloat ratio = (float)w / h;
	gluPerspective(40, ratio, 0.1, 1000); // ����
}

void MyTimer(int value) {
	printf("%d\n",Time);
	Time += 1;
	if (Time < 80) { Time_flow = 0; section = 0; }
	else if (Time < 90) { section = 1000; }
	else if (Time < 260) { Time_flow = 1; section = 1; }
	else if (Time < 270) { section = 1000; }
	else if (Time < 370) { Time_flow = 2; section = 2; }
	else if (Time < 380) { section = 1000; }
	else if (Time < 470) { Time_flow = 3; section = 3; }
	else if (Time < 480) { section = 1000; }
	else if (Time < 750) { Time_flow = 4; section = 4; }
	else { section = 1000; }

	switch (Time_flow) {
	case 0:
		born += 1;

		if (born > 10) {
			born = 10;
			born_mode = 1;
			move_section0 += 0.05;
			if (move_section0 > 1.0) { move_section0 = 1.0; falling += 0.05; }
		}
		break;
	case 1:
		if (bug_bite_action) {
			if (Bug_eat_Delta >= 0.05) { bug_action = true; }
			else if (Bug_eat_Delta <= 0.0) {
				bug_action = false;
				bug_count++;
			}
			if (bug_action) { Bug_eat_Delta -= 0.01; }
			else { Bug_eat_Delta += 0.01; } // �Դ� �ӵ� ����
		}
		else if (move_point != 0.0 && bug_bite_action == false) {
			if (bug_Delta >= -0.2) { move_type = 1; }
			else if (bug_Delta <= -0.5) { move_type = 0; }

			if (move_type == 1) { bug_Delta = bug_Delta - 0.1; move_point += 0.1; }
			else if (move_type == 0) { bug_Delta = bug_Delta + 0.1; }

		}

		if (move_point >= 5.5) {
			moment = 3;
		}
		else if (move_point >= 3.5) { moment = 2; }
		else if (move_point >= 2.0) { moment = 1; }
		break;

	case 2:
		if (delta >= 360) {}
		else { delta += 5; }
		break;

	case 3:

		if (timerForCameraMove < 1)
			timerForCameraMove += 0.05;	
		wait_time += 1;

		if (wait_time > 40) {
			B_move += 0.1;
			if (B_move >= 20.5) { B_move = 20.5; }
			if (B_delta == 360)
				B_delta = 0;
			B_delta += 0.5;
		}
		break;

	case 4:
		if (butt_move) {
			B_move_y += 0.04;
			if (B_delta == 360) { B_delta = 0; }
			B_delta += 0.8;
			flower_move += 0.04;
			if (flower_move > -1.0) {
				under_move = true;
				butt_move = false;
			}
		}
		else if (under_move) {
			if (B_move_z < -0.2) {
				B_move_z += 0.02;
				if (B_move_y < -0.8) {
					B_move_y += 0.02;
				}
				if (B_delta == 360) { B_delta = 0; }
				B_delta += 0.8;
				if (camera_under_angle <= 0.0) { camera_under_angle += 0.07; }
				if (camera_under_earth <= 0.0) { camera_under_earth += 0.05; }
			}
			else {
				if (B_delta <= 300) { B_delta += 0.1; }
			}
			if (flower_move < 0) {
				flower_move += 0.02;
			}
		}
		break;
	default:
		break;
	}


	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);// �� �̻��� ���� ��� ����!
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);// ������(����) ���� ��ġ

	glutCreateWindow("OpenGL Drawing!!!!");

	glClearColor(background_r, background_g, background_b, 1.0);
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape); // ��
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();

	return 0;

}