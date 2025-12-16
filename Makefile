CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes -I includes/shared

NAME = nibbler

SRCS =	src/main.cpp\
		src/Nibbler.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: minilibx
	@make -j compile --no-print-directory

compile: $(NAME)

glfw:
	@make -C glfw/ all --no-print-directory
	@cp glfw/glfw.so .

minilibx:
	@make -C minilibx/ all --no-print-directory
	@cp minilibx/minilibx.so .

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
	@make -C minilibx clean
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)
	@make -C minilibx fclean
	@rm -rf minilibx.so

.PHONY: all clean fclean run re glfw minilibx

-include $(DEPS)
