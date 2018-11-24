#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++){
    FILE *f;  /* ファイルストリームへのポインタ格納用 */
    int c;

    /* fopen()を使いファイルへのストリームを作成し、FILE型へのポインタ型へのポインタ型の変数fに代入 */
    f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      exit(1);
    }
  
    /* int fgetc(FILE *stream) : fから1文字取得し、戻り値としてint 型cに代入 */
    while ((c = fgetc(f)) != EOF) {
      /* 1文字書き出し: 'int putchar(int c)' で  putc((c), (stdout))と同じ意味を持つ */
      if (putchar(c) < 0) exit(1);
    }
    
    fclose(f);
  }
  exit(0);
}
