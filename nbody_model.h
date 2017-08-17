#ifndef nbody_model
#define nbody_model
#include "scientific.h"

void update();
void build_tree();
void choose_octant(node *curr, particle *p);
void insert_particle(int thread_idx, particle *p);
void insert_particle_util(node *root, particle *p);
void calculate_force(int tree_id, int tree_part_id, node *focus, long double m, long double x, long double y, long double z, int part_idx, vec *tmp);
void traversal_util(node *focus);
void traversal();
int tree_size(node *sub_root);
void tree_size_util(node *sub_root, int *counter);

#endif