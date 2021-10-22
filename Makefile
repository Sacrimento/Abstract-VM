NAME		=	avm
FLAGS		= 	-std=c++17 -Wall -Werror -Wextra
COMPILER	=	clang++

DIR_INC		=	./inc/
DIR_SRC		=	./src/
DIR_OBJ		= 	./obj/

HEAD_AVM	=	AVM.h \
				IOperand.h \
				Operand.h \
				parser.h \
				VM.h

SRC_AVM		=	main.cpp \
				parser.cpp \
				VM.cpp

INC_PATH 	= 	$(addprefix $(DIR_INC), $(HEAD_AVM))

OBJ 		= 	$(addprefix $(DIR_OBJ), $(SRC_AVM:.cpp=.o))
INC 		= 	$(addprefix -I, $(DIR_INC))

.PHONY: all obj $(NAME) clean fclean re

all: obj $(NAME)

obj:
	@mkdir -p $(DIR_OBJ)

$(NAME): $(OBJ)
	@$(COMPILER) -o $(NAME) $(OBJ)

$(DIR_OBJ)%.o: $(DIR_SRC)%.cpp $(INC_PATH)
	@$(COMPILER) $(FLAGS) $(INC) -c -o $@ $<

clean:
	@rm -rf $(DIR_OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all