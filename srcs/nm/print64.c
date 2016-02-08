#include <stdio.h>
#include "shared.h"
#include "nm.h"

static char	globaltype(Elf64_Sym *sym,
			   Elf64_Shdr *section_header,
			   t_data64 *data)
{
  if ((void *)&section_header[sym->st_shndx].sh_type
      > data->data + data->filesize)
    return ('?');
  if (section_header[sym->st_shndx].sh_type == SHT_NOBITS)
    return ('B');
  if (section_header[sym->st_shndx].sh_type == SHT_PROGBITS)
    {
      if (section_header[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
	return ('D');
      if (section_header[sym->st_shndx].sh_flags
	  == (SHF_ALLOC | SHF_EXECINSTR))
	return ('T');
      return ('R');
    }
  if (sym->st_shndx == SHN_UNDEF)
    return ('U');
  if (sym->st_shndx == SHN_COMMON)
    return ('C');
  if (sym->st_shndx == SHN_ABS)
    return ('A');
  if (section_header[sym->st_shndx].sh_type == SHT_DYNAMIC)
    return ('D');
  return ('?');
}

static char	localtype(Elf64_Sym *sym,
			  Elf64_Shdr *section_header,
			  t_data64 *data)
{
  if (globaltype(sym, section_header, data) != '?')
    return (globaltype(sym, section_header, data) + 32);
  if ((void *)&section_header[sym->st_shndx].sh_flags
      > data->data + data->filesize)
    return ('?');
  if (section_header[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
    return ('t');
  if (section_header[sym->st_shndx].sh_flags == SHT_NULL)
    return ('n');
  return ('.');
}

static char	weaktype(Elf64_Sym *sym, t_data64 *data)
{
  if ((void *)&sym->st_shndx > data->data + data->filesize
      || (void *)&sym->st_info > data->data + data->filesize)
    return ('?');
  if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
    {
      if (sym->st_shndx != SHN_UNDEF)
	return ('V');
      return ('v');
    }
  if (sym->st_shndx != SHN_UNDEF)
    return ('W');
  return ('w');
}

static char	gettype(Elf64_Sym *sym, t_data64 *data)
{
  if ((void *)&sym->st_info > data->data + data->filesize)
    return (' ');
  if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
    return ('u');
  if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
    return (globaltype(sym, data->section_header, data));
  else if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL)
    return (localtype(sym, data->section_header, data));
  else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
    return (weaktype(sym, data));
  return (' ');
}

void		print_tab(t_data64 *data)
{
  uint32_t	i;
  char		c;

  i = 0;
  while (data->table_sym[i] != NULL)
    {
      c = gettype(data->table_sym[i], data);
      if (c == 'U' || c == 'w'
	  || (void *)&data->table_sym[i]->st_value
	  > data->data + data->filesize)
	printf("                 ");
      else
	printf("%016x ", (uint32_t)data->table_sym[i]->st_value);
      printf("%c ", c);
      if ((void *)&data->strsym[data->table_sym[i]->st_name]
	  < data->data + data->filesize)
	printf("%s\n", &data->strsym[data->table_sym[i]->st_name]);
      ++i;
    }
}
