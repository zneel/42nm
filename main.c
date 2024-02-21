#include "ft_nm.h"
#include <stdlib.h>

int flags;

int
open_file (char *filename)
{
  int fd;

  fd = open (filename, O_RDONLY);
  if (fd < 0)
    {
      if (errno == EACCES)
        return -1;
      else
        {
          ft_dprintf (STDERR_FILENO, "ft_nm: '%s': %s\n", filename,
                      "No such file");
          return -1;
        }
    }
  return fd;
}

void
parse_flags (char *flag)
{
  if (ft_strncmp ("-a", flag, 2))
    flags |= a;
  if (ft_strncmp ("-g", flag, 2))
    flags |= g;
  if (ft_strncmp ("-u", flag, 2))
    flags |= u;
  if (ft_strncmp ("-r", flag, 2))
    flags |= r;
  if (ft_strncmp ("-p", flag, 2))
    flags |= p;
}

void
parse_nm_input (int ac, char **av)
{
  for (int i = 1; i < ac; ++i)
    {
      if (av[i][0] == '-')
        {
          parse_flags (av[i]);
          continue;
        }
    }
}

void
print_error (char *filename, char *error)
{
  ft_dprintf (STDERR_FILENO, "ft_nm: '%s': %s\n", filename, error);
}

int
list_symbols (t_nm nm)
{
  struct stat st;
  char *ptr;
  if (nm.fd < 0)
    return EXIT_FAILURE;
  if (fstat (nm.fd, &st) < 0)
    {
      print_error (nm.filename, strerror (errno));
      return EXIT_FAILURE;
    }
  if ((ptr = mmap (0, st.st_size, PROT_READ, MAP_PRIVATE, nm.fd, 0)) < 0)
    {
      print_error (nm.filename, strerror (errno));
      return EXIT_FAILURE;
    }
  // execute nm with nm.filename
  return EXIT_SUCCESS;
}

int
main (int ac, char **av)
{
  int last_ret = EXIT_SUCCESS;
  if (ac <= 1)
    {
      t_nm nm = {
        .filename = "a.out",
        .fd = open_file ("a.out"),
      };
      if (nm.fd < 0)
        return EXIT_FAILURE;
      // exectute nm with a.out
    }
  if (ac >= 2)
    {
      parse_nm_input (ac, av);
      for (int i = 1; i < ac; ++i)
        {
          if (av[i][0] == '-')
            continue;
          t_nm nm = {
            .filename = av[i],
            .fd = open_file (av[i]),
          };
          last_ret = list_symbols (nm);
        }
    }
  return last_ret;
}