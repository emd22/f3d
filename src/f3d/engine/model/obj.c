#include <f3d/engine/model/obj.h>
#include <f3d/engine/model/mtl.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/handles.h>

#include <f3d/engine/type/buffer.h>
#include <f3d/engine/type/vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

obj_model_t obj_load(const char *path) {
    // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    obj_model_t model;
    model.inited = 0;
    
    buffer_init(&model.vertices, sizeof(vector3f_t), 4096);
    buffer_init(&model.uvs,      sizeof(vector2f_t), 4096);
    buffer_init(&model.normals,  sizeof(vector3f_t), 4096);
    
    buffer_t temp_vertices;
    buffer_t temp_uvs;
    buffer_t temp_normals;
    buffer_init(&temp_vertices, sizeof(vector3f_t), 4096);
    buffer_init(&temp_uvs,      sizeof(vector2f_t), 4096);
    buffer_init(&temp_normals,  sizeof(vector3f_t), 4096);
    
    buffer_t vertex_indices, uv_indices, normal_indices;
    buffer_init(&vertex_indices, sizeof(unsigned), 8192);
    buffer_init(&uv_indices,     sizeof(unsigned), 8192);
    buffer_init(&normal_indices, sizeof(unsigned), 8192);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Could not load '%s'\n", path);
        return model;
    }
    // TODO: up line buffer size, use non infinite loop
    char line[128];
    unsigned line_number = 0;
    int res;
    
    for (;; line_number++) {
        res = fscanf(fp, "%s", line); 
        if (res == EOF)
            break;
        if (!strcmp(line, "v")) {
            vector3f_t vertex;
            fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            buffer_push(&temp_vertices, &vertex);
        }
        else if (!strcmp(line, "vt")) {
            vector2f_t uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            buffer_push(&temp_uvs, &uv);
        }
        else if (!strcmp(line, "vn")) {
            vector3f_t normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            buffer_push(&temp_normals, &normal);
        }
        else if (!strcmp(line, "f")) {
            unsigned vertex_index[4] = { 0 },
                uv_index[4] = { 0 },
                normal_index[4] = { 0 };
            char newl[128];
            fgets(newl, 128, fp);
            int matches = sscanf(
                newl, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
                &vertex_index[0], &uv_index[0], &normal_index[0],
                &vertex_index[1], &uv_index[1], &normal_index[1],
                &vertex_index[2], &uv_index[2], &normal_index[2]
            );
            int i;
            
            for (i = 0; i < 3; i++) {
                vertex_index[i]--;
                uv_index[i]--;
                normal_index[i]--;
            }
            
            if (matches != 9) {
                log_msg(LOG_WARN, "model not using vert 9\n", 0);
                uv_index[0] = 0;
                uv_index[1] = 0;
                uv_index[2] = 0;
                matches = sscanf(
                    newl, "%u//%u %u//%u %u//%u\n",
                    &vertex_index[0], &normal_index[0],
                    &vertex_index[1], &normal_index[1],
                    &vertex_index[2], &normal_index[2]
                );
                if (matches != 6) {
                    log_msg(LOG_ERROR, "Model not supported\n", 0);
                    // TODO: fail gracefully
                    exit(1);                
                }
            }
            for (i = 0; i < 3; i++) {
                buffer_push(&vertex_indices, &vertex_index[i]);
                buffer_push(&uv_indices,     &uv_index[i]);
                buffer_push(&normal_indices, &normal_index[i]);
            }
        }
        else if (!strcmp(line, "mtllib")) {
            char mtlpath[64];
            strcpy(mtlpath, "../models/");
            
            char temp[32];
            fscanf(fp, "%s\n", temp);
            strcat(mtlpath, temp);
            
            //mtl_load(mtlpath);
        }
        else {
            char fut[2048];
            fgets(fut, 2048, fp);
        }
    }
    /*unsigned i;
    for (i = 0; i < vertex_indices.index; i++) {
        unsigned vert_index = ((unsigned *)vertex_indices.data)[i];
        unsigned uv_index   = ((unsigned *)uv_indices.data)[i];
        unsigned norm_index = ((unsigned *)normal_indices.data)[i];
        vector3f_t vertex = ((vector3f_t *)temp_vertices.data)[vert_index-1];
        vector2f_t uv     = ((vector2f_t *)temp_uvs.data)[uv_index-1];
        vector3f_t normal = ((vector3f_t *)temp_normals.data)[norm_index-1];
        buffer_push(&model.vertices, &vertex);
        buffer_push(&model.uvs, &uv);
        buffer_push(&model.normals, &normal);    
    }*/
    //vector3f_t vertex;
    //log_msg(LOG_INFO, "vertices... %llu, %llu\n", vertex_indices.index, vertex_indices.size);
    //for (i = 0; i < vertex_indices.index; i++) {
        //int vertex_index = ((unsigned *)vertex_indices.data)[i];
        //vertex = ((vector3f_t *)temp_vertices.data)[vertex_index-1];
        //buffer_push(&model.vertices, &vertex);
    //}
    model.vertices = temp_vertices;
    //unsigned i;
    //log_msg(LOG_INFO, "VERTS: %u, INDS: %u\n", model.vertices.index, vertex_indices.index);
    //buffer_destroy(&temp_vertices);
    //buffer_destroy(&vertex_indices);
    //vector2f_t uv;
    //for (i = 0; i < uv_indices.index; i++) {
    //    unsigned uv_index = ((unsigned *)uv_indices.data)[i];

    //    uv = ((vector2f_t *)temp_uvs.data)[uv_index-1];
    //    buffer_push(&model.uvs, &uv);
    //}
    model.uvs = temp_uvs;
    //log_msg(LOG_DEBUG, "VERTICES: %d, INDICES: %d\n", model.vertices.index, model.vertex_indices.index);
    //buffer_destroy(&temp_uvs);
    //buffer_destroy(&uv_indices);
    //log_msg(LOG_INFO, "destroying buffers... %llu\n", buffer_total_used);
    
    //vector3f_t normal;
    //for (i = 0; i < normal_indices.index; i++) {
    //    unsigned normal_index = ((unsigned *)normal_indices.data)[i];
    //    normal = ((vector3f_t *)temp_normals.data)[normal_index-1];
    //    buffer_push(&model.normals, &normal);
    //}
    //buffer_destroy(&temp_normals);    
    //buffer_destroy(&normal_indices);
    model.normals = temp_normals;
    fclose(fp);
    model.inited = 1;
    return model;
}

void obj_destroy(obj_model_t *model) {
    if (!model->inited)
        return;
    // vertex
    buffer_destroy(&model->vertices);
    
    buffer_destroy(&model->uvs);
    buffer_destroy(&model->normals);
}
