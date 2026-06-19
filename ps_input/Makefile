NAME		= ps_input
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRC_DIR		= src
OBJ_DIR		= obj
INC_DIR		= inc
LIB_DIR		= lib

SRCS		= $(SRC_DIR)/gen_main.c $(SRC_DIR)/gen_target.c \
			$(SRC_DIR)/gen_rank.c $(SRC_DIR)/gen_agents.c \
			$(SRC_DIR)/gen_number.c $(SRC_DIR)/gen_text.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
INCLUDES	= -I $(INC_DIR)
LIBS		= $(LIB_DIR)/libft.a -lm

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
