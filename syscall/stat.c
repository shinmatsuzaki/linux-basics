/* 
 * LINUXシステムプログラミング P202
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  /* stat()システムコールの返り値はstat構造体へ返される */
  struct stat sb;
  int ret;

  if (argc < 2){
          fprintf(stderr, "usage: %s <file>\n", argv[0]); 
          return 1;
  }

  /* stat()システムコールでファイルの情報を取得 */
  ret = stat(argv[1], &sb);
  if (ret) {
           perror ("stat");
           return 1;
  }

  printf ("%s's inode is %ld\n", argv[1], sb.st_ino);
  printf ("%s's filesize is %ld bytes\n", argv[1], sb.st_size);

  return 0;
}

