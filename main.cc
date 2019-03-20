#include "lib.hpp"

/* compress from stdin to srdout */
int main(int argc, char **argv)
{
  int ret;

  /* do compression if no arguments */
  if (argc == 1)
  {
    ret = def(stdin, stdout, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
      zerr(ret);
    return ret;
  }
  /* otherwise, report usage */
  else
  {
    fputs("zpipe usage: zpipe [-d] < source > dest\n", stderr);
    return 1;
  }
}
