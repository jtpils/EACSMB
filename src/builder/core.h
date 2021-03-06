#ifndef __EACSMB_builder_core_h__
#define __EACSMB_builder_core_h__


#include "../ds.h"
#include "../utilities.h"
#include "../common_math.h"


typedef struct MeshBuilderVertex {
	Vector v, n;
	struct {
		unsigned short u, v;
	} t;
} MeshBuilderVertex;

typedef VEC(MeshBuilderVertex) MBVList;

// not efficient by any stretch. no CSG capabilities.

typedef struct MeshData {
	MBVList verts;
	VEC(int) indices;
} MeshData;


// DO NOT REORDER
enum MB_op_type {
	MB_OP_NONE = 0,
	MB_OP_COMPOSE,
	MB_OP_TRANSFORM,
	MB_OP_CREATE_CYLINDER,
	MB_OP_CREATE_BOX,
	MB_OP_CREATE_SPHERE,
	MB_OP_CREATE_PYRAMID,
	
	// not implemented
	MB_OP_CREATE_TETRAHEDRON, 
	MB_OP_CREATE_ICOSAHEDRON,
	
	MB_OP_INVERT,
	MB_OP_DOUBLESIDE,
	MB_OP_SUBDIVIDE,
	
	MB_OP_MAX_VALUE
};



typedef union MB_operation MB_operation;

typedef VEC(MB_operation*) MB_op_list; 

typedef struct {
	enum MB_op_type type;
	MB_op_list children;
} MB_compose_params;



// TODO: skew, bend, twist, non-linear scaling
typedef struct {
	enum MB_op_type type;
	
	Vector position;
	Vector scale;
	Vector direction;
	float rotation;
	
	MB_op_list children;
} MB_transform_params;


typedef struct {
	enum MB_op_type type;
	
	float radius;
	int radial_segments;
	int vertical_segments;
	
} MB_sphere_params;


typedef struct {
	enum MB_op_type type;
	
	float height;
	float radius;
	int segments;
	int base_segments;
	
} MB_pyramid_params;


typedef struct {
	enum MB_op_type type;
	
	float length;
	float radius;
	
	// TODO: segments along the shaft
	int linear_segments;
	int radial_segments;
	
	char cap_min, cap_max;
} MB_cylinder_params;


typedef struct {
	enum MB_op_type type;
	
	Vector size;
	Vector origin; // normalized coordinates within the box
	
} MB_box_params;

union MB_operation {
	enum MB_op_type type;
	MB_compose_params compose;
	MB_transform_params transform;
	MB_cylinder_params cylinder;
	MB_box_params box;
	MB_sphere_params sphere;
	MB_pyramid_params pyramid;
};

MeshData* meshBuilder_test();





// temporary
MeshData* process_op(MB_operation* op);

MB_operation* mb_op_read_json(char* path);
void MB_free(MB_operation* op);



#endif // __EACSMB_builder_core_h__
