#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h" 

/* CHUNK is simply the buffer size for feeding data to and pulling data from the zlib routines. Larger buffer sizes would be more efficient, especially for inflate(). If the memory is available, buffers sizes on the order of 128K or 256K bytes should be used.*/

#define CHUNK 16384

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */

int def (FILE *source, FILE *dest, int Level);

/* report a zlib or i/o error */
void zerr(int ret);

