#include <bits/stdc++.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <omp.h>
#include "scientific.h"
#include "nbody_model.h"
#include "end.h"

long double x_pos=0, y_pos=0, z_pos=0, angle=0, i_r=1.0, j_r=0, k_r=0, zoom=0.13, tran_incr=0;	//translation icrement amount
int quality=0, steps=0, frame_step=0;
bool init=true;

void render_scene(){
	init = false;
	//exit
	if(steps <= 0){
		end_prog();
		glutLeaveMainLoop();
	}
	else
		steps--;
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//rotation
	glMatrixMode(GL_MODELVIEW);
	glRotated(angle, i_r, j_r, k_r);
	//blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(int i = 0; i < particle_count; i++){
    	particle *p = universe.at(i);
    	long double x0 = (p->x+x_pos)/(s_width*zoom);
    	long double y0 = (p->y+y_pos)/(s_width*zoom);
    	long double z0 = (p->z+z_pos)/(s_width*zoom);
	    glColor4f(p->color[0], p->color[1], p->color[2], p->color[3]);
	    glPushMatrix();
		glTranslatef((float)x0, (float)y0, (float)z0);   	
		glutSolidSphere(p->radius,quality,quality);
	    glPopMatrix();
	}
	for(int i = 0; i < frame_step; i ++)
		build_tree();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape_func(GLint w, GLint h){
	if(init){
		GLfloat width = GLfloat(w);
		GLfloat height = GLfloat(h);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		float aspect = (float)width / (float)height;
		glOrtho(-aspect, aspect, -1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutFullScreen();
	}
	else
		glViewport(0, 0, w, h);
}

void main_func(int *argc, char ***argv, int s, int f, int q, int width, int height){
	frame_step = f;
	steps = s;
	quality = q;
	glutInit(argc,(*argv));			//we pass argc instead of &argc because argc is already a pointer defined in main.cpp
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(700,30);
	glutInitWindowSize(width,height);
	glutCreateWindow("");
	glutDisplayFunc(render_scene);
	glutReshapeFunc(reshape_func);
	glutMainLoop();
}
