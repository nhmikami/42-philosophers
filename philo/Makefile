NAME			=	philo
RM				=	rm -f
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -pthread

SRC_PATH		=	./src
INC_PATH		=	./include

SRC				=	main.c \
					get_args.c \
					init_data.c \
					monitor_routine.c \
					philo_routine.c \
					simulation.c \
					utils.c

OBJ				=	$(addprefix $(SRC_PATH)/, $(SRC:.c=.o))

INC				=	-I$(INC_PATH)

all:				$(NAME)

$(NAME):			$(OBJ)
						$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC_PATH)/philo.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
					$(RM) $(OBJ)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

.PHONY:		all clean fclean re