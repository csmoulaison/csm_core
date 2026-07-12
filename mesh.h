#ifndef csm_mesh_h_INCLUDED
#define csm_mesh_h_INCLUDED

#ifndef MESH_MAX_VERTICES
#define MESH_MAX_VERTICES 65535
#endif

typedef struct {
	union {
		struct {
			f32 position[3];
			f32 normal[3];
			f32 texture_uv[2];
		};
		f32 data[8];
	};
} MeshVertexData;

typedef struct {
	u32 vertices_len;
	u32 indices_len;
	MeshVertexData vertices[MESH_MAX_VERTICES];
} MeshData;

MeshData mesh_from_obj(char* obj);

#ifdef CSM_IMPLEMENTATION

MeshData mesh_from_obj(char* obj) {
    // TODO: Implement
    return MeshData{};
}

#endif
#endif
