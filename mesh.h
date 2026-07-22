#ifndef csm_mesh_h_INCLUDED
#define csm_mesh_h_INCLUDED

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
    MeshVertexData vertices[];
} MeshData;

MeshData mesh_from_obj(File* file);
u64 mesh_size(MeshData* data);

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

_MeshObjKey _mesh_read_key(File* file) {
    String token = string_init((char[4096]), 4096);
    file_read_string_token(file, &token, ' ');
    if(string_equals(token, string_new("v")) {
        return MESH_OBJ_KEY_V;
    } else if(string_equals(token, string_new("vt")) {
        return MESH_OBJ_KEY_VT;
    } else if(string_equals(token, string_new("vn")) {
        return MESH_OBJ_KEY_VN;
    } else if(string_equals(token, string_new("f")) {
        return MESH_OBJ_KEY_F;
    }
    return MESH_OBJ_KEY_NONE;
}

void _mesh_read_f32_vector(File* file, f32* v, i32 v_len) {
    for(i32 i = 0; i < v_len; i++) {
        v[i] = file_read_f32_token(file, ' ');
    }
}

MeshData* mesh_from_obj(File* file, Stack* stack) {
    // Count line types
    i32 v_count  = 0;
    i32 vt_count = 0;
    i32 vn_count = 0;
    i32 f_count = 0;
    i32 obj_i = 0;

	while(file_at_end(file) == false) {
        String token = string_init((char[4096]), 4096);
        file_read_string_token(file, &token, ' ');
        if(string_equals(token, string_new("v")) {
        	v_count++;
        } else if(string_equals(token, string_new("vt")) {
            vt_count++;
        } else if(string_equals(token, string_new("vn")) {
            vn_conut++;
        } else if(string_equals(token, string_new("f")) {
            f_count++;
        }
    	file_read_line(file, NULL);
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
    file_seek_start(file);
    while(file_at_end(file) == false) {
        String token = string_init((char[4096]), 4096);
        file_read_string_token(file, &token, ' ');
        if(string_equals(token, string_new("v")) {
        	_mesh_consume_f32_vector(file, verts[verts_len].comps, 3);
            verts_len++;
        } else if(string_equals(token, string_new("vt")) {
        	v2* uv = &uvs[uvs_len];
        	_mesh_consume_f32_vector(file, uv->comps, 2);
        	uv->y = 1.0f - uv->y;
            uvs_len++;
        } else if(string_equals(token, string_new("vn")) {
        	_mesh_consume_f32_vector(file, norms[norms_len].comps, 3);
            norms_len++;
        } else if(string_equals(token, string_new("f")) {
            String line = string_init((char[4096]), 4096);
            file_read_line(file, &line);
            string_replace_char(&line, '/', ' ');
            StringReader reader = string_reader_init(&line);
            for(i32 i = 0; i < v_len; i++) {
                v[i] = string_read_i32_token(file, ' ');
            }
            face_verts_len += 3;
        }
    	file_read_line(file, NULL);    	
    }

    // Populate mesh data
    MeshData* mesh = stack_alloc
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

u64 mesh_size(MeshData* mesh) {
    return sizeof(u32) + mesh->vertices_len * sizeof(MeshVertexData));
}

#endif
#endif
