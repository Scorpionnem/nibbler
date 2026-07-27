NAME :=	nibbler_glfw.so

CXX :=		c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 -fPIC

ROOT :=			../../
SHARED_INC :=	$(ROOT)inc/shared/
INC_DIR :=		inc/
SRC_DIR :=		src/
OBJ_DIR :=		.obj/

GLFW_CFLAGS :=	$(shell pkg-config --cflags glfw3)
GLFW_LIBS :=	$(shell pkg-config --libs glfw3)

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(SHARED_INC) $(GLFW_CFLAGS)
LFLAGS :=		$(GLFW_LIBS) -lGL

SRCS :=	$(addprefix $(SRC_DIR),			\
			GlfwGraphics.cpp			\
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

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
