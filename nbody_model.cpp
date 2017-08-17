#include <pthread.h>
#include <bits/stdc++.h>
#include <omp.h>
#include "nbody_model.h"
#include "scientific.h"
#include "data.h"

void build_tree(){
	for(int i = 0; i < cpu_count; i++)
		root[i] = new node(0,0,0,s_width);
	#pragma omp parallel for num_threads(cpu_count) 
	for(int i = 0; i < particle_count; i++){
		particle *p = universe.at(i);
		int thread_idx = omp_get_thread_num();
		p->tree_part_id = thread_idx;
		insert_particle(thread_idx, p); 
	}
	update(); 
	for(int i = 0; i < cpu_count; i++)
		delete root[i];
}	

bool out_of_bounds(particle *p){
	return p->x >= s_width/2 || p->x <= -s_width/2 ||
		   p->y >= s_width/2 || p->y <= -s_width/2 ||
		   p->z >= s_width/2 || p->z <= -s_width/2;
}
	
void choose_octant(node *curr, particle *p){
	if(p->x < curr->x){
		if(p->y < curr->y){
			if(p->z < curr->z)
				insert_particle_util(curr->g,p);
			else
				insert_particle_util(curr->c,p);
		}
		else{
			if(p->z < curr->z)
				insert_particle_util(curr->e,p);
			else
				insert_particle_util(curr->a,p);
		}
	}
	else{
		if(p->y < curr->y){
			if(p->z < curr->z)
				insert_particle_util(curr->h,p);
			else
				insert_particle_util(curr->d,p);
		}
		else{
			if(p->z < curr->z)
				insert_particle_util(curr->f,p);
			else
				insert_particle_util(curr->b,p);
		}
	}
}	
	
void insert_particle_util(node *focus, particle *p){
	if(focus->body == nullptr && focus->a == nullptr)		//no body found and leaf node
		focus->body = p;
	else if(focus->body != nullptr){						//body found and leaf node
		particle *q = focus->body;
		focus->expand();
		focus->body = nullptr;
		choose_octant(focus, p);
		choose_octant(focus, q);
		focus->get_mass_aprox();
	}
	else{													//no body found and internal node
		choose_octant(focus, p);
		focus->get_mass_aprox();
	}
}	
	
void insert_particle(int tree_id, particle *p){				//tree_id is the tree number assigned for cpu_idx
	insert_particle_util(root[tree_id], p);
}	

void update(){
	std::stack<int> removal_stack;							//contains the idx
	for(int i = 0; i < particle_count; i++){
		particle *p = universe.at(i);
		if(p->type == 0){
			vec **net_force_arr = (vec**) malloc(sizeof(vec*)*cpu_count);
			long double x = p->x;
			long double y = p->y;
			long double z = p->z;
			long double m = p->m;
			int part_idx = p->id;
			//take force step from each tree
			#pragma omp parallel for num_threads(cpu_count)
			for(int j = 0; j < cpu_count; j++){								
				vec *tmp = new vec(0,0,0);
				calculate_force(j, p->tree_part_id, root[j], m, x, y, z, part_idx, tmp);
				net_force_arr[j] = tmp;
			}
			//summing up the net forces 
			vec *net_force = new vec(0,0,0);
			for(int j = 0; j < cpu_count; j++){
				vec *ptr = net_force;
				net_force = vec::add(net_force, net_force_arr[j]);
				delete net_force_arr[j];
				delete ptr;
			}
			delete p->a;
			delete p->f;
			p->f = net_force;
			//update the acceleration and velocity of the particle
			p->a = new vec(p->f->i, p->f->j, p->f->k);
			p->a->scalar((p->f->m / p->m) * time_step);
			vec *ptr = p->v;
			p->v = vec::add(p->v, p->a);
			delete ptr;
			p->x += p->v->i * time_step;
			p->y += p->v->j * time_step;
			p->z += p->v->k * time_step;
			if(out_of_bounds(p)){
				#pragma omp critical
				removal_stack.push(i);			//critical 
			}
			free(net_force_arr);
		}
	}
	while(removal_stack.size() > 0){
		int idx = removal_stack.top();
		removal_stack.pop();
		particle *gb = universe.at(idx);
		delete gb;								//particle_count--
		gb = nullptr;
		universe.erase(universe.begin()+idx);
	}
}

//tree id is the identification of the tree we are currently traversing, tree_part_id is the tree the particle belongs to
void calculate_force(int tree_id, int tree_part_id, node *focus, const long double m, const long double x, const long double y, const long double z, const int part_idx, vec *tmp){
	if(focus->body != nullptr && focus->body->id != part_idx){
		vec *gravity = new vec(focus->body->x-x, focus->body->y-y, focus->body->z-z);
		gravity->scalar(
			(GV * focus->body->m * m) /
			(pow(focus->body->x-x, 2) + pow(focus->body->y-y, 2) + pow(focus->body->z-z, 2))
		);
		vec *sum = vec::add(tmp, gravity);
		tmp->i  = sum->i;
		tmp->j  = sum->j;
		tmp->k  = sum->k;
		delete gravity;
		delete sum;
	}
	else if(focus->width / sqrt(pow(focus->cmx-x,2) + pow(focus->cmy-y,2) + pow(focus->cmz-z,2)) < open_angle){			//total aproximation
		long double tmp_com = focus->com;
		long double tmp_cmx = focus->cmx;
		long double tmp_cmy = focus->cmy;
		long double tmp_cmz = focus->cmz;											//TODO total aprox, maybe don't implement? Its far too inaccurate
		if(focus->width / sqrt(pow(focus->cmx-x,2) + pow(focus->cmy-y,2) + pow(focus->cmz-z,2)) > 0.5 && tree_id == tree_part_id){	//belongs to tree
			tmp_com -= m;
			tmp_cmx = (focus->com*focus->cmx - m*x) / tmp_com;
			tmp_cmy = (focus->com*focus->cmy - m*y) / tmp_com;
			tmp_cmz = (focus->com*focus->cmz - m*z) / tmp_com;
		}
		vec *gravity = new vec(tmp_cmx-x, tmp_cmy-y, tmp_cmz-z);
		gravity->scalar(
			(GV * tmp_com * m) / 
			(pow(tmp_cmx-x,2) + pow(tmp_cmy-y,2) + pow(tmp_cmz-z,2))
		);
		tmp->i = (gravity->i + tmp->i);
		tmp->j = (gravity->j + tmp->j);
		tmp->k = (gravity->k + tmp->k);
		delete gravity;
	}
	else{
		if(focus->a != nullptr)
			calculate_force(tree_id, tree_part_id, focus->a, m, x, y, z, part_idx, tmp);
		if(focus->b != nullptr)
			calculate_force(tree_id, tree_part_id, focus->b, m, x, y, z, part_idx, tmp);
		if(focus->c != nullptr)
			calculate_force(tree_id, tree_part_id, focus->c, m, x, y, z, part_idx, tmp);
		if(focus->d != nullptr)
			calculate_force(tree_id, tree_part_id, focus->d, m, x, y, z, part_idx, tmp);

		if(focus->e != nullptr)
			calculate_force(tree_id, tree_part_id, focus->e, m, x, y, z, part_idx, tmp);
		if(focus->f != nullptr)
			calculate_force(tree_id, tree_part_id, focus->f, m, x, y, z, part_idx, tmp);
		if(focus->g != nullptr)
			calculate_force(tree_id, tree_part_id, focus->g, m, x, y, z, part_idx, tmp);
		if(focus->h != nullptr)
			calculate_force(tree_id, tree_part_id, focus->h, m, x, y, z, part_idx, tmp);
	}
}

void tree_size_util(node *sub_root, int *counter){
	if(sub_root == nullptr)
		return;
	if(sub_root->body != nullptr)
		(*counter)++;
	tree_size_util(sub_root->a, counter);
	tree_size_util(sub_root->b, counter);
	tree_size_util(sub_root->c, counter);
	tree_size_util(sub_root->d, counter);

	tree_size_util(sub_root->e, counter);
	tree_size_util(sub_root->f, counter);
	tree_size_util(sub_root->g, counter);
	tree_size_util(sub_root->h, counter);
}

int tree_size(node *sub_root){
	int counter = 0;
	tree_size_util(sub_root, &counter);
	return counter;
}

void traversal_util(node *focus){
	if(focus != nullptr){
		if(focus->is_leaf())
			std::cout << focus->body->id << " ";
		else{
			traversal_util(focus->a);
			traversal_util(focus->b);
			traversal_util(focus->c);
			traversal_util(focus->d);

			traversal_util(focus->e);
			traversal_util(focus->f);
			traversal_util(focus->g);
			traversal_util(focus->h);
		} 
	}
}

void traversal(){
	for(int i = 0; i < cpu_count; i++)
		traversal_util(root[i]);
	std::cout << std::endl;
}