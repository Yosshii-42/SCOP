UNAME_S := $(shell uname -s)

NAME	= SCOP
NAME_B  = SCOP_bonus

CXX		= c++
CC		= cc

GLAD_DIR = glad

CXXFLAGS	= -Wall -Wextra -Werror -O3 -std=c++17 \
			  -I./include \
			  -I$(GLAD_DIR)/include \
			  -MMD -MP

CFLAGS		= -Wall -Wextra -Werror -O3 \
			  -I./include \
			  -I$(GLAD_DIR)/include

# macOSとLinuxでライブラリを分ける
ifeq ($(UNAME_S), Darwin)
	LIBS	= -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
	LIBS	= $(shell pkg-config --libs glfw3) -lGL -ldl -lpthread
endif

SRCS	= $(GLAD_DIR)/src/glad.c

SRC_M  	= srcs/main.cpp
SRC_B  	= srcs/main_bonus.cpp

OBJS	= $(SRCS:.cpp=.o)
OBJS	:= $(OBJS:.c=.o)

OBJ_M	= $(SRC_M:.cpp=.o)
OBJ_B	= $(SRC_B:.cpp=.o)

DEPS	= $(OBJS:.o=.d) \
		  $(OBJ_M:.o=.d) \
		  $(OBJ_B:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(OBJ_M)
	$(CXX) $(CXXFLAGS) $(OBJS) $(OBJ_M) $(LIBS) -o $(NAME)

$(NAME_B): $(OBJS) $(OBJ_B)
	$(CXX) $(CXXFLAGS) $(OBJS) $(OBJ_B) $(LIBS) -o $(NAME_B)

$(GLAD_DIR)/src/%.o: $(GLAD_DIR)/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJ_M) $(OBJ_B) $(DEPS)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean 
	$(MAKE) all

-include $(DEPS)
