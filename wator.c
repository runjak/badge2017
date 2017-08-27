#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rand.c"

#define WATOR_WIDTH 64
#define WATOR_HEIGHT 64
#define WATOR_RESPAWN 200
#define WATOR_FISH_UP 40
#define WATOR_SHARK_UP 10
#define WATOR_SHARK_DOWN 10
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
  return fish > 128;
}

static uint8_t wator_reproduce(uint8_t fish)
{
  if(fish == 255){
    return 255;
  }

  if(fish == 127){
    return 64;
  }

  return 0;
}

uint8_t wator_fishUp(uint8_t fish){
  if(wator_isShark(fish)){
    if(fish < (255 - WATOR_SHARK_UP)){
      return fish + WATOR_SHARK_UP;
    }

    return 255;
  }

  if(fish < (128 - WATOR_FISH_UP)){
    return fish + WATOR_FISH_UP;
  }

  return 127;
}

uint8_t wator_fishDown(uint8_t fish){
  uint8_t fNext = fish - WATOR_SHARK_DOWN;

  if(wator_isShark(fish)){
    if(wator_isShark(fNext)){
      return fNext;
    }

    return 0;
  }

  return 0;
}

uint8_t wator_age(uint8_t fish)
{
  if(wator_isShark(fish)){
    return wator_fishDown(fish);
  }

  return wator_fishUp(fish);
}

static void wator_move(world* w, uint8_t x, uint8_t y)
{
  uint8_t fish = wator_age((*w)[x][y]);
  (*w)[x][y] = fish;

  const uint8_t prevX = (x - 1 + WATOR_WIDTH) % WATOR_WIDTH;
  const uint8_t prevY = (y - 1 + WATOR_HEIGHT) % WATOR_HEIGHT;
  const uint8_t nextX = (x + 1) % WATOR_WIDTH;
  const uint8_t nextY = (y + 1) % WATOR_HEIGHT;
  const uint8_t nexts[8][2] = {
    { x, prevY },
    { prevX, prevY },
    { prevX, y },
    { prevX, nextY },
    { x, nextY },
    { nextX, nextY },
    { nextX, y },
    { nextX, prevY },
  };

  uint8_t offset = rand_get() % 8;
  uint8_t target = 0;
  for(uint8_t i = 0; i < 8; i++) {
    offset = (offset + 1) % 8;
    target = wator_get(w, nexts[offset][0], nexts[offset][1]);

    if(wator_isShark(fish)){
      if(!wator_isShark(target)){
        break;
      }
    }else{
      if(!wator_alive(target)){
        break;
      }
    }
  }

  if(wator_isShark(fish)){
    if(wator_isShark(target)){
      return;
    }

    if(wator_alive(target)){
      fish = wator_fishUp(fish);
    }
  }else if(wator_alive(target)){
    return;
  }

  uint8_t offspring = wator_reproduce(fish);
  if(offspring != 0){
    fish = offspring;
  }
  (*w)[nexts[offset][0]][nexts[offset][1]] = fish;
  (*w)[x][y] = wator_reproduce(fish);
}

static void wator_update(world* w)
{
  bool hasShark = false;
  bool hasFish = false;
  for(uint8_t x = 0; x < WATOR_WIDTH; x++) {
    for(uint8_t y = 0; y < WATOR_HEIGHT; y++) {
      uint8_t fish = wator_get(w, x, y);
      if(wator_alive(fish)){
        bool isShark = wator_isShark(fish);
        hasShark |= isShark;
        hasFish |= !isShark;
        wator_move(w, x, y);
      }
    }
  }

  if(!hasShark || !hasFish){
    for(uint8_t i = 0; i < WATOR_RESPAWN; i++){
      uint8_t x = rand_get() % WATOR_WIDTH;
      uint8_t y = rand_get() % WATOR_HEIGHT;

      if(!hasShark){
        (*w)[x][y] = rand_get() | 128;
      }else{
        (*w)[x][y] = rand_get() & 127;
      }
    }
  }
}
