#ifndef scientific
#define scientific																	//contains all units, scientific variables, and global variables

class vec{
	public:
		long double i, j, k, m;
		vec(long double i, long double j, long double k);
		~vec();
		void scalar(long double magnitude);
		static vec* add(vec *u, vec *v);
		static vec* unit(vec *u);
		static vec* cross_product(vec *u, vec *v);
		static long double dot_product(vec *u, vec *v);
};

class particle{
	public:
		float color[4];
		float radius;
		int type, tree_part_id;
		long id;
		long double x, y, z, m;
		vec *a, *f, *v;
		particle(long double m, long double x, long double y, long double z, long double i, long double j, long double k);
		~particle();
		void set_type(int type);													//type is 0 or 1
		void set_color(float r, float g, float b, float a);
		void set_radius(float r);
};

class node{
	public:
		long double width, x, y, z, com, cmx, cmy, cmz;
		node *a, *b, *c, *d, *e, *f, *g, *h;
		particle *body;
		node(long double x, long double y, long double z, long double width);
		~node();
		bool is_leaf();																//if an external node with a body
		void expand();																//expand 8 quadrants
		void get_mass_aprox();														//aproximate center of mass
};

extern std::vector<particle*> universe;	//where all of the particles are stored
extern const long double AU;																//astronomical unit
extern const long double PC;																//parsec
extern const long double GV;																//gravitational constant
extern const long double LC;																//speed of light
extern const long double LY;																//light year
extern const long double STD;															//seconds to day 
extern const long double DTY;															//day to year
extern const long double YTM;															//year to myrs
extern const long double PI;																//pi
extern long double open_angle;
extern long double time_step;															
extern long double s_width;
extern int cpu_count;
extern long vector_count, node_count, particle_count, id_count;
extern node **root;																	//each root for cpu
#endif