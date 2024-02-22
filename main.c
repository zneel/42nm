#include "ft_nm.h"

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
usage ()
{
  ft_dprintf (STDERR_FILENO, "Usage: %s [option(s)] [file(s)]\n", "ft_nm");
  ft_dprintf (STDERR_FILENO,
              "List symbols in [file(s)] (a.out by default).\n");
  ft_dprintf (STDERR_FILENO, "The options are:\n\
  -a, --debug-syms       Display debugger-only symbols\n\
  -g, --extern-only      Display only external symbols\n\
  -u, --undefined-only   Display only undefined symbols\n\
  -r, --reverse-sort     Reverse the sense of the sort\n\
  -p, --no-sort          Do not sort the symbols\n");
}

void
parse_flags (char *flag)
{
  if (ft_strncmp ("-a", flag, 2) == 0 && ft_strlen (flag) == 2)
    flags |= a;
  else if (ft_strncmp ("-g", flag, 2) == 0 && ft_strlen (flag) == 2)
    flags |= g;
  else if (ft_strncmp ("-u", flag, 2) == 0 && ft_strlen (flag) == 2)
    flags |= u;
  else if (ft_strncmp ("-r", flag, 2) == 0 && ft_strlen (flag) == 2)
    flags |= r;
  else if (ft_strncmp ("-p", flag, 2) == 0 && ft_strlen (flag) == 2)
    flags |= p;
  else
    {
      ft_dprintf (STDERR_FILENO, "ft_nm: invalid option -- '%s'\n", flag);
      usage ();
      exit (EXIT_FAILURE);
    }
  ft_printf ("a=%d\n", flags & a);
  ft_printf ("g=%d\n", flags & g);
  ft_printf ("u=%d\n", flags & u);
  ft_printf ("r=%d\n", flags & r);
  ft_printf ("p=%d\n", flags & p);
}

void
parse_nm_flags (int ac, char **av)
{
  for (int i = 1; i < ac; ++i)
    if (av[i][0] == '-')
      parse_flags (av[i]);
}

void
print_error (char *filename, char *error)
{
  ft_dprintf (STDERR_FILENO, "ft_nm: '%s': %s\n", filename, error);
}

bool
check_file (t_nm *nm, Elf64_Ehdr *ehdr)
{
  if (ehdr->e_ident[EI_MAG0] != ELFMAG0 || ehdr->e_ident[EI_MAG1] != ELFMAG1
      || ehdr->e_ident[EI_MAG2] != ELFMAG2
      || ehdr->e_ident[EI_MAG3] != ELFMAG3)
    {
      print_error (nm->filename, "file format not recognized");
      return false;
    }
  else if (ehdr->e_ident[EI_CLASS] != ELFCLASS32
           && ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
      print_error (nm->filename, "Not a 32-bit or 64-bit ELF file");
      return false;
    }
  else if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB)
    {
      print_error (nm->filename, "Not a little-endian ELF file");
      return false;
    }
  return true;
}

void
dump (t_nm *nm, char *ptr)
{
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;
  // Elf32_Ehdr *ehdr32 = (Elf32_Ehdr *)ptr;
  if (!check_file (nm, ehdr))
    return;
  nm->arch = ehdr->e_ident[EI_CLASS] == ELFCLASS32 ? elf32 : elf64;
  if (nm->arch == elf32)
    {
      ft_printf ("32-bit\n");
    }
  else
    {
      ft_printf ("64-bit\n");
    }
  return;
}

int
list_symbols (t_nm *nm)
{
  struct stat st;
  char *ptr;
  if (nm->fd < 0)
    return EXIT_FAILURE;
  if (fstat (nm->fd, &st) < 0)
    {
      print_error (nm->filename, strerror (errno));
      return EXIT_FAILURE;
    }
  ptr = mmap (0, st.st_size, PROT_READ, MAP_PRIVATE, nm->fd, 0);
  if (ptr == MAP_FAILED)
    {
      print_error (nm->filename, strerror (errno));
      return EXIT_FAILURE;
    }
  dump (nm, ptr);
  return EXIT_SUCCESS;
}

int
main (int ac, char **av)
{
  int ret = EXIT_SUCCESS;
  if (ac == 1)
    return EXIT_FAILURE;
  parse_nm_flags (ac, av);
  if (ac <= 1)
    {
      t_nm nm = {
        .filename = "a.out",
        .fd = open_file ("a.out"),
        .arch = unknown,
      };
      if (nm.fd < 0)
        return EXIT_FAILURE;
      ret = list_symbols (&nm);
    }
  if (ac >= 2)
    {
      for (int i = 1; i < ac; ++i)
        {
          if (av[i][0] == '-')
            continue;
          t_nm nm = {
            .filename = av[i],
            .fd = open_file (av[i]),
            .arch = unknown,
          };
          ret = list_symbols (&nm);
        }
    }
  return ret;
}