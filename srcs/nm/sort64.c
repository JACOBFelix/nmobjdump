#include <stdlib.h>
#include "nm.h"
#include "shared.h"

static void		my_swap_sym(Elf64_Sym **first, Elf64_Sym **second)
{
  Elf64_Sym		*third;

  third = *first;
  *first = *second;
  *second = third;
}

int		sort_tab(t_data64 *data, int i)
{
  while (data->table_sym[i] != NULL)
    {
      if ((void *)data->table_sym[i] + sizeof(Elf64_Sym)> data->data
	  + data->filesize)
	return (FAILURE);
      if (data->table_sym[i + 1] != NULL && (void *)&data->table_sym[i + 1]
	  + sizeof(Elf64_Sym) > data->data + data->filesize)
	return (FAILURE);
      if (data->table_sym[i + 1] != NULL
	  && should_swap(data->table_sym[i],
			 data->table_sym[i + 1], data, -1) == SUCCESS)
	{
	  my_swap_sym(&data->table_sym[i], &data->table_sym[i + 1]);
	  if (i > 0)
	    i -= 1;
	}
      else if (data->table_sym[i + 1] != NULL
	       && should_swap(data->table_sym[i],
			      data->table_sym[i + 1], data, -1) == ERROR)
	return (FAILURE);
      else
	i++;
    }
  return (SUCCESS);
}
