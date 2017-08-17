#include <bits/stdc++.h>
#include <X11/Xlib.h>
#include "nbody_model.h"
#include "data.h"
#include "simulation.h"
#include "scientific.h"
#include "graphics.h"
#include "end.h"
//NOTE: make sure to compile with -lX11
struct timespec start_time, end_time;

void end_prog(){
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	double time_elapsed = (end_time.tv_sec - start_time.tv_sec);
	time_elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
	std::cout << "[Finished in : " << time_elapsed << "]" << std::endl;			//shows elapsed time
	close_ofstream();
	free(root);
}

int main(int argc, char **argv){
	//getting display configurations
	srand(time(NULL));//seed
	Display *disp = XOpenDisplay(NULL);
	Screen *screen = DefaultScreenOfDisplay(disp);
	create_ofstream("/home/lx_user/Documents/programming/c++/experimental/computional_physics/galactic_evolution/results/test.txt");
	cpu_count = std::thread::hardware_concurrency();
	root = (node**) malloc(sizeof(node*)*cpu_count);
	
	simulate();																	//creates environment
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	//export_data("m");															//export misc
	//export_data("v");															//export visualization data 	
	//export_data("p");															//export physics data
	//export_data("d");															//displacement
	main_func(&argc,&argv,10000,30,6,screen->width/2,screen->height/2);				//enter opengl code
	return 0;
}