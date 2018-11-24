/*
 * va-arg-test.c
 *
 * 普通のLinuxプログラミングでlighthttpdをmakeで
 * コンパイルしようとしたときにエラーコードが出たため 
 * 検証用に作成。
 *
 *
 * va_argマクロは、<stdarg.h>に含まれており、
 * 可変長引数から型で指定された型を持つ引数を一つ取得する。
 * 引数apは、va_startマクロかva_copyマクロによって初期化されている必要がある。
 * va_argマクロを呼び出すごとに、つぎの実引数の値が順に返されるように
 * apを更新する。
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int Summanation(int, ...);

int main(void)
{
  int result;
 
  result = Summanation(3, 1, 2, 3);
  printf("1から3までの相和は%dです. \n", result);

  return EXIT_SUCCESS;
}

/**
 *  @brief 相和を求める
 *  @param[in] count 引数の数
 *  @param[in] ...   可変長引数
 *  @return          相和
 */


int Summanation(int count, ...)
{
  va_list ap;
  int i;
  int sum = 0;
  
  va_start(ap, count);

  for (i = 0; i < count; i++){
      sum += va_arg(ap, int);
  }

  va_end(ap);

  return sum;
} 
