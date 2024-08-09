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
SDL_Surface *gCurrentSurface = NULL;

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

// Loading our media (bitmap file)
bool load_media() {
  // Loading success flag
  bool success = true;

  // Load default surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] =
      load_surface("04_key_presses/press.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
    printf("Failed to load default image!\n");
    success = false;
  }

  // Load up surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] =
      load_surface("04_key_presses/up.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
    printf("Failed to load up image!\n");
    success = false;
  }

  // Load down surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] =
      load_surface("04_key_presses/down.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
    printf("Failed to load down image!\n");
    success = false;
  }

  // Load left surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] =
      load_surface("04_key_presses/left.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
    printf("Failed to load left surface!\n");
    success = false;
  }

  // Load right surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] =
      load_surface("04_key_presses/right.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
    printf("Failed to load right surface!\n");
    success = false;
  }

  return success;
}

// Deallocate memory before we exit
void close() {
  // Deallocate surface
  printf("Deallocating the surfaces...\n");
  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = NULL;

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
  // Load image at specified path
  SDL_Surface *loaded_surface = SDL_LoadBMP(path.c_str());
  if (loaded_surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
  }

  return loaded_surface;
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

      gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

      // While program has not been quit
      while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit
          if (e.type == SDL_QUIT) {
            quit = true;
            // User presses a key
          } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
              break;

            case SDLK_DOWN:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
              break;

            case SDLK_LEFT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
              break;

            case SDLK_RIGHT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
              break;

            default:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
              break;
            }
          }
        }

        // Apply the current image
        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
    }
  }

  // Free up resources and close SDL
  close();

  return 0;
}
