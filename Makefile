
#######	VARIABLES #######

# Project
NAME		= webserv

# Compiler
CPP			= g++
FLAGS		= -Wall -Wextra -Werror -MMD -MP -std=c++98 -g3
EXTRAF		= -Wshadow #-Wno-shadow

# Paths
SRC_PATH	= ./src/
BIN_PATH	= ./bin/

# Includes
INCLUDES	= inc/

# Clean
RM			= rm -rf

# Files
SRC			= main.cpp \
			FileParser.cpp \
			ServerInfo.cpp \
			Base.cpp \
			Client.cpp \
			Location.cpp \
			utilsParsing.cpp \
			utilsPrint.cpp \
			utilsGeneral.cpp \
			#multiplexer.cpp \

OBJ			= ${addprefix ${BIN_PATH}, ${SRC:.cpp=.o}}

DEPS		= ${OBJ:.o=.d}

LEAKS		= valgrind

LF			= --leak-check=full \
        	--show-leak-kinds=all \
    		--track-origins=yes \
			--track-fds=yes \

#######	RULES #######

all : ${NAME}

db :  ${DB}

.c.o :
	@ ${CPP} ${FLAGS} ${EXTRAF} -I ${INCLUDES} -c $< -o ${<:.cpp=.o}

${BIN_PATH}%.o: ${SRC_PATH}%.cpp
	@ mkdir -p ${dir $@}
	@ echo "\n\033[97;4m🚧 compiling $@ files 🚧\033[0m\n"
	@ ${CPP} ${FLAGS} ${EXTRAF} -c $< -o $@
	@ echo "\033[32;1m★ bin OK ★\033[0m\n"

${NAME} : ${OBJ}
	@ ${CPP} -o ${NAME} -g ${FLAGS} ${EXTRAF} ${OBJ}
	@ clear
	@ toilet -f pagga.tlf --gay "Enjoy ${NAME} !"

${DB} : ${OBJ}
	@ g++ -o ${NAME} -g3 ${FLAGS} ${EXTRAF} ${OBJ}
	@ clear
	@ toilet -f pagga.tlf --gay "Enjoy ${NAME} !"

run: all
	./${NAME} "add config default file here"

bonus : all

clean :
	@ echo "\n\033[97;4m🚧 cleaning object files 🚧\033[0m\n"
	@ ${RM} ${OBJ}
	@ ${RM} ${BIN_PATH}
	@ echo "\033[32;1m★ objects cleaned ★\033[0m\n"

fclean : clean
	@ ${RM} ${NAME}

re : fclean all

.PHONY : all clean fclean re

-include ${DEPS}
