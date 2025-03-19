#ifndef SLD_WINDOW_H
#define SDL_WINDOW_H

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "camera.h"

typedef struct {
    SDL_Window* sdl_window;
    SDL_GLContext context;
} Window;

Window init_window(const char *title, int width, int height);
void window_poll_events(Window *window, int *running);
void window_swap_buffers(Window *window);
void destroy_window(Window *window);
float get_delta_time(Uint64 *last);
void limit_frame_rate(float delta_time, float target_framerate);
void change_render_mode();
const Uint8* get_button_pressed();

#endif