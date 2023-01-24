NAME = philo
LIBFT = libft/libft.a
SRCS = src/philo.c src/error.c src/utils.c
OBJS = $(patsubst src/%, obj/%, $(SRCS:.c=.o))
CFLAGS = -Wall -Wextra -Werror
############################
$(NAME): $(OBJS) $(LIBFT)
	gcc $(CFLAGS) -g $^ -o $@ -Llibft -lft 

$(LIBFT): 
	make bonus -C libft

all: $(NAME)

obj/%.o: src/%.c
	gcc $(CFLAGS) -c -g $< -o $@ -Ilibft -Iinc

clean:
	rm $(OBJS)

fclean: clean
	rm $(NAME)
	make fclean -C libft

re: fclean $(NAME)
