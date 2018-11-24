/* C言語ポインタ完全制覇 P103 */

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

/* function */

void tiny_printf(char *format, ...)
{
  int i;

  /* va_list型の変数apの宣言。va_listはcharへのポインタ。 */
  va_list ap;

  /* ポインタapを、引数formatの次の位置に向ける */
  /* 実際はここでは、"sdd"が*formatにわたっているが、なぜ&演算子をつけていないのに、アドレスが渡せるのかがよくわからない。。 */
  va_start(ap, format);

  for (i = 0; format[i] != '\0'; i++) {
      switch (format[i]) {
        case 's':
          printf("%s ", va_arg(ap, char*)); 
          break;

        case 'd':
          printf("%d ", va_arg(ap, int));
          break;

        default:
          assert(0);
    }
  }
  va_end(ap);
  putchar('\n');
}

int main(void)
{
  /* tiny_printfでは、第1引数に以下に続く引数の型を指定し、第2引数以降で表示する値を指定する */
  /* 第1引数の"sdd"は、s=文字列、d=数価×2を意味する。 */
  /* 第2引数以降で、実際に文字列、数価、数価の順に渡している。(値渡し) */
  tiny_printf("sdd", "result..", 3, 5);

  return 0;
}

