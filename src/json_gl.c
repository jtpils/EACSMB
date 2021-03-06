#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h> 

#include "c_json/json.h"
#include "json_gl.h" 
#include "hash.h" 

static HashTable* lookup;

struct enum_data {
	char* k;
	GLenum v;
};

#define gl_enum_data(str, macro, val) {str, val},
// #define gl_enum_data(str, macro, val) {str, macro},
static struct enum_data enum_values_for_gl[] = {
	#include "gl_enum_data.c" 
	{0,0}
};



void json_gl_init_lookup() {
	struct enum_data* d;
	
	if(lookup) return;
	lookup = HT_create(13); // 8192; there are 5671 enums
	
	d = enum_values_for_gl;
	
	fprintf(stderr, "initializing GLenum lookup...");
	while(d->k) {
		HT_set(lookup, d->k, d->v);
		d++;
	}
	fprintf(stderr, " done.\n");
}




int json_as_GLenum(struct json_value* v, GLenum* out) {
	
	switch(v->type) { // actual type
		case JSON_TYPE_UNDEFINED:
		case JSON_TYPE_NULL:
			*out = 0;
			return 1;
			
		case JSON_TYPE_INT: // treat it as a literal value
			*out = v->v.integer;
			return 0;
			
		case JSON_TYPE_DOUBLE:
			*out = v->v.dbl;  // treat it as a literal value, converted to int
			return 0;
			
		case JSON_TYPE_STRING: // look up the enum
			return HT_get(lookup, v->v.str, out);
			
			
		case JSON_TYPE_OBJ: // all invalid
		case JSON_TYPE_ARRAY:
		case JSON_TYPE_COMMENT_SINGLE:
		case JSON_TYPE_COMMENT_MULTI:
		default:
			*out = 0;
			return 1;
	}
}


static float index_as_float(json_value_t* obj, int index, float def) {
	json_value_t* v;
	float f;
	int ret;
	
	if(!json_as_float(v, &f)) {
		return def;
	}
	
	return f;
}

static float key_as_float(json_value_t* obj, char* key, float def) {
	json_value_t* v;
	float f;
	int ret;
	
	if(!json_obj_get_key(obj, key, &v)) {
		return def;
	}
	
	if(!json_as_float(v, &f)) {
		return def;
	}
	
	return f;
}



int json_obj_key_as_vector(struct json_value* obj, char* key, int len, float* out, float* defaults) {
	json_value_t* v;
	
	for(int c = 0; c < len; c++) {
		out[c] = defaults[c];
	}
	
	if(!json_obj_get_key(obj, key, &v)) {
		return 1;
	}
	
	json_as_vector(v, len, out);
	
	return 0;
}



// probably broken
int json_as_vector(struct json_value* v, int max_len, float* out) {
	int i;
	float f;
	struct json_array_node* an;
	
	if(v->type == JSON_TYPE_OBJ) {
		out[0] = key_as_float(v, "x", 0.0);
		out[1] = key_as_float(v, "y", 0.0);
		out[2] = key_as_float(v, "z", 0.0);
		out[3] = key_as_float(v, "w", 1.0);
		
		// get x/y/z/w, r/g/b/a
	}
	else if(v->type == JSON_TYPE_ARRAY) {
		// [1,2,3,4]
		an = v->v.arr->head;
		i = 0;
		while(an) {
			json_as_float(an->value, &f);
			out[i++] = f;
			an = an->next;
		}
		
		while(i < max_len - 1) out[i++] = 0.0f;
		if(max_len == 4 && i == 3) out[3] = 1.0f;
	}
	else if(v->type == JSON_TYPE_DOUBLE) {
	// duplicate numbers to every field 
		json_as_float(v, &f);
		for(i = 0; i < max_len; i++) out[i] = f;
	}
	
	return 0;
}



int json_vector_array(struct json_value* j, int components, float* defaults, float** out, int* outLen) {
	float* o;
	int i = 0;
	int olen = 0;
	struct json_array_node* link;
	
	olen = j->v.arr->length;
	o = malloc(olen * sizeof(*o) * components);
	
	
	link = j->v.arr->head;
	while(link) {
		json_value_t* jv;
		
		for(int c = 0; c < components; c++) {
			o[i * components + c] = defaults[c];
		}
		
		json_as_vector(jv, components, o + (i * components));
		
		jv = link->value;
		i++;
	}
	
	*out = o;
	*outLen = olen;
	
	return 0;
}



static int get_minmax(struct json_value* v, struct json_value** min, struct json_value** max) {
		
	if(v == NULL) return 99;
	
	// find the right objects
	if(v->type == JSON_TYPE_OBJ) {
		if(json_obj_get_key(v, "min", min)) return 1;
		if(json_obj_get_key(v, "max", max)) return 2;
	}
	else if(v->type == JSON_TYPE_ARRAY) {
		if(v->v.arr->length < 2) {
			fprintf(stderr, "json_get_minmax: array input with less than 2 children\n");
			return 3;
		}
		*min = v->v.arr->head->value;
		*max = v->v.arr->head->next->value;
	}
	
	return 0;
}


int json_vector3_minmax(struct json_value* v, Vector* min, Vector* max) {
	int ret;
	struct json_value* j_min, *j_max;
	
	if(ret = get_minmax(v, &j_min, &j_max)) return ret;
		
	// grab the values
	json_as_vector(j_min, 3, (float*)min);
	json_as_vector(j_max, 3, (float*)max);
	
	return 0;
}

int json_double_minmax(struct json_value* v, double* min, double* max) {
	int ret;
	struct json_value* j_min, *j_max;
	
	if(ret = get_minmax(v, &j_min, &j_max)) return ret;
		
	// grab the values
	json_as_double(j_min, min);
	json_as_double(j_max, max);
	
	return 0;
}




int json_as_type_gl(struct json_value* v, enum json_type_gl t, void* out) {
	printf("!!! %s.%d: %s not implemented!\n", __FILE__, __LINE__, __func__);
	switch(t) {
		
	
	}
}
