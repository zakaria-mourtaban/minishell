NAME = libft.a

CFLAGS = -Wall -Werror -Wextra -std=c99
CC = gcc

all: $(NAME); 

SRCS = ft_atoi.c ft_isalnum.c ft_isdigit.c ft_isspace.c ft_memchr.c ft_memmove.c ft_putendl_fd.c ft_split.c ft_striteri.c ft_strlcpy.c ft_strncmp.c ft_strtrim.c ft_toupper.c ft_bzero.c ft_isalpha.c ft_isprint.c ft_memcmp.c ft_memset.c ft_putnbr_fd.c ft_strchr.c ft_strjoin.c ft_strlen.c ft_strnstr.c ft_substr.c ft_calloc.c ft_isascii.c ft_itoa.c ft_memcpy.c ft_putchar_fd.c ft_putstr_fd.c ft_strdup.c ft_strlcat.c ft_strmapi.c ft_strrchr.c ft_tolower.c ft_get_next_line.c ft_strjoingnl.c ft_lstmap.c ft_lstnew.c ft_lstlast.c ft_lstsize.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstadd_back.c ft_lstadd_front.c

OBJS = $(SRCS:.c=.o)

# BONUS_SRCS = ft_lstmap.c ft_lstnew.c ft_lstlast.c ft_lstsize.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstadd_back.c ft_lstadd_front.c

# BONUS_OBJS = $(BONUS_SRCS:.c=.o);

ALL_OBJS= $(OBJS) # $(BONUS_OBJS) 

$(NAME): $(OBJS)
	ar src $(NAME) $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
# bonus: $(OBJS) $(BONUS_OBJS)
# 	$(AR) -r $(NAME) $?
clean:
	rm -f $(OBJS:.c=.o) $(BONUS_OBJS:.c=.o)
fclean: clean
	rm -f $(NAME) 
re: fclean all

.PHONY: all clean fclean re bonus
