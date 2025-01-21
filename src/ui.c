void ui_update() {
    struct nk_context *ctx = state.window.nk_ctx;

    static char *items[] = {"ParticleContainer", "SoAContainer", "LinkedCellContainer", "ParallelLCContainer"};
    if (nk_begin(ctx, "Controls", nk_rect(50, 50, 230, 350),
                 NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                 NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {

        nk_layout_row_dynamic(ctx, 25, 1);
        if (nk_combo_begin_label(ctx, items[state.sim.type], nk_vec2(nk_widget_width(ctx), 200))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            for (int i = 0; i < 4; ++i)
                if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT)) {
                    state.sim.type = i;
                    state.sim.problem.changed = true;
                }
            nk_combo_end(ctx);
        }

        nk_layout_row_static(ctx, 30, 80, 1);
        if (!state.sim.playing) {
            if (nk_button_label(ctx, "Play")) {
                state.sim.playing = true;
                state.sim.timer = 0.0f;
            }
        } else {
            if (nk_button_label(ctx, "Pause")) {
                state.sim.playing = false;
            }
        }

        f32 step_time = 1.0f / state.sim.steps_per_second;
        if (state.sim.timer > step_time) {
            sim_step();
            state.sim.geometry.changed = true;
            state.sim.timer = 0.0f;
        }

        nk_layout_row_dynamic(ctx, 25, 1);

        nk_value_float(ctx, "Frame time", state.time.frame_dt * 1000);
        if (nk_property_float(ctx, "dt:", -10.0f, &state.sim.integrator.dt, 10.0f, 0.1f, 0.1f)) state.sim.integrator.changed = true;
        nk_label(ctx, "Problem", NK_TEXT_LEFT);
        if (nk_property_int(ctx, "x:", 0, &state.sim.problem.n.x, 1000, 1, 1)) state.sim.problem.changed = true;
        if (nk_property_int(ctx, "y:", 0, &state.sim.problem.n.y, 1000, 1, 1)) state.sim.problem.changed = true;
        if (nk_property_int(ctx, "z:", 0, &state.sim.problem.n.z, 1000, 1, 1)) state.sim.problem.changed = true;
    }
    nk_end(ctx);
}
