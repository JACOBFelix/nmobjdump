#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "shared.h"
#include "objdump.h"

static off_t	calcfilesize(int const fd)
{
  off_t	i;

  i = lseek(fd, 0, SEEK_END);
  if (i == -1)
    {
      fprintf(stderr, "Error: can't know file size\n");
      return (0);
    }
  return (i);
}

static void	pre_obj(char const *const str)
{
  int		fd;
  t_objdata	data;

  data.file_name = (char *)str;
  if ((fd = open(data.file_name, O_RDONLY )) == -1)
    return ;
  if ((data.filesize = calcfilesize(fd)) < (off_t)sizeof(Elf64_Ehdr))
    return ;
  if ((data.data = mmap(NULL, data.filesize, PROT_READ, MAP_SHARED, fd, 0))
      == MAP_FAILED)
    {
      close(fd);
      return ;
    }
  close(fd);
  data.header = (Elf64_Ehdr *)data.data;
  start_obj(&data);
  munmap(data.data, data.filesize);
}

int	main(int ac, char const *const ag[])
{
  int	i;

  if (ac == 1)
    pre_obj("a.out");
  else if (ac == 2)
    pre_obj(ag[1]);
  else
    {
      i = 0;
      while (++i < ac)
	{
	  pre_obj(ag[i]);
	}
    }
  return (0);
}
