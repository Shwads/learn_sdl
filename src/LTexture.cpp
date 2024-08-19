#include "LTexture.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2_image/SDL_image.h>
#include <string>

LTexture::LTexture() {
  // Initialise
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture() {
  // Deallocate
  free();
}

bool LTexture::load_from_file(std::string path, SDL_Renderer *gRenderer) {
  // Get rid of pre-exisiting texture
  free();

  // The final texture
  SDL_Texture *new_texture = NULL;

  // Load image at specified path
  SDL_Surface *loaded_surface = IMG_Load(path.c_str());

  if (loaded_surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           IMG_GetError());
  } else {
    // Color key image
    SDL_SetColorKey(loaded_surface, SDL_TRUE,
                    SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
    if (new_texture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    } else {
      // Get image dimensions
      mWidth = loaded_surface->w;
      mHeight = loaded_surface->h;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
  }

  // Return success
  mTexture = new_texture;
  return mTexture != NULL;
}

void LTexture::free() {
  if (mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

// clip in render copy to tell the function to only copy part of the texture
// to the screen's renderer
void LTexture::render(int x, int y, SDL_Renderer *gRenderer, SDL_Rect *clip) {
  SDL_Rect render_quad = {x, y, mWidth, mHeight};

  if (clip != NULL) {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }

  // Render to screen
  SDL_RenderCopy(gRenderer, mTexture, clip, &render_quad);
}

int LTexture::getWidth() { return mWidth; }

int LTexture::getHeight() { return mHeight; }
