NAME	= ps_check

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iinc

SRCS	= checker_main_bonus.c checker_bonus.c checker_utils_bonus.c \
		  checker_op_ps_bonus.c checker_op_r_bonus.c checker_op_rr_bonus.c

OBJ_DIR	= obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIB_DIR		= lib
PUSH_SWAP	= $(LIB_DIR)/push_swap.a
LIBFT		= $(LIB_DIR)/libft.a

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(PUSH_SWAP) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
