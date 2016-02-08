#include <string.h>
#include <stdlib.h>
#include "nm.h"
#include "shared.h"

static char		must_swap(char a, char b)
{
  if (a >= 'a' && a <= 'z')
    a -= 32;
  if (b >= 'a' && a <= 'z')
    b -= 32;
  if (a >= 'A' && a <= 'Z' && b >= 'A' && b <= 'Z')
    return (a - b);
  return (a - b);
}

static int	check_swap_follow(char *first, char *second, int f, int s)
{
  char		ret;

  while (first[f] && second[s])
    {
      while (first[f] == '_' || first[f] == '@')
	++f;
      while (second[s] == '_' || second[s] == '@')
	++s;
      if (first[f] == 0)
	return (FAILURE);
      if (second[s] == 0)
	return (SUCCESS);
      if ((ret = must_swap(first[f], second[s])) < 0)
      	return (FAILURE);
      else if (ret > 0)
      	return (SUCCESS);
      ++f;
      ++s;
    }
  if (first[f] == 0)
    return (FAILURE);
  return (SUCCESS);
}

static int	check_swap(char *first, char *second, t_data64 *data)
{
  int		f;
  int		s;

  f = -1;
  s = -1;
  while (first[++f])
    if ((void *)&first[f] > data->data + data->filesize)
      return (ERROR);
  while (second[++s])
    if ((void *)&second[s] > data->data + data->filesize)
      return (ERROR);
  f = -1;
  while (first[++f] == '_')
    ;
  s = -1;
  while (second[++s] == '_')
    ;
  return (check_swap_follow(first, second, f, s));
}

int		should_swap(Elf64_Sym *first_sym,
			    Elf64_Sym *sec_sym,
			    t_data64 *data,
			    int f)
{
  char		*first;
  char		*second;
  int		s;

  s = -1;
  if ((first = &data->strsym[first_sym->st_name])
      >= (char *)data->data + data->filesize)
    return (ERROR);
  if ((second = &data->strsym[sec_sym->st_name])
      >= (char *)data->data + data->filesize)
    return (ERROR);
  while (first[++f] == '_' && &first[f] < (char *)data->data + data->filesize)
    ;
  while (second[++s] == '_' && &second[s]
	 < (char *)data->data + data->filesize)
    ;
  if (&second[s] >=  (char *)data->data + data->filesize ||
      &first[f] >= (char *)data->data + data->filesize)
    return (ERROR);
  if (strcmp(&first[f], &second[s]) == 0 && s > f)
    return (SUCCESS);
  if (strcmp(&first[f], &second[s]) == 0)
    return (FAILURE);
  return (check_swap(first, second, data));
}
