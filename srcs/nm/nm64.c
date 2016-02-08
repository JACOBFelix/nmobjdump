#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shared.h"
#include "nm.h"

static void		nm64_symbol_header_handler(t_data64 *data,
						   uint16_t p,
						   uint16_t i,
						   uint16_t a)
{
  while (a++ < i - 1)
    {
      if ((void *)&data->sym[a] + sizeof(Elf64_Sym)
  	  > (void *)data->data + data->filesize)
  	return ;
      if (data->sym[a].st_name != 0 && data->sym[a].st_info != STT_FILE)
  	{
  	  data->table_sym[p] = &data->sym[a];
  	  if (&data->strsym[data->table_sym[p]->st_name]
  	      >= (char *)data + data->filesize)
  	    return ;
  	  p++;
  	}
    }
  data->table_sym[p] = NULL;
  if (sort_tab(data, 0) == FAILURE)
    return ;
  print_tab(data);
  free(data->table_sym);
}

static void		nm64_symbol_header(t_data64 *data)
{
  uint16_t		i;

  if ((i = data->section_symbole->sh_size / sizeof(Elf64_Sym)) < 1)
    return ;
  if ((data->table_sym = malloc(sizeof(Elf64_Sym *) * (i + 1))) == NULL)
    {
      fprintf(stderr, "Error allocation memory\n");
      return ;
    }
  nm64_symbol_header_handler(data, 0, i, 0);
}

static void		nm64_section_header_follow(t_data64 *data)
{
  if ((data->sym = (Elf64_Sym *)
       (data->data + data->section_symbole->sh_offset))
      > (Elf64_Sym *)((void *)(data + data->filesize)))
    return ;
  if ((data->strsym = (char *)data->data
       + data->str_section_header->sh_offset) > (char *)data + data->filesize)
    return ;
  nm64_symbol_header(data);
}

static void		nm64_section_header(t_data64 *data, uint16_t i)
{
  if ((data->strtab = (char *)(data->data + data->section_header
			       [data->header->e_shstrndx].sh_offset))
      > ((char *)data->data + data->filesize))
      return ;
  while (i++ < data->header->e_shnum)
    {
      if ((void *)&data->section_header[i] + sizeof(Elf64_Sym)
	  > (void *)data->data + data->filesize)
	return ;
      if (data->section_header[i].sh_size != 0)
	{
	  if (data->section_header[i].sh_type == SHT_SYMTAB)
	    data->section_symbole = (Elf64_Shdr *)&data->section_header[i];
	  else if (data->section_header[i].sh_type == SHT_STRTAB)
	    data->str_section_header = (Elf64_Shdr *)&data->section_header[i];
	}
    }
  if (data->section_symbole == NULL || data->str_section_header == NULL)
    {
      fprintf(stderr, "Can't find section header wish have symbols\n");
      return ;
    }
  nm64_section_header_follow(data);
}

int		start_nm64(t_data64 *data)
{
  if (data->header->e_ident[EI_MAG0] != ELFMAG0
      || data->header->e_ident[EI_MAG1] != ELFMAG1
      || data->header->e_ident[EI_MAG2] != ELFMAG2
      || data->header->e_ident[EI_MAG3] != ELFMAG3
      || (data->header->e_type != ET_EXEC
	  && data->header->e_type != ET_REL
	  && data->header->e_type != ET_DYN
	  && data->header->e_type != ET_CORE))
    return (FAILURE);
  if (((void *)(data->section_header = (Elf64_Shdr *)
	(data->data + data->header->e_shoff)))
      > (void *)data->data + data->filesize)
    return (FAILURE);
  if ((data->strtab = (char *)
       (data->data
	+ data->section_header[data->header->e_shstrndx].sh_offset))
      > (char *)data->data + data->filesize)
    return (FAILURE);
  data->str_section_header = NULL;
  data->section_symbole = NULL;
  nm64_section_header(data, 0);
  return (SUCCESS);
}
