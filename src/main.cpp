#include <SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface *gHelloWorld = NULL;

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
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool load_media() {
  // Loading success flag
  bool success = true;

  // Load splash image
  gHelloWorld =
      SDL_LoadBMP("02_getting_an_image_on_the_screen/hello_world.bmp");

  if (gHelloWorld == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n",
           "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
    success = false;
  }

  return success;
}

void close() {
  // Deallocate surface
  SDL_FreeSurface((gHelloWorld));
  gHelloWorld = NULL;

  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
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
      // Apply the image
      SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

      // Update the surface
      SDL_UpdateWindowSurface(gWindow);
      SDL_Event e;
      bool quit = false;
      while (quit == false) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            quit = true;
        }
      }
    }
  }

  // Free up resources and close SDL 
  close();

  return 0;
}
