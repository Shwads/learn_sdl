#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <string>
#include <SDL2/SDL_render.h>

class LTexture {
public:
  // Initialises variables
  LTexture();

  // Deallocates memory
  ~LTexture();

  // Loads image from specified path
  bool load_from_file(std::string path, SDL_Renderer *gRenderer);

  // Deallocates textures
  void free();

  // Renders textures at given point
  void render(int x, int y, SDL_Renderer *gRenderer);

  // Gets image dimensions
  int getWidth();
  int getHeight();

private:
  // The actual texture 
  SDL_Texture *mTexture;

  int mWidth;
  int mHeight;
};

#endif
