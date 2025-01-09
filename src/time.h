#ifndef TIME_H
#define TIME_H

void update_time();
f32 get_time();
f32 get_time_delta();
f32 get_frame_time();
void set_max_fps(f32 fps);
b32 should_redraw();
void reset_frame_time();

#endif // TIME_H
