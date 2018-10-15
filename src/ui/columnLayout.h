#ifndef __EACSMB_ui_columnLayout_h__
#define __EACSMB_ui_columnLayout_h__






typedef struct GUIColumnLayout {
	GUIHeader header;
	
	GUIWindow* bg; // optional
	
	float spacing;
	
	
} GUIColumnLayout;

GUIColumnLayout* guiColumnLayoutNew(Vector2 pos, float spacing, float zIndex);




#endif // __EACSMB_ui_columnLayout_h__
