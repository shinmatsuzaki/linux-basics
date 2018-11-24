/*
 * cat.c
 *
 * 普通のLinuxプログラミング P86
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* プロトタイプ宣言 */
static void do_cat(const char *path);
static void die(const char *s);

/* メイン関数 */
int
main(int argc, char *argv[])
{
  int i;
  if (argc < 2) {
      fprintf(stderr, "%s: file name not given\n", argv[0]);
      exit(1);
  }

  for(i = 1; i < argc; i++){
    do_cat(argv[1]);
  }
  exit(0);
}

/* バッファサイズの指定 */
#define BUFFER_SIZE 2048

static void
do_cat(const char *path)
{
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;

  fd = open(path, O_RDONLY);
  for (;;) {
      n = read(fd, buf, sizeof buf);
      /* read失敗時 */
      if (n < 0) die(path);

      /* 全てのデータのreadが完了 */
      if (n == 0) break;

      if (write(STDOUT_FILENO, buf, n) < 0) die(path);
  }
  if (close(fd) < 0) die(path);

}

static void
die(const char *s)
{
  perror(s);
  /* この関数が発動している時点で順正常系で動いているので、戻り値:1で終了する */
  exit(1);
}
