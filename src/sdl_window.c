#include "sdl_window.h"

Window init_window(const char *title, int width, int height) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Window window;

    window.sdl_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    SDL_SetRelativeMouseMode(SDL_TRUE);
    window.context = SDL_GL_CreateContext(window.sdl_window);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    return window;
}

void window_poll_events(Window *window, int *running, Camera *camera) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = 0;
        }
        if (event.type == SDL_MOUSEMOTION) {
            handle_mouse_input(camera, event.motion.xrel, event.motion.yrel);
        }
    }
}

void window_swap_buffers(Window *window) {
    SDL_GL_SwapWindow(window->sdl_window);
}

void destroy_window(Window *window) {
    SDL_GL_DeleteContext(window->context);
    SDL_DestroyWindow(window->sdl_window);
}

float get_delta_time(Uint64 *last) {
    Uint64 now = SDL_GetPerformanceCounter();
    float delta_time = (float)((now - *last) / (float)SDL_GetPerformanceFrequency());
    *last = now;
    return delta_time;
}

// Doesnt work correctly
void limit_frame_rate(float delta_time, float target_framerate) {
    float target_frame_time = 1000.0f / target_framerate;
    if (delta_time < target_frame_time) {
        SDL_Delay((Uint64)((target_frame_time - delta_time) * 1000.0f));
    }
}

void change_render_mode() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_1])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (keystate[SDL_SCANCODE_2])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

const Uint8* get_button_pressed() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    return keystate;
}