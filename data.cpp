#include <bits/stdc++.h>
#include "data.h"
#include "scientific.h"
#include "simulation.h"

std::ofstream result;

void create_ofstream(std::string dir){ 
	result.open(dir);
}

void flush_buffer(){
	result.flush();
}

void close_ofstream(){
	result.close();
}

void export_data(std::string s){
	if(s.compare("v") == 0){					//v visualization
		result << "v\n";
		result << std::to_string(particle_count)+"\n";
		for(int i = 0; i < particle_count; i++){
			particle *p = universe.at(i);
			result << std::to_string(p->color[0]) +" "+ std::to_string(p->color[1]) +" "+ std::to_string(p->color[2]) +" "+ std::to_string(p->color[3]) + "\n";
			result << std::to_string(p->radius) + "\n";
		}    
	}
	else if(s.compare("d") == 0){		//d particle
		result << "d\n";
		result << std::to_string(particle_count)+"\n";
		for(int i = 0; i < particle_count; i++)
			result << std::to_string(universe.at(i)->x)+" "+std::to_string(universe.at(i)->y)+" "+std::to_string(universe.at(i)->z)+" "+"\n";
	}
	else if(s.compare("p") == 0){			//p physics, used by advanced visualization techniques heatmaps, netforce coloring, etc
		result << "p\n";
		result << std::to_string(particle_count)+"\n";
		for(int i = 0; i < universe.size(); i++){
			long double m = universe.at(i)->m;
			vec *f = universe.at(i)->f; 
			vec *a = universe.at(i)->a;
			vec *v = universe.at(i)->v;
			result << std::to_string(m) +"\n";
			result << std::to_string((long double)f->i)+" "+std::to_string((long double)f->j)+" "+std::to_string((long double)f->k)+"\n";
			result << std::to_string((long double)a->i)+" "+std::to_string((long double)a->j)+" "+std::to_string((long double)a->k)+"\n";
			result << std::to_string((long double)v->i)+" "+std::to_string((long double)v->j)+" "+std::to_string((long double)v->k)+"\n";
		}
	}
	else if(s.compare("m") == 0){			//m misc
		result << "m\n";
		result << std::to_string(s_width)+"\n";
		result << std::to_string(particle_count)+"\n";
		result << std::to_string(node_count)+"\n";
		result << std::to_string(vector_count)+"\n";
	}
	flush_buffer();
}