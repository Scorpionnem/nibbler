NAME :=	nibbler_sdl.so

CXX :=		c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 -fPIC

ROOT :=			../../
SHARED_INC :=	$(ROOT)inc/
INC_DIR :=		inc/
SRC_DIR :=		src/
OBJ_DIR :=		.obj/

SDL_CFLAGS :=	$(shell sdl2-config --cflags)
SDL_LIBS :=		$(shell sdl2-config --libs)

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(SHARED_INC) $(SDL_CFLAGS)
LFLAGS :=		$(SDL_LIBS)

SRCS :=	$(addprefix $(SRC_DIR),			\
			SDLGraphicsDL.cpp			\
			entrypoint.cpp				\
		)

OBJS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LFLAGS)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
