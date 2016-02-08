#define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>
#include "shared.h"
#include "nm.h"

static off_t	calcfilesize(int const fd, char const *const name)
{
  off_t	i;

  i = lseek(fd, 0, SEEK_END);
  if (i == -1)
    {
      fprintf(stderr, "Error: File %s can't know size\n", name);
      return (0);
    }
  return (i);
}

static void	nm(int const fd, char const *const name)
{
  t_data64	data64;

  data64.file_name = (char *)name;
  if ((data64.filesize = calcfilesize(fd, name)) < (off_t)sizeof(Elf64_Shdr))
    {
      dprintf(2, "Error:File %s is too small or can't know filesize\n", name);
      return ;
    }
  if ((data64.data
       = mmap(NULL, data64.filesize, PROT_READ, MAP_SHARED, fd, 0))
      == MAP_FAILED)
    {
      fprintf(stderr, "Error: Can't load in memory file %s\n", name);
      return ;
    }
  data64.header = (Elf64_Ehdr *)data64.data;
  if (data64.header->e_ident[4] == ELFCLASS32)
    printf("32 bits\n");
  else if (data64.header->e_ident[4] == ELFCLASS64)
    start_nm64(&data64);
  munmap(data64.data, data64.filesize);
}

static void	start_nm(char const *const name)
{
  int		fd;

  if ((fd = open(name, O_RDONLY)) == -1)
    {
      fprintf(stderr, "Error: Can't open file %s\n", name);
      return ;
    }
  nm(fd, name);
  if (close(fd) == -1)
    fprintf(stderr, "Error: Can't close file %s\n", name);
}

int	main(int const ac,
	     char const *const ag[],
	     char const *const env[] UNUSED)
{
  int	i;

  if (ac == 1)
    start_nm("a.out");
  else if (ac == 2)
    start_nm(ag[1]);
  else
    {
      i = 0;
      while (++i < ac)
	{
	  printf("%s:\n", ag[i]);
	  start_nm(ag[i]);
	}
    }
  return (0);
}
