CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fPIC
GLFW_CFLAGS  = $(shell pkg-config --cflags glfw3)
GLFW_LDFLAGS = $(shell pkg-config --libs glfw3)

INCLUDES =	-I includes/\
			-I ../includes/shared\

EXTERNALDIR = external

NAME = glfw.so

SRCS = src/entrypoint.cpp src/GLFWDL.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all:
	@make -j compile --no-print-directory

compile: $(NAME)

$(EXTERNALDIR):
	@mkdir -p $(EXTERNALDIR)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(GLFW_LDFLAGS) $(INCLUDES) -o $@ $^ -shared

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(GLFW_CFLAGS) -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

.PHONY: all clean fclean run re

-include $(DEPS)
