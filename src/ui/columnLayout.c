
#include "stdlib.h"
#include "string.h"




#include "../gui.h"





void guiColumnLayoutRender(GUIColumnLayout* cl, GameState* gs, PassFrameParams* pfp);





GUIColumnLayout* guiColumnLayoutNew(Vector2 pos, float spacing, float zIndex) {
	
	GUIColumnLayout* w;
	
	static struct gui_vtbl static_vt = {
		.Render = guiColumnLayoutRender,
	};
	
	
	w = calloc(1, sizeof(*w));
	CHECK_OOM(w);
	
	guiHeaderInit(&w->header);
	w->header.vt = &static_vt;
	
	w->spacing = spacing;
	
	// empty layout has no size
	w->header.hitbox.min.x = 0;
	w->header.hitbox.min.y = 0;
	w->header.hitbox.max.x = 0;
	w->header.hitbox.max.y = 0;
	
	w->header.topleft = pos;
	
	return w;
}



// TODO: re-sort







void guiColumnLayoutRender(GUIColumnLayout* cl, GameState* gs, PassFrameParams* pfp) {
	
// 	guiRender(ed->bg, gs, pfp);
// 	guiRender(ed->textControl, gs, pfp);
	
	// adjust positions based on size
	float total_h = 0.0;
	VEC_EACH(&cl->header.children, i, child) {
		total_h += cl->spacing;
		
		child->h.topleft.x = cl->header.topleft.x;
		child->h.topleft.y = cl->header.topleft.y + total_h;
		
		guiRender(child, gs, pfp);
		
		total_h += child->h.size.y;
	}
	
// 	int n = VEC_LEN(&cl->header);
	
	
	
}

void guiColumnLayoutDelete(GUIColumnLayout* o) {
	
}









