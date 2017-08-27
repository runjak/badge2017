#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rand.c"

#define WATOR_WIDTH 64
#define WATOR_HEIGHT 64
typedef uint8_t world[WATOR_WIDTH][WATOR_HEIGHT];

static world* wator_init()
{
  world* w = malloc(sizeof(world));
	w = memset(w, 0, sizeof(world));

  volatile world* v = w;
  for(uint8_t x = 0; x < WATOR_WIDTH; x++){
    for(uint8_t y = 0; y < WATOR_HEIGHT; y++){
      (*v)[x][y] = rand_get();
    }
  }

  return w;
}

static uint8_t wator_get(world* w, uint8_t x, uint8_t y)
{
  return (*w)[x % WATOR_WIDTH][y % WATOR_HEIGHT];
}

static bool wator_alive(uint8_t fish)
{
  return fish > 0;
}

static bool wator_isShark(uint8_t fish)
{
  return (fish & 0x80) > 0;
}
