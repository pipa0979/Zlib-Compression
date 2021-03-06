#include <stdio.h>
#include <zlib.h>
#include <limits.h> /* for PATH_MAX */
#include <iostream>
using namespace std;

int compressFile(FILE *in, const char * const outFileName)
{
   /* Buffer to hold data read */
   char buf[BUFSIZ] = { 0 };
   size_t bytes_read = 0;
   gzFile out = gzopen(outFileName, "ab");
   if (!out)
     {
        /* Handle error */
        fprintf(stderr, "Unable to open %s for writing\n", outFileName);
        return -1;
     }
   bytes_read = fread(buf, 1, BUFSIZ, in);
   while (bytes_read > 0)
     {
        int bytes_written = gzwrite(out, buf, bytes_read);
        if (bytes_written == 0)
          {
             int err_no = 0;
             fprintf(stderr, "Error during compression: %s", gzerror(out, &err_no));
             gzclose(out);
             return -1;
          }
        bytes_read = fread(buf, 1, BUFSIZ, in);
     }
   gzclose(out);

   return 0;
}

int main(int argc, char *argv[])
{
   char outFileName[PATH_MAX] = { 0 };
   FILE *fp = fopen(argv[1], "r");

   if (fp)
     {
        sprintf(outFileName, "%s.gz", argv[1]);
        if (compressFile(fp, outFileName) != 0)
          {
             fclose(fp);
             return -1;
          }

        fclose(fp);
     }
   return 0;
}