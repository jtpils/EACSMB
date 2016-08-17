#ifndef __EACSMB_ROAD_H__
#define __EACSMB_ROAD_H__


typedef struct RoadVertex {
	Vector2 v;
	struct { float u, v; } t;
} RoadVertex;

// it occurs this is named incorrectly...
typedef struct RoadControlPoint {
	Vector2 cp0;
	Vector2 cp2;
	Vector2 cp1;
} RoadControlPoint;

typedef struct RoadSpline {
	int numSegments;
	RoadControlPoint cps[16];
} RoadSpline;

// dunno what to call this
typedef struct RoadBlock {
	int nextRoad, maxRoads;
	
	RoadControlPoint* cps; 
	
	char dirty;
	
} RoadBlock;



void drawRoad(RoadBlock* rb, GLuint dtex, Matrix* view, Matrix* proj);
void roadblock_update_vbo(RoadBlock* rb);
RoadBlock* allocRoadBlock();



#endif // __EACSMB_ROAD_H__