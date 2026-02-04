CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes -I includes/shared

NAME = nibbler

SRCS =	src/main.cpp\
		src/Nibbler/dl.cpp\
		src/Nibbler/game.cpp\
		src/Nibbler/parsing.cpp\
		src/Nibbler/main.cpp\

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: sdl glfw
	@make -j compile --no-print-directory

compile: $(NAME)

glfw:
	@make -C glfw/ all --no-print-directory
	@cp glfw/glfw.so .

sdl:
	@make -C sdl/ all --no-print-directory
	@cp sdl/sdl.so .

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning objects
	@make -C glfw clean --no-print-directory
	@make -C sdl clean --no-print-directory
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@make -C glfw fclean --no-print-directory
	@make -C sdl fclean --no-print-directory
	@rm -rf $(NAME)

.PHONY: all clean fclean run re glfw sdl

-include $(DEPS)
