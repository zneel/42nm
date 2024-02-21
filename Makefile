NAME	:= ft_nm
CC 		:= clang
CFLAGS	:= -Wall -Wextra -Werror -I./includes -MMD
SRCS	:= main.c


OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.d)

INCLUDES := -Iincludes -Ilibft/includes
LIBS := -lft -Llibft
LIBRARY := libft/libft.a
LIBFT_PATH := libft

ifeq ($(DEBUG), 1)
	CFLAGS+=-g3
endif

ifeq ($(SAN), 1)
	CFLAGS+=-fsanitize=address
endif

ifeq ($(DEV), 1)
	CFLAGS+=-fsanitize=address -g3
endif

all : $(NAME)

$(NAME): $(LIBRARY) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBRARY) $(LIBS)
	@echo "Done!"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBRARY):
	make -j -sC $(LIBFT_PATH)

clean :
	rm -f $(OBJS)
	rm -f $(DEPS)
	make -sC libft clean
	@echo "Done!"

fclean : clean
	rm -f $(NAME)
	make -sC libft fclean
	@echo "Done!"

re : fclean all

-include $(DEPS)

.PHONY : all clean fclean re