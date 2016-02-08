#ifndef NM_H_
# define NM_H_

# include <elf.h>
# define ERROR	-1000
# define NO_ERR	-2000
typedef struct	s_data64
{
  Elf64_Ehdr	*header;
 char		*file_name;
  Elf64_Shdr	*section_header;
  Elf64_Shdr	*section_symbole;
  Elf64_Shdr	*str_section_header;
  char		*strtab;
  Elf64_Sym	*sym;
  char		*strsym;
  void		*data;
  Elf64_Sym	**table_sym;
  off_t		filesize;
}		t_data64;

int		start_nm64(t_data64 *);
int		sort_tab(t_data64 *, int);
void		print_tab(t_data64 *);
int		should_swap(Elf64_Sym *,
			    Elf64_Sym *,
			    t_data64 *,
			    int);
#endif /* !NM_H_ */
