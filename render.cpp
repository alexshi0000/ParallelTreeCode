#include <bits/stdc++.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define AU (1.396e+11)
using namespace std;

//TODO EVERYTHING NEEDS TO BE CHANGED

float zoom = 1;
double width = 1e6*AU;

void scroll_func(int button, int state, int x, int y){
   	if ((button == 3) || (button == 4)){
		if (button == 3 && state == GLUT_UP) 
			zoom *= 1.1;
		if (state == GLUT_DOWN && button == 4)  
			zoom /= 1.1;
   	} 
}

void display_func(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    string command;
    cin >> command;
    if(command.compare("d") == 0){
    	int n;
    	cin >> n;
    	for(int i = 0; i < n; i++){
    		double x, y, z;
    		cin >> x >> y >> z;
			float x1,y1,z1;
			glColor4f(1,1,1,1);
			x1 = x / (width * (double)zoom);	
			y1 = y / (width * (double)zoom);
			z1 = z / (width * (double)zoom);
			glPushMatrix ();
		        glTranslatef (x1, y1, z1);
		        glutSolidSphere (0.003, 5, 5);
			glPopMatrix ();
		}
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv){
	freopen("/home/lx_user/Documents/programming/c++/experimental/computional_physics/galactic_evolution/results/test.txt","r",stdin);
	glutInit(&argc,argv);			
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(700,30);
	glutInitWindowSize(900,900);
	glutCreateWindow("");
	glutDisplayFunc(display_func);
	glutMouseFunc(scroll_func);
	glutMainLoop();
}