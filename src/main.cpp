#include "LTexture.h"
#include <SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_platform.h>
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

// Scene sprites
SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

//
// ==============================================
//              CREATING THE WINDOW
// ==============================================
//
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
//
//
//

bool load_media() {
  bool success = true;

  // Load sprite sheet texture
  if (!gSpriteSheetTexture.load_from_file(
          "11_clip_rendering_and_sprite_sheets/dots.png", gRenderer)) {
    printf("Failed to load sprite sheet texture!\n");
    success = false;
  } else {
    // Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 100;
    gSpriteClips[0].h = 100;

    // Set top right sprite
    gSpriteClips[1].x = 100;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 100;
    gSpriteClips[1].h = 100;

    // Set bottom left sprite
    gSpriteClips[2].w = 0;
    gSpriteClips[2].y = 100;
    gSpriteClips[2].w = 100;
    gSpriteClips[2].h = 100;

    // Set bottom right sprite
    gSpriteClips[3].x = 100;
    gSpriteClips[3].y = 100;
    gSpriteClips[3].w = 100;
    gSpriteClips[3].h = 100;
  }

  return success;
}

// Deallocate memory before we exit
void close() {
  // Free loaded image
  printf("Freeing textures...\n");
  gSpriteSheetTexture.free();

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

        // Clear the screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render top left sprite
        gSpriteSheetTexture.render(0, 0, gRenderer, &gSpriteClips[0]);

        // Render top right sprite
        gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0,
                                   gRenderer, &gSpriteClips[1]);

        // Render bottom left sprite
        gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h,
                                   gRenderer, &gSpriteClips[2]);

        // Render bottom right sprite
        gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w,
                                   SCREEN_HEIGHT - gSpriteClips[3].h, gRenderer,
                                   &gSpriteClips[3]);

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  // Free up resources and close SDL
  close();

  return 0;
}
