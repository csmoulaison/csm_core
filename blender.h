#ifndef blender_h_INCLUDED
#define blender_h_INCLUDED

char blender_export_obj_cmd[] = 
    "blender --background --python build/blender_export_obj.py -- %s.blend %s.obj";

char blender_export_bmp_cmd[] = 
    "blender --background --python build/blender_export_bmp.py -- %s.blend %s.bmp";

char blender_export_obj_python[] = 
    "import bpy, sys, pathlib\n"
    "blend_file, obj_file = sys.argv[-2:]\n"
    "bpy.ops.wm.open_mainfile(filepath=blend_file)\n"
    "mesh = bpy.data.objects["mesh"]\n"
    "mesh.select_set(True)\n"
    "bpy.context.view_layer.objects.active = mesh\n"
    "bpy.ops.wm.obj_export(\n"
        "filepath = obj_file,\n"
        "apply_modifiers = True,\n"
        "apply_transform = False,\n"
        "export_selected_objects = True,\n"
        "export_uv = True,\n"
        "export_normals = True,\n"
        "export_materials = False,\n"
        "export_triangulated_mesh = True)\n"

char blender_export_bmp_python[] = 
    "import bpy, sys, pathlib\n"
    "blend_file, bmp_file = sys.argv[-2:]\n"
    "bpy.ops.wm.open_mainfile(filepath=blend_file)\n"
    "workdir = pathlib.Path().resolve()\n"
    "filepath = str(workdir / bmp_file)\n"
    "img = bpy.data.images["ao"]\n"
    "scene = bpy.context.scene\n"
    "original_format = scene.render.image_settings.file_format\n"
    "scene.render.image_settings.file_format = 'BMP'\n"
    "img.save_render(filepath, scene=scene)\n"
    "scene.render.image_settings.file_format = original_format\n";

void blender_export_obj(String src_path, String dst_path);
void blender_export_bmp(String src_path, String dst_path);

#ifdef CSM_IMPLEMENTATION

void blender_export_obj(String src_path, String dst_path) {
    // NOW: Implement
    panic();
}

void blender_export_bmp(String src_path, String dst_path) {
    // NOW: Implement
    panic();
}

#endif
#endif
