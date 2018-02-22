#include "SDL2/SDL.h"
#include <assert.h>

#include "basic.h"
#include "mathstuff.h"
#include "memorystuff.h"
#include "stringstuff.h"
//#include "pthreads.h"

SDL_Window *the_window = NULL;

u8 initialize() {
  if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
    string_t error = temp_string_from_cstr(SDL_GetError());
    hc_print(cake("SDL_Init failed: '%str'\n"), error);
    return FALSE;
  }
  
  the_window = SDL_CreateWindow(
    "Harvest and Construct",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    SDL_WINDOW_SHOWN
  );
  
  if(!the_window) {
    string_t error = temp_string_from_cstr(SDL_GetError());
    hc_print(cake("SDL_CreateWindow failed: '%str'\n"), error);
    return FALSE;
  }
  
  return TRUE;
}

void deinitialize() {
  SDL_DestroyWindow(the_window);
  SDL_Quit();
}

typedef struct {
  f32 x, y, z;
} v3;

#ifdef WIN64
#include <windows.h>
#endif

#ifdef LINUX64
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

int main(int argc, char **argv) {
  
  // saltucake(NOTE): These are just some string function tests.
  string_t name = cake("Aino");
  
  hc_print(cake("Help me, %str.\n"), name);
  
  hc_puts(cake("Hello, everyone!\n\n"));
  
  hc_print(cake("0 = %u8\n\n"), 0);
  hc_print(cake("sizeof(u8)  = %u8\n"), 8*sizeof(u8));
  hc_print(cake("sizeof(u16) = %u8\n"), 8*sizeof(u16));
  hc_print(cake("sizeof(u32) = %u8\n"), 8*sizeof(u32));
  hc_print(cake("sizeof(u64) = %u8\n"), 8*sizeof(u64));
  
  v3 v = {1, 2.2, 3.5};
  
  hc_print(cake("v = (%f32, %f32, %f32)\n"), v.x, v.y, v.z);
  hc_print(cake("PI  = %f64\n"), PI);
  hc_print(cake("TAU = %f64\n\n"), TAU);
  
  for(u8 i = 0; i <= 50; ++i)
    hc_print(cake("%f32\n"), i / 10.0f);
  
  if(!initialize())
    return 1;
  
  // saltycake(NOTE): Just set the window surface pixels to 0 for now, so it's more visible.
  SDL_Surface *window_surface = SDL_GetWindowSurface(the_window);
  if(window_surface) {
    //memset(window_surface->pixels, 0, window_surface->pitch*window_surface->h);
    
    u64 pitch = window_surface->pitch;
    for(u64 i = 0; i < 1000; ++i) {
      u64 amount = pitch*(i / 1000.0f);
      for(u64 j = 0; j < window_surface->h; ++j)
        memset(window_surface->pixels + j*pitch, 255, amount);
      
      SDL_UpdateWindowSurface(the_window);
    }
  }
  
  deinitialize();
  
  #ifdef WIN64
  getch();
  #else
  struct termios orig_config;
  tcgetattr(STDIN_FILENO, &orig_config);
  
  struct termios io_config = orig_config;
  io_config.c_lflag &= ~ICANON;
  io_config.c_cc[VMIN] = 1;
  io_config.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, 0, &io_config);
  u8 input_char;
  
  hc_puts(cake("Press any key to exit.\n"));
  read(STDIN_FILENO, &input_char, sizeof(u8));
  
  tcsetattr(STDIN_FILENO, 0, &orig_config);
  #endif
  
  return 0;
}
