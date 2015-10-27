#ifndef __map_h__
#define __map_h__





typedef struct MapBlock {
	void* data[2];
	size_t stride;
	
	int width;
	int height;
	
	char* description;
	
	struct MapBlock* near[2][2];
	
	
} MapBlock;

MapBlock* allocMapBlock(size_t stride, int w, int h);

// terrain textures must always be a power of two
#define TERR_TEX_SZ 1024
// terrain blocks must always be a power of two minus two
#define TERR_BLOCK_SZ (TERR_TEX_SZ - 2)

// disused in latest algorithm
// // number of patches per block, in one dimension. must be integral
// #define TERR_PATCH_DIVISOR 32
// // maximum divisions per patch
// #define TERR_MAX_TESS 32

typedef struct TerrainPatchVertex {
	float x, y, z;
	float hmU, hmV; // these are in texels
	short divX, divY;
} TerrainPatchVertex;


typedef struct TerrainBlock {
	float zs[TERR_TEX_SZ * TERR_TEX_SZ];
// 	float zs[TERR_BLOCK_SZ * TERR_BLOCK_SZ];
	int cx, cy;
	AABB2 box;
	
	GLuint tex;
	
} TerrainBlock;




void initTerrain(); 
TerrainBlock* allocTerrainBlock(int cx, int cy);
void updateTerrainTexture(TerrainBlock* tb);

void drawTerrainBlock(TerrainBlock* tb, Matrix* mModel, Matrix* mView, Matrix* mProj);


// stuff below is too complicated for now. more knowledge is needed about the game to proceed.


/*
#define MAP_SZ 32


#define MAP_IDX_MASK(x,y) (((1 << 2) << (x)) & (1 << (y)))

struct MapBlock;



typedef struct MapInfo {
	short dataStride;
	
	MapNode root;
}



typedef struct MapNode {
	
	
	unsigned char level;
	unsigned char usage;
	
	int x, y; // center coords
	struct MapNode* parent;
	
	union {
		struct MapNode* kids[2][2];
		struct MapBlock* blocks[2][2];
	};
} MapNode;


typedef struct MapBlock {
	void* data;
	int cx, cy;
	int counter;
	struct MapBlock* xp, *xm, *yp, *ym;
} MapBlock;




// MapBlock* allocRootMapNode(short stride);







void printMapMemoryStats();

*/

#endif // __map_h__