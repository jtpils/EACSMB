#ifndef __EACSMB_SCENE_H__
#define __EACSMB_SCENE_H__

#include "pass.h"

/*
typedef struct SceneItemInfo {
	AABB2 aabb;
	
	uint32_t selectable : 1;
	
	uint32_t eid;
	void* data;
} SceneItemInfo;


typedef struct QuadTreeNode {
	AABB2 aabb;
	int level;
	VEC(SceneItemInfo*) items;
	
	struct QuadTreeNode* parent;
	struct QuadTreeNode* kids[2][2];
} QuadTreeNode;


typedef struct QuadTree {
	int maxLevels;
	int nodeMaxCount; // trigger to subdivide
	int nodeMinCount; // trigger to recombine
	
	int totalCount;
	
	
	QuadTreeNode* root;
} QuadTree;*/




typedef struct Scene {
	
// 	Pass* solids;
// 	Pass* decals;
// 	Pass* lighting;
// 	
	VEC(PassDrawable*) allDrawables;
	
} Scene;


void Scene_init(Scene* sc);




/*


QuadTreeNode* QTNode_alloc(QuadTreeNode* parent, char ix, char iy);
void QTNode_init(QuadTreeNode* n, QuadTreeNode* parent, char ix, char iy);
void QuadTree_insert(QuadTree* qt, SceneItemInfo* info);


*/




#endif // __EACSMB_SCENE_H__
