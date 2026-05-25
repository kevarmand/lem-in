SRCS	= ft_atoi.c\
		  ft_bzero.c\
		  ft_calloc.c\
		  ft_extremum.c\
		  ft_isalnum.c\
		  ft_isalpha.c\
		  ft_isascii.c\
		  ft_isdigit.c\
		  ft_isprint.c\
		  ft_itoa.c\
		  ft_memchr.c\
		  ft_memcmp.c\
		  ft_memcpy.c\
		  ft_memmove.c\
		  ft_memset.c\
		  ft_putchar_fd.c\
		  ft_putendl_fd.c\
		  ft_putnbr_fd.c\
		  ft_putstr_fd.c\
		  ft_split.c\
		  ft_strchr.c\
		  ft_strcmp.c\
		  ft_strdup.c\
		  ft_striteri.c\
		  ft_strjoin.c\
		  ft_strlcat.c\
		  ft_strlcpy.c\
		  ft_strlen.c\
		  ft_strmapi.c\
		  ft_strncmp.c\
		  ft_strnstr.c\
		  ft_strrchr.c\
		  ft_strtrim.c\
		  ft_substr.c\
		  ft_tolower.c\
		  ft_toupper.c\
		  gnl/get_next_line.c\
		  gnl/get_next_line_utils.c\
		  hashmap/hashmap_get.c\
		  hashmap/hashmap_hashf.c\
		  hashmap/hashmap_memory.c\
		  hashmap/hashmap_insert.c\
		  hashmap/hashmap_remove.c\
		  hashmap/hashmap_rehash.c\
		  vector.c

OBJS	= $(SRCS:.c=.o)

SBONUS	= ft_lstadd_back_bonus.c\
		  ft_lstadd_front_bonus.c\
		  ft_lstclear_bonus.c\
		  ft_lstdelone_bonus.c\
		  ft_lstiter_bonus.c\
		  ft_lstlast_bonus.c\
		  ft_lstmap_bonus.c\
		  ft_lstnew_bonus.c\
		  ft_lstsize_bonus.c

SRC_PRINT	= src_printf/ft_printf.c\
			  src_printf/parse.c\
			  src_printf/parse_utils.c\
			  src_printf/print.c\
			  src_printf/print2.c\
			  src_printf/print_bonus.c\
			  src_printf/print_utils.c\
			  src_printf/utils.c\
			  src_printf/utils2.c

OBONUS		= $(SBONUS:.c=.o)
OBJS_PRINT	= $(SRC_PRINT:.c=.o)

NAME	= libft.a

FLAGS	= -Wall -Wextra -Werror -I. -Ignl

.c.o	:
	@cc $(FLAGS) -c $< -o ${<:.c=.o}

$(NAME) : $(OBJS) $(OBJS_PRINT)
	@ar -rcs $@ $^

bonus	: $(NAME) $(OBJS) $(OBONUS)
	ar -rcs $(NAME) $^

all	: $(NAME)	

clean	:
	rm -f $(OBJS) $(OBONUS) $(OBJS_PRINT)

fclean	:	clean
		rm -f $(NAME)

re	:	fclean all

.PHONY:	all clean fclean re bonus
