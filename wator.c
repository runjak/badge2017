#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rand.c"

typedef uint8_t world[64][64];

static world* wator_init()
{
  world* w = malloc(sizeof(world));
	w = memset(w, 0, sizeof(world));

  volatile world* v = w;
  for(uint8_t x = 0; x < 64; x++){
    for(uint8_t y = 0; y < 64; y++){
      (*v)[x][y] = rand_get();
    }
  }

  return w;
}

static uint8_t wator_get(world* w, int x, int y)
{
  return (*w)[x % 64][y % 64];
}

static bool wator_alive(uint8_t fish)
{
  return fish > 0;
}

static bool wator_isShark(uint8_t fish)
{
  return (fish & 0x80) > 0;
}
