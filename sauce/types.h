
typedef struct{ //represent edge by pair of vertex indices
	int x;
	int y;
}link;

typedef struct{ //2d vector
	double x;
	double y;
}vec2;

typedef struct{ //2d vector
	double x;
	double y;
	double z;
}vec3;

typedef struct{ //2d transformation
	double rotate;
	vec2 scale;
	vec2 translate;
}transform;

typedef struct{ //2d vector with transformation
	transform localtransform;
	double x;
	double y;
}point;

typedef struct{ //line with 2d vertices
	transform localtransform;
	vec2 vertex[2];
}line;

typedef struct{ //triangle with 2d vertices
	transform localtransform;
	vec2 vertex[3];
}triangle;

typedef struct{ //2d mesh
	transform localtransform;
	vec2 * vertex;
	link * edge;
}mesh;

typedef struct{ //2d mesh with texture coordinates
	transform localtransform;
	vec2 * vertex;
	vec2 * uv;
	link * edge;
}uvmesh;

typedef struct{ //3d vector
	double x;
	double y;
	double z;
}vec3;

typedef struct{ //3d transform
	vec3 rotate;
	vec3 scale;
	vec3 translate;
}transform3d;

typedef struct{ //3d vector with transform
	transform3d localtransform;
	double x;
	double y;
	double z;
}point3d;

typedef struct{ //triangle with 3d vertices
	transform3d localtransform;
	vec3 vertex[2];
}line3d;

typedef struct{ //triangle with 3d vertices
	transform3d localtransform;
	vec3 vertex[3];
}triangle3d;

typedef struct{ //3d mesh
	transform3d localtransform;
	vec3 * vertex;
	link * edge;
}mesh3d;

typedef struct{ //3d mesh with texture coordinates
	transform3d localtransform;
	vec3 * vertex;
	vec3 * uv;
	link * edge;
}uvmesh3d;
