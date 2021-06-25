#include "controller/controller.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL2/SDL.h"

static bool s_exit = false;
static SDL_Window *s_window = NULL;
static SDL_Renderer *s_renderer = NULL;

/// TODO: Threading.

void controller_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialise SDL: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    /// TODO: Loop through modes and select the window size, minus 100 pixels
    /// in all directions.

    s_window = SDL_CreateWindow(
        "Asteroids",
        600, 600,
        600, 600,
        SDL_WINDOW_RESIZABLE
    );

    if (!s_window) {
        printf("Failed to create SDL window: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED);
    if (!s_renderer) {
        printf("Failed to create SDL renderer: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
}

void controller_handle_events()
{
    // Poll for all queued events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_KEYUP           : controller_handle_key_up(&event); break;
            case SDL_KEYDOWN         : controller_handle_key_down(&event); break;
            case SDL_MOUSEBUTTONUP   : controller_handle_mouse_up(&event); break;
            case SDL_MOUSEBUTTONDOWN : controller_handle_mouse_down(&event); break;
            case SDL_MOUSEWHEEL      : controller_handle_mouse_wheel(&event); break;
            case SDL_MOUSEMOTION     : controller_handle_mouse_motion(&event); break;
            case SDL_QUIT : {
                s_exit = true;
                break;
            }
            default: break;
        }
    }
}

void controller_handle_key_up(SDL_Event *event)
{

}

void controller_handle_key_down(SDL_Event *event)
{

}

void controller_handle_mouse_up(SDL_Event *event)
{

}

void controller_handle_mouse_down(SDL_Event *event)
{

}

void controller_handle_mouse_wheel(SDL_Event *event)
{

}

void controller_handle_mouse_motion(SDL_Event *event)
{

}

void controller_draw()
{
    static const float R = 150.0;
    static float theta = 0;

    /// TODO: Move to view

    // Clear the screen.
    SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(s_renderer);

    // Set render colour to white.
    SDL_SetRenderDrawColor(s_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    int x = 300 + R * cos(theta);
    int y = 300 - R * sin(theta);
    theta = theta > 6.28318530718 ? 0 : theta + 0.001;

    SDL_RenderDrawLine(s_renderer, 300, 300, x, y);
    SDL_RenderPresent(s_renderer);
}

void controller_main()
{
    controller_init();

    while (!s_exit)
    {
        controller_handle_events();
        controller_draw();
    }
}