NAME :=	Nibbler
SERVERNAME :=	Nibbler_server

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

SDL_AU := libs/sdl_audio/nibbler_sdl_audio.so
SDL_SO := libs/sdl/nibbler_sdl.so
CACA_SO := libs/caca/nibbler_caca.so
MBATTY_SO := libs/mbatty/mbattylib.so

all: $(SDL_SO) $(CACA_SO) $(SDL_AU) $(MBATTY_SO) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(SDL_AU):
	@make -C libs/sdl_audio/ all --no-print-directory

$(SDL_SO):
	@make -C libs/sdl/ all --no-print-directory

$(CACA_SO):
	@make -C libs/caca/ all --no-print-directory

$(MBATTY_SO):
	@make -C libs/mbatty/ all --no-print-directory

server:
	@make -C server/ all --no-print-directory
	@mv -T server/server Nibbler_server

clean:
	@make -C libs/sdl_audio/ clean --no-print-directory
	@make -C libs/sdl/ clean --no-print-directory
	@make -C libs/caca/ clean --no-print-directory
	@make -C libs/mbatty/ clean --no-print-directory
	@make -C server/ clean --no-print-directory
	rm -rf $(OBJ_DIR)

fclean:
	@make -C libs/sdl_audio/ fclean --no-print-directory
	@make -C libs/sdl/ fclean --no-print-directory
	@make -C libs/caca/ fclean --no-print-directory
	@make -C libs/mbatty/ fclean --no-print-directory
	@make -C server/ fclean --no-print-directory
	rm -rf $(OBJ_DIR)
	rm -rf $(NAME)
	rm -rf $(SERVERNAME)

re: fclean all

.PHONY: all server clean fclean re

-include $(DEPS)
