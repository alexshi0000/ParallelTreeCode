#ifndef data
#define data
#include "scientific.h"
#include "simulation.h"

extern std::ofstream result;
void export_data(std::string s);
void create_ofstream(std::string dir);
void close_ofstream();
void flush_buffer();

#endif