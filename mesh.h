#ifndef csm_mesh_h_INCLUDED
#define csm_mesh_h_INCLUDED

// TODO: mesh.h is a great playground for a sorely needed string library.

#ifndef MESH_MAX_VERTICES
#define MESH_MAX_VERTICES 65535
#endif

typedef struct {
    union {
        struct {
            v3 position;
            v3 normal;
            v2 uv;
        };
        f32 data[8];
    };
} MeshVertexData;

typedef struct {
    u32 vertices_len;
    MeshVertexData vertices[MESH_MAX_VERTICES];
} MeshData;

MeshData mesh_from_obj(char* obj);

#ifdef CSM_IMPLEMENTATION

typedef struct {
    union {
        struct {
            i32 position;
            i32 uv;
            i32 normal;
        };
        i32 data[3];
    };
} _MeshObjFaceVertex;

typedef enum {
    MESH_OBJ_KEY_NONE,
    MESH_OBJ_KEY_V,
    MESH_OBJ_KEY_VT,
    MESH_OBJ_KEY_VN,
    MESH_OBJ_KEY_F
} _MeshObjKey;

_MeshObjKey _mesh_consume_key(char* obj, i32* obj_i) {
    if(obj[*obj_i] == 'v') {
        if(obj[*obj_i + 1] == ' ') {
            *obj_i += 2;
            return MESH_OBJ_KEY_V;
        } else if(obj[*obj_i + 1] == 't') {
            *obj_i += 3;
            return MESH_OBJ_KEY_VT;
        } else if(obj[*obj_i + 1] == 'n') {
            *obj_i += 3;
            return MESH_OBJ_KEY_VN;
        }
    } else if(obj[*obj_i] == 'f' && obj[*obj_i + 1] == ' ') {
            *obj_i += 2;
        return MESH_OBJ_KEY_F;
    }
    return MESH_OBJ_KEY_NONE;
}

void _mesh_finish_obj_line(char* obj, i32* obj_i) {
    while(obj[*obj_i] != '\0') {
        if(obj[*obj_i] == '\n') {
            *obj_i += 1;
            return;
        }
        *obj_i += 1;
    }
}

void _mesh_consume_token(char* obj, i32* obj_i, char* buf) {
    while(obj[*obj_i] == ' ') {
        *obj_i += 1;
    }
    i32 si = 0;
    while(obj[*obj_i] != ' ' && obj[*obj_i] != '\n' && obj[*obj_i] != '\0') {
        buf[si] = obj[*obj_i];
        si++;
        *obj_i += 1;
    }
    buf[si] = '\0';
}

void _mesh_consume_f32_vector(char* obj, i32* obj_i, f32* v, i32 v_len) {
    for(i32 i = 0; i < v_len; i++) {
        char s[32];
        _mesh_consume_token(obj, obj_i, s);
        char* end_ptr;
        f32 n = strtof(s, &end_ptr);
        if(s == end_ptr) {
            fprintf(stderr, "Error reading f32 in obj file.");
            panic();
        }
        v[i] = n;
    }
}

void _mesh_consume_i32_vector(char* obj, i32* obj_i, i32* v, i32 v_len) {
    for(i32 i = 0; i < v_len; i++) {
        char s[32];
        _mesh_consume_token(obj, obj_i, s);
        char* end_ptr;
        i32 n = strtol(s, &end_ptr, 10);
        if(s == end_ptr) {
            fprintf(stderr, "Error reading i32 in obj file.");
            panic();
        }
        v[i] = n;
    }
}

MeshData mesh_from_obj(char* obj) {
    // Count line types
    i32 v_count  = 0;
    i32 vt_count = 0;
    i32 vn_count = 0;
    i32 f_count = 0;
    i32 obj_i = 0;

	while(obj[obj_i] != '\0') {
    	_MeshObjKey key = _mesh_consume_key(obj, &obj_i);
    	switch(key) {
        	case MESH_OBJ_KEY_V:  
            	v_count++; break;
        	case MESH_OBJ_KEY_VT: 
            	vt_count++; break;
        	case MESH_OBJ_KEY_VN: 
            	vn_count++; break;
        	case MESH_OBJ_KEY_F:  
            	f_count++; break;
        	default: break;
    	}
    	_mesh_finish_obj_line(obj, &obj_i);
    }

    // Allocate line data buffers
    u64 vert_bytes = v_count  * sizeof(v3);
    u64 uv_bytes   = vt_count * sizeof(v2);
    u64 norm_bytes = vn_count * sizeof(v3);
    u64 face_bytes = f_count  * 3 * sizeof(_MeshObjFaceVertex);
    Buffer buffer = buffer_malloc(
        vert_bytes + uv_bytes + norm_bytes + face_bytes, "Global");
    Stack stack = stack_init(buffer.memory, buffer.size, "Global");

    v3* verts = (v3*)(stack_alloc(&stack, vert_bytes));
    i32 verts_len = 0;
    v2* uvs = (v2*)(stack_alloc(&stack, uv_bytes));
    i32 uvs_len = 0;
    v3* norms = (v3*)(stack_alloc(&stack, norm_bytes));
    i32 norms_len = 0;
    _MeshObjFaceVertex* face_verts = (_MeshObjFaceVertex*)(stack_alloc(&stack, face_bytes));
    i32 face_verts_len = 0;

    // Read/store line data 
    obj_i = 0;
    while(obj[obj_i] != '\0') {
    	_MeshObjKey key = _mesh_consume_key(obj, &obj_i);
    	switch(key) {
        	case MESH_OBJ_KEY_V: {
            	_mesh_consume_f32_vector(obj, &obj_i, verts[verts_len].comps, 3);
                verts_len++;
            } break;
        	case MESH_OBJ_KEY_VT: {
            	v2* uv = &uvs[uvs_len];
            	_mesh_consume_f32_vector(obj, &obj_i, uv->comps, 2);
            	uv->y = 1.0f - uv->y;
                uvs_len++;
            } break;
        	case MESH_OBJ_KEY_VN: {
            	_mesh_consume_f32_vector(obj, &obj_i, norms[norms_len].comps, 3);
                norms_len++;
            } break;
        	case MESH_OBJ_KEY_F: {
            	i32 tmp_i = obj_i;
            	while(obj[tmp_i] != '\n' && obj[tmp_i] != '\0') {
            		if(obj[tmp_i] == '/') obj[tmp_i] = ' ';
            		tmp_i++;
            	}
            	_mesh_consume_i32_vector(obj, &obj_i, face_verts[face_verts_len].data, 9);
                face_verts_len += 3;
            } break;
            default: break;
    	}
    	_mesh_finish_obj_line(obj, &obj_i);
    }

    // Populate mesh data
    MeshData mesh;
    mesh.vertices_len = face_verts_len; // = verts_len in indexed case
    // indices_len would go here
     
	for(i32 i = 0; i < face_verts_len; i++) {
    	_MeshObjFaceVertex face_vert = face_verts[i];
    	i32 vert_index = face_vert.position - 1;
    	i32 uv_index   = face_vert.uv - 1;
    	i32 norm_index = face_vert.normal - 1;

        // For flat shaded, not indexed
        MeshVertexData* mesh_vert = &mesh.vertices[i];
        mesh_vert->position = verts[vert_index];
        mesh_vert->uv = uvs[uv_index];
        mesh_vert->normal = norms[norm_index];
	}

	buffer_free(&buffer);
    return mesh;
}

#endif
#endif
