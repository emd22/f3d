#ifndef F3D_SCENE_H
#define F3D_SCENE_H

#include <f3d/engine/rendering/render_view.h>
#include <f3d/engine/rendering/skybox.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/light.h>
#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/object/object.h>
#include <f3d/engine/limits.h>

#define SCENE_ENABLE_SHADOWS 0x01
#define SCENE_ENABLE_SKYBOX  0x02

typedef enum {
    SCENE_SKYBOX,
    SCENE_LIGHT,
    SCENE_OBJECT,
} scene_attach_type_t;

typedef struct {
    char name[32];
    
    object_buffer_t objects;
    
    light_t *lights[MAX_SCENE_LIGHTS];
    int lights_index;
    
    skybox_t skybox;
    int flags;
} scene_t;

extern scene_t scenes[MAX_SCENES];
extern scene_t *selected_scene;
extern int scenes_index;

scene_t *scene_new(const char *name);
void scene_render_shadows(scene_t *scene, shader_t *shader_main);
void scene_objects_render(scene_t *scene, shader_t *shader, camera_t *camera, bool render_materials);
void scene_select(scene_t *scene, shader_t *shader_main);
render_view_t *scene_new_view(scene_t *scene, camera_t *camera, int width, int height, int attachment);
void scene_object_update(scene_t *scene, object_t *object, shader_t *shader_main);
void scene_attach(scene_t *scene, scene_attach_type_t type, void *ptr);
void scene_render(shader_t *shader_main, scene_t *scene);
void scene_destroy(scene_t *scene);

#endif
