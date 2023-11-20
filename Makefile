# Project
NAME = webserv

# Compiler
CPP			= c++
CPPFLAGS	= -Wall -Werror -Wextra -std=c++98

# Paths
SRC_PATH	= ./src/
BIN_PATH	= ./bin/

# Includes
INCLUDES	= inc/

# Clean
RM	= rm -rf

# Files
SRC	=	main.cpp \

OBJ	= $(addprefix $(BIN_PATH), $(SRC:.cpp=.o))

DEPS		= $(OBJ:.o=.d)


.cpp.o :	
		${CPP} ${CPPFLAGS} -c++ $< -o ${<:.cpp=.o}

$(BIN_PATH)%.o: $(SRC_PATH)%.c
		mkdir -p $(dir $@)
		@ $(CPP) $(CPPFLAGS) -c $< -o $@
		
${NAME} :	${OBJ}
		${CPP} -o ${NAME} -g ${CPPFLAGS} ${OBJ}
		clear
	              
all :		${NAME}

clean :
		${RM} ${BIN_PATH}

fclean :	clean
		${RM} ${NAME}

re :		fclean all

.PHONY :	all clean fclean re

-include $(DEPS)
