// modified POSIX.1-2001 implementation  for rand, taken from rand_r man page.

static uint32_t rand_next = 42;

static uint8_t rand_get()
{
  rand_next = rand_next * 1103515245 + 12345;
  return (rand_next / 0xFF);
}
