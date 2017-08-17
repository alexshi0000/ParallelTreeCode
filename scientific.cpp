#include <bits/stdc++.h>
#include "scientific.h"

const long double AU = 1.396e+11;
const long double PI = 3.14159265359;
const long double GV = 6.673e-11;
const long double LY = 9.461e+15;
const long double LC = 299792458;
const long double STD = (1.0/86400);
const long double DTY = (1.0/365.25);
const long double YTM = (1.0/1e+6);

node **root;
long vector_count = 0, particle_count = 0, node_count = 0, id_count = 0;
int cpu_count;
long double open_angle;
long double time_step;
long double s_width;
std::vector<particle*> universe;

vec::vec(long double i, long double j, long double k){
	this->i = i;
	this->j = j;
	this->k = k;
	m = sqrt(i*i + j*j + k*k);
	vector_count++;
}

vec::~vec(){
	vector_count--;
}

void vec::scalar(long double magnitude){
	if(m == 0){}
		//std::cout << "cannot apply scalar multiplication" << std::endl;
	else{
		long double coeff = magnitude / m;
		m = magnitude;
		i *= coeff;
		j *= coeff;
		k *= coeff;
	}
}

vec* vec::add(vec *u, vec *v){
	return new vec(u->i+v->i, u->j+v->j, u->k+v->k);
}

vec* vec::unit(vec *u){
	vec *v = new vec(u->i, u->j, u->k);
	v->scalar(1.0);
	return v;
}

vec* vec::cross_product(vec *u, vec *v){
	vec *result = new vec(0,0,0);
	result->i = u->j * v->k - u->k * v->j;
	result->j = v->i * u->k - u->i * v->k;
	result->k = u->i * v->j - v->i * u->j;
	return result;
}

long double vec::dot_product(vec *u, vec *v){
	return u->i*v->i + u->j*v->j + u->k*v->k;
}

particle::particle(long double m, long double x, long double y, long double z, long double i, long double j, long double k){
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	color[3] = 0;
	radius = 0;
	id = id_count;
	tree_part_id = -1;					//to be assigned on build_tree()
	this->m = m;
	this->x = x;
	this->y = y;
	this->z = z;
	v = new vec(i,j,k);
	a = new vec(0,0,0);
	f = new vec(0,0,0);
	type = 0;
	particle_count++;
	id_count++;
	universe.push_back(this);
}

particle::~particle(){
	delete a;
	delete f;
	delete v;
	a = nullptr;
	f = nullptr;
	v = nullptr;
	particle_count--;
}

void particle::set_type(int type){
	this->type = type;
}

void particle::set_color(float r, float g, float b, float a){
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void particle::set_radius(float r){
	radius = r;
}

node::node(long double x, long double y, long double z, long double width){
	com = 0;
	cmx = 0;
	cmy = 0;
	cmz = 0;
	body = nullptr;
	a = nullptr;
	b = nullptr;
	c = nullptr;
	d = nullptr;
	e = nullptr;
	f = nullptr;
	g = nullptr;
	h = nullptr;
	this->x = x;
	this->y = y;
	this->z = z;
	this->width = width;
	node_count++;
}

node::~node(){
	delete a;
	delete b;
	delete c;
	delete d;
	delete e;
	delete f;
	delete g;
	delete h;
	body = nullptr;
	a = nullptr;
	b = nullptr;
	c = nullptr;
	d = nullptr;
	e = nullptr;
	f = nullptr;
	g = nullptr;
	h = nullptr;
	node_count--;
}

void node::expand(){
	a = new node(x-width/4, y+width/4, z+width/4, width/2);
	b = new node(x+width/4, y+width/4, z+width/4, width/2);
	c = new node(x-width/4, y-width/4, z+width/4, width/2);
	d = new node(x+width/4, y-width/4, z+width/4, width/2);

	e = new node(x-width/4, y+width/4, z-width/4, width/2);
	f = new node(x+width/4, y+width/4, z-width/4, width/2);                                 
	g = new node(x-width/4, y-width/4, z-width/4, width/2);
	h = new node(x+width/4, y-width/4, z-width/4, width/2);
}

void node::get_mass_aprox(){
	com = 0;
	cmx = 0;
	cmy = 0;
	cmz = 0;
	if(a->body != nullptr){
		com += a->body->m;
		cmx += a->body->x * a->body->m;
		cmy += a->body->y * a->body->m;
		cmz += a->body->z * a->body->m;
	}
	else{
		com += a->com;
		cmx += a->cmx * a->com;
		cmy += a->cmy * a->com;
		cmz += a->cmz * a->com;
	}
	if(b->body != nullptr){
		com += b->body->m;
		cmx += b->body->x * b->body->m;
		cmy += b->body->y * b->body->m;
		cmz += b->body->z * b->body->m;
	}
	else{
		com += b->com;
		cmx += b->cmx * b->com;
		cmy += b->cmy * b->com;
		cmz += b->cmz * b->com;
	}
	if(c->body != nullptr){
		com += c->body->m;
		cmx += c->body->x * c->body->m;
		cmy += c->body->y * c->body->m;
		cmz += c->body->z * c->body->m;
	}
	else{
		com += c->com;
		cmx += c->cmx * c->com;
		cmy += c->cmy * c->com;
		cmz += c->cmz * c->com;
	}
	if(d->body != nullptr){
		com += d->body->m;
		cmx += d->body->x * d->body->m;
		cmy += d->body->y * d->body->m;
		cmz += d->body->z * d->body->m;
	}
	else{
		com += d->com;
		cmx += d->cmx * d->com;
		cmy += d->cmy * d->com;
		cmz += d->cmz * d->com;
	}
	if(e->body != nullptr){
		com += e->body->m;
		cmx += e->body->x * e->body->m;
		cmy += e->body->y * e->body->m;
		cmz += e->body->z * e->body->m;
	}
	else{
		com += e->com;
		cmx += e->cmx * e->com;
		cmy += e->cmy * e->com;
		cmz += e->cmz * e->com;
	}
	if(f->body != nullptr){
		com += f->body->m;
		cmx += f->body->x * f->body->m;
		cmy += f->body->y * f->body->m;
		cmz += f->body->z * f->body->m;
	}
	else{
		com += f->com;
		cmx += f->cmx * f->com;
		cmy += f->cmy * f->com;
		cmz += f->cmz * f->com;
	}
	if(g->body != nullptr){
		com += g->body->m;
		cmx += g->body->x * g->body->m;
		cmy += g->body->y * g->body->m;
		cmz += g->body->z * g->body->m;
	}
	else{
		com += g->com;
		cmx += g->cmx * g->com;
		cmy += g->cmy * g->com;
		cmz += g->cmz * g->com;
	}
	if(h->body != nullptr){
		com += h->body->m;
		cmx += h->body->x * h->body->m;
		cmy += h->body->y * h->body->m;
		cmz += h->body->z * h->body->m;
	}
	else{
		com += h->com;
		cmx += h->cmx * h->com;
		cmy += h->cmy * h->com;
		cmz += h->cmz * h->com;
	}
	cmx = cmx / com;
	cmy = cmy / com;
	cmz = cmz / com;
}

bool node::is_leaf(){
	return body != nullptr &&
		   a == nullptr	   &&
		   b == nullptr    && 
		   c == nullptr	   &&
		   d == nullptr    &&
		   e == nullptr	   &&
		   f == nullptr    &&
		   g == nullptr	   &&
		   h == nullptr;
}