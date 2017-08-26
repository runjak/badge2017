#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t world[128][64];

static world* wator_init()
{
  world* w = malloc(sizeof(world));
	w = memset(w, 0, sizeof(world));

  volatile world* v = w;
  for(uint8_t i = 0; i < 128; i++){
    uint8_t x = i;
    uint8_t y = i % 64;

    (*v)[x][y] = 5;
  }

  return w;
}

static uint8_t wator_get(world* w, int x, int y)
{
  return (*w)[x % 128][y % 64];
}

static bool wator_alive(uint8_t fish)
{
  return fish > 0;
}
