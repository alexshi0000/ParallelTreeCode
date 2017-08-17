#include <bits/stdc++.h>
#include "simulation.h"
#include "scientific.h"
#include "data.h"

long double gen_rand(long double lo, long double hi){
	return (hi-lo) * ((long double)rand()/(long double)RAND_MAX) + lo;
}

void set_time_step(long double ts){
	time_step = ts;
}

void set_open_angle(long double theta){
	open_angle = theta;
}

void set_width(long double w){
	s_width = w;
}

void add_part(long double m, long double x, long double y, long double z, long double i, long double j, long double k){
	new particle(m,x,y,z,i,j,k);
}

void set_color(float r, float g, float b, float a){
	universe.back()->set_color(r,g,b,a);
}

void set_type(int type){
	universe.back()->set_type(type);
}

void set_radius(float r){
	universe.back()->set_radius(r);
}

long double rv_correction = 0;		//rotational velocity correction based on halo
bool dark_matter = false;
//rotational velocity is keplerian. TODO add rotation curve code. 
void spheriod(particle *center, int count, long double mass, long double radius, long double c, long double x_shift, long double y_shift, long double z_shift, 
					   long double h, long double f, long double l, long double angle){	//galactic center, particle cound, radius, flattening constant, position vector, rotational vector, angle of rotation
	for(int i = 0; i < count; i++){
		long double x_pos, y_pos;
		while(true){
			x_pos = gen_rand(-1,1);
			y_pos = gen_rand(-1,1);
			if(y_pos >= -sqrt(1-pow(x_pos,2)) && y_pos <= sqrt(1-pow(x_pos,2)))
				break;
		}
		long double z_max = sqrt(c*(1-pow(x_pos,2)-pow(y_pos,2)));
		long double z_min = -sqrt(c*(1-pow(x_pos,2)-pow(y_pos,2)));
		long double z_pos = gen_rand(z_min,z_max);
		x_pos *= radius;
		y_pos *= radius;
		z_pos *= radius;
		long double m = y_pos / x_pos;
		m = -1/m;
		long double b = y_pos - x_pos*m;
		vec *velocity = NULL;
		if(y_pos > 0){
			velocity = new vec(-radius/2,(x_pos-radius/2)*m + b - y_pos, 0);
			velocity->scalar(sqrt(
				(long double)(GV*(center->m+rv_correction))	/
				(long double)(sqrt(pow(x_pos,2) + pow(y_pos,2) + pow(z_pos,2)))
			));
		}
		else{
			velocity = new vec(radius/2,(x_pos+radius/2)*m + b - y_pos, 0);
			velocity->scalar(sqrt(
				(long double)(GV*(center->m+rv_correction))	/
				(long double)(sqrt(pow(x_pos,2) + pow(y_pos,2) + pow(z_pos,2)))
			));
		}
		add_part(mass, x_pos+x_shift, y_pos+y_shift, z_pos+z_shift, velocity->i, velocity->j, velocity->k);
		delete velocity;
		set_color(1,1,1,0.333);
		set_radius(0.0027);
		if(dark_matter)
			set_color(0,0,0,0);
	}
}

//test for accuracy by using this function and comparing earth x == 0 at T = 365.25 days
void solar_system_test(){
	set_open_angle(0.5);
	set_time_step(25000);
	set_width(10*AU);
		
	add_part(5.972e24,AU,0,0,0,2.978589e4,0); //earth
	set_color(0.1,1,0.1,1.0);
	set_radius(0.005);

	add_part(1.989e30,0,0,0,0,0,0);	//sun
	set_color(1,0.95,0.65,1.0);
	set_radius(0.01);
	add_part(5.972e24,108.2e9,0,0,0,35e3,0); //venus
	set_color(0.8,0.8,0.15,1.0);
	set_radius(0.0045);

	add_part(6.39e23,227.9e9,0,0,0,24.1e3,0); //mars
	set_color(0.9,0.3,0.1,1.0);
	set_radius(0.0033);
}

void simulate(){
	solar_system_test();
}	