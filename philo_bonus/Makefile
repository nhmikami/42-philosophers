NAME			=	philo_bonus
RM				=	rm -f
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -pthread

SRC_PATH		=	./src
INC_PATH		=	./include

SRC				=	main_bonus.c \
					get_args_bonus.c \
					init_data_bonus.c \
					libft_bonus.c \
					philo_routine_bonus.c \
					simulation_bonus.c \
					utils_bonus.c

OBJ				=	$(addprefix $(SRC_PATH)/, $(SRC:.c=.o))

INC				=	-I$(INC_PATH)

all:				$(NAME)

$(NAME):			$(OBJ)
						$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC_PATH)/philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

bonus:				all

clean:
					$(RM) $(OBJ)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

.PHONY:		all bonus clean fclean re