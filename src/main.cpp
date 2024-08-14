#include <SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool load_media();
void close();
SDL_Surface *load_surface(std::string path);

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// Currently displayed surface
SDL_Surface *gStretchedSurface = NULL;

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
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool load_media() {
  bool success = true;

  gStretchedSurface = load_surface(
      "05_optimized_surface_loading_and_soft_stretching/stretch.bmp");
  if (gStretchedSurface == NULL) {
    printf("Failed to load stretch surface!\n");
    success = false;
  }

  return success;
}

// Deallocate memory before we exit
void close() {
  // Deallocate surface
  SDL_FreeSurface(gStretchedSurface);
  gStretchedSurface = NULL;

  // Destroy window
  printf("Destroying the window...\n");
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  printf("Quitting SDL subsystems...\n");
  SDL_Quit();
}

// Get surface from bitmap path
SDL_Surface *load_surface(std::string path) {
  // The final optimised image
  SDL_Surface *optimised_surface = NULL;

  // Load image at specified path
  SDL_Surface *loaded_surface = SDL_LoadBMP(path.c_str());

  if (loaded_surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
  } else {
    // Convert surface to screen format
    optimised_surface =
        SDL_ConvertSurface(loaded_surface, gScreenSurface->format, 0);
    if (optimised_surface == NULL) {
      printf("Unable to optimise image %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
  }

  return optimised_surface;
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

        // Apply the current image
        // SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Apply the stretched image
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
    }
  }

  // Free up resources and close SDL
  close();

  return 0;
}
