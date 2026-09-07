NAME :=	server

CXX :=		c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3

ROOT :=			../
SHARED_INC :=	$(ROOT)inc/
INC_DIR :=		inc/
SRC_DIR :=		src/
OBJ_DIR :=		.obj/

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(SHARED_INC)
LFLAGS :=

SRCS :=	$(addprefix $(SRC_DIR),			\
			main.cpp				\
		)

OBJS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

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
