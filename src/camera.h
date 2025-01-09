#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
    vec3 pos;
    vec3 front;
    vec3 right;
    vec3 up;
    f32 pitch;
    f32 yaw;
    vec3 target;
} Camera;

Camera make_camera(vec3 pos, f32 pitch, f32 yaw);
mat4 camera_get_view_matrix(Camera *cam);
void camera_set_orientation(Camera *cam, f32 pitch, f32 yaw);
void camera_update_vectors(Camera *cam);
void move_camera_along(Camera *cam, vec3 dir, f32 distance);
void move_cam(Camera *cam);

#endif // CAMERA_H
