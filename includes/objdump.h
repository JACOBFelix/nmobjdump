#ifndef OBJDUMP_H_
# define OBJDUMP_H_

#include <stdlib.h>
#include <elf.h>
typedef struct	s_mach
{
  uint16_t	e_mach;
  char		*name;
}		t_mach;

typedef struct	s_objdata
{
  char		*file_name;
  off_t		filesize;
  void		*data;
  Elf64_Ehdr	*header;
  Elf64_Shdr	*section;
  char		*strtab;
}		t_objdata;

void		start_obj(t_objdata *);
void		try_obj(t_objdata *);

#endif /* !OBJDUMP_H_ */
