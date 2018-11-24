#define STDOUT_FILENO 1
#define WRITE_SYS_NUM 1

int main(int argc, char *argv[]){
  int err;
  const char *text = "abc\n";

  /* write system call # = 1 */
  asm volatile( "mov %0, %%rax" : : "i"( WRITE_SYS_NUM ) );

  /* 1st argument (fd=stdout) */
  asm volatile( "mov %0, %%rdi" : : "i"( STDOUT_FILENO ) );

  /* 2nd argument ( *text ) */
  asm volatile( "mov %0, %%rsi" : : "m"( text ) );

  /* 3rd argument ( size ) */
  asm volatile( "mov %0, %%rdx" : : "i"( sizeof( text ) - 1 ) );
  asm volatile( "syscall" : "=a"( err ) );

  return(0);
}
