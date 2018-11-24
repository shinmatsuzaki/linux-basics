#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define BUFFER_SIZE	1000
#define ALLOC_SIZE	(100*1024*1024) /* 確保するメモリ量 */

static char command[BUFFER_SIZE];
static char command2[BUFFER_SIZE];

int main(void)
{
 pid_t pid;
 pid = getpid();

 /* commandに実行するCLIコマンドを格納 */
 snprintf(command, BUFFER_SIZE, "pmap -x %d", pid);
 snprintf(command2, BUFFER_SIZE, "cat /proc/meminfo | grep anon");

 puts("*** memory map before memory allocation ***");
 fflush(stdout);

 /* system()を使用し、メモリのマップ情報(事前)を表示 */
 system(command);
 puts("");
 system(command2);

 /* メモリ確保 */
 void *new_memory;
 new_memory = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
 if (new_memory == (void *) -1)
         err(EXIT_FAILURE, "mmap() failed");
 puts("");


 printf("*** succeeded to allocate memory: address = %p; size = 0x%x ***\n", new_memory, ALLOC_SIZE);
 puts("");

 /* system()を実行し、メモリのマップ情報(事後)を表示 */
 puts("*** memory map after memory allocation ***");
 fflush(stdout);
 system(command);
 puts("");
 system(command2);

 /* メモリ解放 */
 if (munmap(new_memory, ALLOC_SIZE) == -1)
            err(EXIT_FAILURE, "munmap() failed");

 puts("");
 printf("*** succeeded to release memory ***\n");
 puts("");

 /* system()を実行し、メモリのマップ情報(事後)を表示 */
 puts("*** memory map after memory release ***");
 fflush(stdout);
 system(command);
 puts("");
 system(command2);

exit(EXIT_SUCCESS);
}


