#include "LTexture.h"
#include <SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool load_media();
void close();
SDL_Surface *load_surface(std::string path);

// Loads individual image as texture
SDL_Texture *load_texture(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;

// Creating the window
bool init() {
  bool success = true;

  // Initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

      if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      } else {
        // Initialise renderer colour
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialise PNG loading
        int img_flags = IMG_INIT_PNG;

        if (!(IMG_Init(img_flags) & img_flags)) {
          printf("SDL image could not initialise! SDL_image error: %s\n",
                 IMG_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

bool load_media() {
  bool success = true;

  // Load 'Foo' texture
  if (!gFooTexture.load_from_file("10_color_keying/foo.png", gRenderer)) {
    printf("Failed to load Foo texture image!\n");
    success = false;
  }

  // Load background texture
  if (!gBackgroundTexture.load_from_file("10_color_keying/background.png",
                                         gRenderer)) {
    printf("Failed to load background texture image!\n");
    success = false;
  }

  return success;
}

// Deallocate memory before we exit
void close() {
  // Free loaded image
  printf("Freeing textures...\n");
  gBackgroundTexture.free();
  gFooTexture.free();

  // Destroy window
  printf("Destroying the window...\n");
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gRenderer = NULL;
  gWindow = NULL;

  // Quit SDL subsystems
  printf("Quitting SDL subsystems...\n");
  IMG_Quit();
  SDL_Quit();
}

int main() {
  if (!init()) {
    printf("Failed to initialise!\n");
  } else {
    // Load media
    if (!load_media()) {
      printf("Failed to load media!\n");
    } else {

      SDL_Event e;
      bool quit = false;

      // While program has not been quit
      while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit
          if (e.type == SDL_QUIT) {
            quit = true;
            // User presses a key
          }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render background texture to screen
        gBackgroundTexture.render(0, 0, gRenderer);

        // Render foo to the screen
        gFooTexture.render(240, 190, gRenderer);

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  // Free up resources and close SDL
  close();

  return 0;
}
