#include <string.h>
#include <stdio.h>
#include "shared.h"
#include "objdump.h"

static int	check_header(t_objdata *data)
{
  if (data->header->e_ident[EI_MAG0] != ELFMAG0
      || data->header->e_ident[EI_MAG1] != ELFMAG1
      || data->header->e_ident[EI_MAG2] != ELFMAG2
      || data->header->e_ident[EI_MAG3] != ELFMAG3
      || (data->header->e_type != ET_EXEC
	  && data->header->e_type != ET_DYN
	  && data->header->e_type != ET_CORE))
    return (FAILURE);
  if (((void *)(data->section
		= (Elf64_Shdr *)(data->data + data->header->e_shoff)))
      > data->data + data->filesize)
    return (FAILURE);
  if (((void *)(data->strtab = (char *)data->data
		+ data->section[data->header->e_shstrndx].sh_offset))
      > data->data + data->filesize)
    return (FAILURE);
  return (SUCCESS);
}

static int	print_start(t_objdata *data, char const *const mach)
{
  if ((void *)&data->header->e_flags > data->data + data->filesize
      || (void *)&data->header->e_entry > data->data + data->filesize)
    return (FAILURE);
  printf("\n%s:\tfile format elf64-x86-64\narchitecture: %s, flags 0x%08x\n",
	 data->file_name, mach, data->header->e_flags);
  printf("start adress 0x%016lx\n\n", data->header->e_entry);
  return (SUCCESS);
}

static int	follow_print_machine(t_objdata *data, const t_mach mach[])
{
  int		i;

  i = -1;
  if ((void *)&data->header->e_machine > data->data + data->filesize)
    return (FAILURE);
  while (++i < 19)
    if (data->header->e_machine == mach[i].e_mach)
      return (print_start(data, mach[i].name));
  return (FAILURE);
}

static int		print_machine(t_objdata *data)
{
  static const t_mach	mach[19] = {
    {EM_NONE, "An unknown machine."},
    {EM_M32, "AT&T WE 32100."},
    {EM_SPARC, "Sun Microsystems SPARC."},
    {EM_386, "Intel 80386."},
    {EM_68K, "Motorola 68000."},
    {EM_88K, "Motorola 88000."},
    {EM_860, "Intel 80860."},
    {EM_MIPS,
     "MIPS RS3000 (big-endian only)."},
    {EM_PARISC, "HP/MA."},
    {EM_SPARC32PLUS,
     "SPARC with enhanced instruction set."},
    {EM_PPC, "PowerPC."},
    {EM_PPC64, "PowerPC 64-bit."},
    {EM_S390, "IBM S/390."},
    {EM_ARM, "Advanced RISH Machines."},
    {EM_SH, "Renesas SuperH."},
    {EM_SPARCV9, "SPARC v9 64-bit."},
    {EM_IA_64, "Intel Itanium."},
    {EM_X86_64, "AMD x86-64."},
    {EM_VAX, "DEC Vax."}};

  return (follow_print_machine(data, mach));
}

void	start_obj(t_objdata *data)
{
  if (check_header(data) == FAILURE || print_machine(data) == FAILURE)
    return ;
  try_obj(data);
}
