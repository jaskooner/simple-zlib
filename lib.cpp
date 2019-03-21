#include "lib.hpp"

int def(FILE *source, FILE *dest, int level)
{
  int ret, flush;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];

  /* allocate defalte state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  ret = deflateInit(&strm, level);
  if (ret != Z_OK)
    return ret;

  /* compress until end of file */
  do
  {
    strm.avail_in = fread(in, 1, CHUNK, source);
    if (ferror(source))
    {
      (void)deflateEnd(&strm);
      return Z_ERRNO;
    }
    flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
    strm.next_in = in;

    /* run deflate() on input until output buffer not full,
finish compression if all of source has been read in */

    do
    {
      strm.avail_out = CHUNK;
      strm.next_out = out;

      ret = deflate(&strm, flush); /* no bad return value */
      assert(ret != Z_STREAM_ERROR);

      have = CHUNK - strm.avail_out;
      if (fwrite(out, 1, have, dest) != have || ferror(dest))
      {
        (void)deflateEnd(&strm);
        return Z_ERRNO;
      }

    } while (strm.avail_out == 0);
    assert(strm.avail_in == 0); /* all input will be used */

    /* done when last data in file processed */
  } while (flush != Z_FINISH);
  assert(ret == Z_STREAM_END); /* stream will be complate */

  /*clean up an dreturn */
  (void)deflateEnd(&strm);
  return Z_OK;
}

int inf(FILE *source, FILE *dest)
{
  // The local variables have the same functionality as they do for
  // def(). The only difference is that there is no flush variable,
  // since inflate() can tell from the zlib stream itself when the
  // stream is complete.
  int ret;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  // The initialization of the state is the same, except that there
  // is no compression level, of course, and two more elements of the
  // structure are initialized. avail_in and next_in must be
  // initialized before calling inflateInit(). This is because the
  // application has the option to provide the start of the zlib
  // stream in order for inflateInit() to have access to information
  // about the compression method to aid in memory allocation. In the
  // current implementation of zlib (up through versions 1.2.x), the
  // method-dependent memory allocations are deferred to the first
  // call of inflate() anyway. However those fields must be
  // initialized since later versions of zlib that provide more
  // compression methods may take advantage of this interface.
  // In any case, no decompression is performed by inflateInit(),
  // so the avail_out and next_out fields do not need to be
  // initialized before calling.

  // Here avail_in is set to zero and next_in is set to Z_NULL
  // to indicate that no input data is being provided.
  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  ret = inflateInit(&strm);
  if (ret != Z_OK)
    return ret;

  // The outer do-loop decompresses input until inflate() indicates
  // that it has reached the end of the compressed data and has
  // produced all of the uncompressed output. This is in contrast
  // to def() which processes all of the input file. If end-of-file
  // is reached before the compressed data self-terminates, then the
  // compressed data is incomplete and an error is returned.

  /* decompress until deflate stream ends or end of file */
  do
  {
    strm.avail_in = fread(in, 1, CHUNK, source);
    if (ferror(source))
    {
      (void)inflateEnd(&strm);
      return Z_ERRNO;
    }
    if (strm.avail_in == 0)
      break;
    strm.next_in = in;

    /* run inflate() on input until output buffer not full */
    do
    {
      strm.avail_out = CHUNK;
      strm.next_out = out;

      ret = inflate(&strm, Z_NO_FLUSH);
      assert(ret != Z_STREAM_ERROR); /* state not clobbered */
      switch (ret)
      {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR; /* and fall through */
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
        (void)inflateEnd(&strm);
        return ret;
      }

      have = CHUNK - strm.avail_out;
      if (fwrite(out, 1, have, dest) != have || ferror(dest))
      {
        (void)inflateEnd(&strm);
        return Z_ERRNO;
      }

    } while (strm.avail_out == 0);

    /* done when inflate() says it's done */
  } while (ret != Z_STREAM_END);

  /* clean up and return */
  (void)inflateEnd(&strm);
  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void zerr(int ret)
{
  fputs("zpipe: ", stderr);
  switch (ret)
  {
  case Z_ERRNO:
    if (ferror(stdin))
      fputs("error reading stdin\n", stderr);
    if (ferror(stdout))
      fputs("error writing stdout\n", stderr);
    break;
  case Z_STREAM_ERROR:
    fputs("invalid compression level\n", stderr);
    break;
  case Z_DATA_ERROR:
    fputs("invalid or incomplete deflate data\n", stderr);
    break;
  case Z_MEM_ERROR:
    fputs("out of memory\n", stderr);
    break;
  case Z_VERSION_ERROR:
    fputs("zlib version mismatch!\n", stderr);
  }
}