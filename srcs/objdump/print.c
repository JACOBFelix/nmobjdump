#include <string.h>
#include <stdio.h>
#include "shared.h"
#include "objdump.h"

void		print_hexa(uint8_t *data, int size)
{
  int		i;

  i = 0;
  while (i < 16 && i < size)
    {
      if (i % 4 == 0 && i != 0)
	printf(" ");
      printf("%02x", data[i]);
      i++;
    }
  while (i < 16)
    {
      if (i % 4 == 0 && i != 0)
	printf(" ");
      printf("  ");
      i++;
    }
}

void		print_ascii(uint8_t *data, int size)
{
  int		i;

  i = -1;
  printf("  ");
  while (++i < 16 && i < size)
    {
      if (data[i] < 32)
	printf(".");
      else if (data[i] > 126)
	printf(".");
      else
	printf("%c", data[i]);
    }
}

void		print_pos(char *str, Elf64_Shdr *shdr, int k, int i)
{

  static int	a = -1;
  static int	b = 0;

  if (a != i)
    {
      a = i;
      b = 0;
    }
  else
    b += 16;
  if (strncmp(str, ".debug", 6) != 0)
    printf(" %04x", (int) shdr[0].sh_addr + b);
  else
    printf(" %04x", k);
}

static int	second_try(Elf64_Shdr *shdr, char *strtab, uint8_t *data, int i)
{
   int		k;

   if ((shdr->sh_size != 0 ||
       shdr->sh_type == SHT_SYMTAB ||
       shdr->sh_type == SHT_NOBITS ||
       shdr->sh_type == SHT_STRTAB) &&
       strcmp(".dynstr", &strtab[shdr[i].sh_name]) != 0)
     return (SUCCESS);
   k = shdr[i].sh_offset;
   printf("Contents of section %s:\n", &strtab[shdr[i].sh_name]);
   while (k < (int)(shdr[i].sh_offset + shdr[i].sh_size))
     {
       print_pos(&strtab[shdr[i].sh_name],
		 shdr + i, k - shdr[i].sh_offset, i);
       printf(" ");
       print_hexa(data + k, shdr[i].sh_offset + shdr[i].sh_size - k);
       print_ascii(data + k, shdr[i].sh_offset + shdr[i].sh_size - k);
       printf("\n");
       k += 16;
     }
   return (SUCCESS);
}

void		try_obj(t_objdata *data)
{
  int		i;

  i = 0;
  while (++i < data->header->e_shnum)
    if ((void *)&data->section[i].sh_addr > data->data + data->filesize
	|| (void *)&data->section[i].sh_type > data->data + data->filesize
	|| (void *)&data->section[i].sh_addr > data->data + data->filesize
	|| (void *)&data->section[i].sh_name > data->data + data->filesize
	|| (void *)&data->section[i].sh_offset > data->data + data->filesize
	|| (void *)&data->section[i].sh_size > data->data + data->filesize
	|| (void *)data->data + data->section[i].sh_offset
	+ data->section[i].sh_size > data->data + data->filesize
	||
	second_try(data->section, data->strtab,
		      (uint8_t *)data->data, i) == FAILURE)
      return ;
  return ;
}
