#ifndef simulation
#define simulation
#include "scientific.h"

void set_time_step(long double ts);
void set_open_angle(long double theta);
void add_part(long double m, long double x, long double y, long double z, long double i, long double j, long double k);
void simulate();		//main calling function
void set_color(float r, float g, float b, float a);
void set_radius(float r);

#endif