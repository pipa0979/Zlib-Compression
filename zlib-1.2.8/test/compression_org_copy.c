#include <stdio.h>
#include <zlib.h>
#include <limits.h> /* for PATH_MAX */

int compressFile(FILE *in, const char * const outFileName)
{
   /* Buffer to hold data read */
   char buf[BUFSIZ] = { 0 };
   size_t bytes_read = 0;
   gzFile *out = gzopen(outFileName, "wb");
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
         fprintf(stderr, "Error during compression: %s", gzerror(out, &err_no))\
;
         gzclose(out);
         return -1;
      }
      bytes_read = fread(buf, 1, BUFSIZ, in);
   }
   gzclose(out);

   return 0;
}

int main()
{
   char outFileName[PATH_MAX] = { 0 };
   FILE *fp = fopen(__FILE__, "r");
   if (fp)
   {
      sprintf(outFileName, "%s.gz", __FILE__);
      if (compressFile(fp, outFileName) != 0)
      {
         fclose(fp);
         return -1;
      }

      fclose(fp);
   }

   return 0;
}
/*
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
using namespace boost::filesystem;
using namespace boost::lambda;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: tut1 path\n";
    return 1;
  }
  std::cout << argv[1] << " " << file_size(argv[1]) << '\n';
  return 0;
}
*/
/*
#include <fstream>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

int main() 
{
    using namespace std;

    ifstream file("hello.z", ios_base::in | ios_base::binary);
    filtering_streambuf<input> in;
    in.push(zlib_decompressor());
    in.push(file);
    boost::iostreams::copy(in, cout);
}
*/