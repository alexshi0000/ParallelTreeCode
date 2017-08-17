#ifndef graphics
#define graphics
#include <GL/gl.h>

void export_scene();				//these two scene functions will run in parallel
void render_scene();				//display function
void init();
void reshape_func(GLint w, GLint h);
void main_func(int *argc, char ***argv, int s, int f, int quality, int width, int height);			

extern long double x_pos, y_pos, z_pos, angle, i_r, j_r, k_r, zoom, tran_incr;			//camera variables
extern int steps, quality;
#endif	