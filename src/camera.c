Camera make_camera(vec3 pos, f32 pitch, f32 yaw) {
    Camera result;
    result.pos = pos;
    result.pitch = pitch;
    result.yaw = yaw;
    return result;
}

mat4 camera_get_view_matrix(Camera *cam) {
    // calculate look at matrix
    camera_update_vectors(cam);
    return _m4_look_at(cam->front, cam->right, cam->up, cam->pos);
}

void camera_set_orientation(Camera *cam, f32 pitch, f32 yaw) {
    if (pitch > 89.9f) {
        pitch = 89.9f;
    } 
    if (pitch < -89.9f) {
        pitch = -89.9f;
    }

    cam->pitch = pitch;
    cam->yaw   = yaw;
}

void camera_update_vectors(Camera *cam) {
    f32 r_pitch = to_radians(cam->pitch);
    f32 r_yaw = to_radians(cam->yaw);
    vec3 target_dir = v3_norm(vec3(
        cosf(r_pitch) * sinf(r_yaw),
        sinf(r_pitch),
        cosf(r_pitch) * cosf(r_yaw)
    ));

    f32 distance = v3_len(v3_sub(cam->pos, cam->target));
    cam->pos = v3_add(cam->target, v3_mul_f32(target_dir, distance));
    cam->front = v3_mul_f32(target_dir, -1.0f);
    cam->right = v3_norm(v3_cross(cam->front, vec3(0.0f, 1.0f, 0.0f)));
    cam->up    = v3_norm(v3_cross(cam->right, cam->front));
}

void move_camera_along(Camera *cam, vec3 dir, f32 distance) {
    dir.y = 0; // project onto world plane
    dir = v3_mul_f32(v3_norm(dir), distance);

    cam->pos    = v3_add(cam->pos, dir);
    cam->target = v3_add(cam->target, dir);
}

void move_cam(Camera *cam) {
    f32 delta = get_frame_time();
    f32 mouse_sens = 10.0f * delta;

    if (is_mouse_button_down(GLFW_MOUSE_BUTTON_LEFT)) {
        vec2 offset = v2_mul_f32(get_mouse_delta(), mouse_sens);
        camera_set_orientation(cam, cam->pitch + offset.y, cam->yaw - offset.x);
    }

    f32 wheel_y = state.input.mouse.wheel.y;
    f32 zoom_speed = 20.0f * delta;
    cam->pos = v3_add(cam->pos, v3_mul_f32(cam->front, wheel_y * zoom_speed));

    f32 move_speed = 50.0f * delta;
    if (is_key_down(GLFW_KEY_W)) {
        move_camera_along(cam, cam->front, move_speed);
    }
    if (is_key_down(GLFW_KEY_S)) {
        move_camera_along(cam, cam->front, -move_speed);
    }
    if (is_key_down(GLFW_KEY_D)) {
        move_camera_along(cam, cam->right, move_speed);
    }
    if (is_key_down(GLFW_KEY_A)) {
        move_camera_along(cam, cam->right, -move_speed);
    }
    camera_update_vectors(cam);
}
