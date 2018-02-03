

#include "render.h" 



/*

	MeshData* md;
	MB_operation* root;
	
	
	root = read_json("assets/models/test.json");
	if(!root) {
		printf("failed to read test json file\n");
		exit(1);
	}
	
	md = process_op(root);
	if(!md) {
		printf("failed to process mesh operations \n");
		exit(1);
	}
*/




void updateMeshManager() {
	/*

	sm = calloc(1, sizeof(*sm));
	CHECK_OOM(sm);

	sm->vertexCnt = VEC_LEN(&md->verts);
	sm->indexCnt = VEC_LEN(&md->indices);
	sm->indexWidth = 2;
	
	printf("verts: %d, indices: %d \n", sm->vertexCnt, sm->indexCnt);
	
	
	sm->vertices = malloc(sizeof(*sm->vertices) * sm->vertexCnt);
	CHECK_OOM(sm->vertices);
	sm->indices.w16 = malloc(sizeof(*sm->indices.w16) * sm->indexCnt);
	CHECK_OOM(sm->indices.w16);

	for(i = 0; i < sm->vertexCnt; i++) sm->vertices[i] = *((StaticMeshVertex*)&VEC_ITEM(&md->verts, i));
	for(i = 0; i < sm->indexCnt; i++) sm->indices.w16[i] = VEC_ITEM(&md->indices, i);
	
	
	
	StaticMesh_updateBuffers(sm);
	*/
}
void guiBuilderControlRender(GUIBuilderControl* bc, GameState* gs) {
	bc->rt->texID = RenderPipeline_getOutputTexture(bc->rpipe);
	
	guiRender(bc->rt, gs);
	
	//printf("sdf\n");
}

void guiBuilderControlDelete(GUIBuilderControl* rt) {
	//RenderPipeline_destroy(rt->rpl);
	//free(rt->rpl);
}


void guiBuilderControlResize(GUIBuilderControl* bc, Vector2 newSz) {
	
	guiResize(bc->bg, newSz);
//	guiResize(bc->rt, newSz);
}

void geom_pass_render(MeshManager* mm, PassDrawable* pd, PassDrawParams* dp) {
	
	meshManager_draw(mm, dp->mWorldView, dp->mViewProj);
	
}



GUIBuilderControl* guiBuilderControlNew(Vector2 pos, Vector2 size, int zIndex) {
	
	GUIBuilderControl* bc;
	
	static struct gui_vtbl static_vt = {
		.Render = guiBuilderControlRender,
		.Delete = guiBuilderControlDelete,
		.Resize = guiBuilderControlResize
	};
	
	
	bc = calloc(1, sizeof(*bc));
	CHECK_OOM(bc);
	
	guiHeaderInit(&bc->header);
	bc->header.vt = &static_vt;
	
	bc->header.hitbox.min.x = pos.x;
	bc->header.hitbox.min.y = pos.y;
	bc->header.hitbox.max.x = pos.x + size.x;
	bc->header.hitbox.max.y = pos.y + size.y;
	
	bc->header.topleft = pos;
	bc->header.size = size;
	bc->header.z = 0;
	
	
	bc->bg = guiWindowNew(
		(Vector2){pos.x, pos.y}, 
		(Vector2){size.x, size.y}, 
		zIndex + .0001
	);
	//bc->bg = (Vector){0.9, 0.1, .9};
	guiRegisterObject(bc->bg, bc);
	
	
	
	// set up the pipeline
	RenderPipeline* rpipe = calloc(1, sizeof(*rpipe));
	RenderPipeline_init(rpipe);
	bc->rpipe = rpipe;
	
	RenderPass* pass;
	
	// geometry pass
	pass = calloc(1, sizeof(*pass));
	pass->clearColor = 1;
	pass->clearDepth = 1;
	
	pass->fboIndex = 0;
	
	RenderPass_init(pass, loadCombinedProgram("dynamicMeshInstanced"));
	
	bc->mm = meshManager_alloc();
	meshManager_readConfigFile(bc->mm, "assets/config/models.json");
	meshManager_updateGeometry(bc->mm);
	StaticMeshInstance smi[] = {
		{
			{1,1,4}, 2.5,
			{1, 0, 0}, 0.0,
			.9, 0,0,0
		}
	};
	meshManager_addInstance(bc->mm, 0, &smi[0]);
	meshManager_updateInstances(bc->mm);
	
	PassDrawable* br1 = calloc(1, sizeof(*br1));
	br1->draw = geom_pass_render;
	br1->data = bc->mm;
	
	VEC_PUSH(&pass->drawables, br1);
	
	VEC_PUSH(&rpipe->passes, pass);
	
	
	RenderPipeline_addShadingPass(rpipe, "builderShading"); 
	
	rpipe->clearColor = (Vector4){0.1, 0.1, 0.1, 1};
	
	//giTest->texIndex = -1;
	//Builder_initFBOs(rpipe);
	
	
	bc->rt = guiRenderTargetNew(pos, size, rpipe);
	guiRegisterObject(bc->rt, bc->bg);
	
	
	return bc;
}





