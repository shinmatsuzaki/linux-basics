#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char *path);

int main(int argc, char *argv[])
{
  int i;

  if (argc < 2){
      fprintf(stderr, "%s: no arguments\n", argv[0]);
      exit(1);
  }

  for(i = 1; i < argc; i++){
      do_ls(argv[i]);
  }

  exit(0);
}

static void
do_ls(char *path)
{
  DIR *d;
  struct dirent *ent;

  d = opendir(path);
  if(!d){
     perror(path);
     exit(1);
  }
  while (ent = readdir(d)){
     printf("%8ld", ent->d_ino);
     printf("%s", ent->d_name);
     printf("%4d\n", ent->d_reclen);     
  }
  closedir(d);
}
