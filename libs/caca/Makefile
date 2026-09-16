NAME :=	nibbler_caca.so

CXX :=		c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 -fPIC
# `pkg-config --cflags caca`

ROOT :=			../../
SHARED_INC :=	$(ROOT)inc/
INC_DIR :=		inc/
SRC_DIR :=		src/
OBJ_DIR :=		.obj/

CACA_LIBS :=	`pkg-config --libs caca`
# CACA_LIBS :=	-Llib -lcaca

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(SHARED_INC)
LFLAGS :=		$(CACA_LIBS)

SRCS :=	$(addprefix $(SRC_DIR),			\
			CACAGraphicsDL.cpp			\
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
