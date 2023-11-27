# Project
NAME = webserv

# Compiler
CPP		= c++
FLAGS	= -Wall -Werror -Wextra# -std=c++98

# Paths
SRC_PATH	= ./src/
BIN_PATH	= ./bin/

# Includes
INCLUDES	= inc/*.hpp

# Clean
RM	= rm -rf

# Files
SRC	= main.cpp \

OBJ	= ${addprefix ${BIN_PATH}, ${SRC:.cpp=.o}}

DEPS		= ${OBJ:.o=.d}

.c.o :	
		${CPP} ${FLAGS} -I ${INCLUDES} -c $< -o ${<:.cpp=.o}

${BIN_PATH}%.o: ${SRC_PATH}%.cpp
		mkdir -p ${dir $@}
		@ ${CPP} ${FLAGS} -c $< -o $@

${NAME} :	${OBJ}
		${CPP} -o ${NAME} -g ${FLAGS} ${OBJ} 
		clear
			@toilet -f pagga.tlf --gay "Enjoy ${NAME} !"

all :		${NAME}

clean :
		${RM} ${OBJ}

fclean :	clean
		${RM} ${BIN_PATH}
		${RM} ${NAME}

re :		fclean all

.PHONY :	all clean fclean re

-include ${DEPS}
