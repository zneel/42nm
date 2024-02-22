#ifndef FT_NM
#define FT_NM

#include "ft_printf.h"
#include "libft.h"
#include "lists.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// program flags
extern int flags;

typedef enum e_flag
{
  a = 1 << 0,
  g = 1 << 1,
  u = 1 << 2,
  r = 1 << 3,
  p = 1 << 4,
} t_flag;

typedef enum e_arch
{
  unknown,
  elf32,
  elf64,
} arch_t;

typedef struct s_nm
{
  char *filename;
  int fd;
  arch_t arch;
} t_nm;

#endif