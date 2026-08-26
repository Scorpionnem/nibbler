NAME :=	Nibbler

CXX :=		c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 -pthread

INC_DIR :=	inc/
SRC_DIR :=	src/
OBJ_DIR :=	.obj/

INCLUDE_DIRS :=	-I$(INC_DIR)
LFLAGS :=		-ldl

SRCS :=	$(addprefix $(SRC_DIR),			\
			main.cpp					\
			Nibbler.cpp					\
		)

OBJS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.d)

SDL_SO := libs/sdl/nibbler_sdl.so
CACA_SO := libs/caca/nibbler_caca.so
# GLFW_SO := libs/glfw/nibbler_glfw.so

all: $(SDL_SO) $(CACA_SO) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(SDL_SO):
	@make -C libs/sdl/ all --no-print-directory

$(CACA_SO):
	@make -C libs/caca/ all --no-print-directory

clean:
	@make -C libs/sdl/ clean --no-print-directory
	@make -C libs/caca/ clean --no-print-directory
	rm -rf $(OBJ_DIR)

fclean:
	@make -C libs/sdl/ fclean --no-print-directory
	@make -C libs/caca/ fclean --no-print-directory
	rm -rf $(OBJ_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
