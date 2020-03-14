#ifndef C3D_MATH_H
#define C3D_MATH_H

#include <f3d/engine/types.h>

#define C3D_PI 3.1415926535

inline float math_deg_to_rad(float degrees) {
    return (degrees*C3D_PI/180);
}

mat4_t math_lookat(vector3f_t from, vector3f_t to, vector3f_t upvec);
void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar);
void math_frustum_mat4(mat4_t *mat, float left, float right, float bottom, float top, float znear, float zfar);

#endif