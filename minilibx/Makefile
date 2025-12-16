CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fPIC

INCLUDES = -I includes/ -I ../includes/shared -I $(EXTERNALDIR)/$(MLXDIR)/

EXTERNALDIR = external
MLXDIR = minilibx-linux
MLX = $(EXTERNALDIR)/$(MLXDIR)/libmlx.a

NAME = minilibx.so

SRCS = src/entrypoint.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: minilibx
	make -j compile

compile: $(MLX) $(NAME)

$(EXTERNALDIR):
	@mkdir -p $(EXTERNALDIR)

$(MLX): $(EXTERNALDIR)
	@make -C $(EXTERNALDIR)/$(MLXDIR) all --no-print-directory

minilibx: $(EXTERNALDIR)
	@if ls $(EXTERNALDIR) | grep -q "$(MLXDIR)"; then \
		echo "\033[32;1;4mminilibx-linux Found\033[0m"; \
	else \
		echo "\033[31;1;4mminilibx-linux Not Found\033[0m"; \
		echo "\033[31;1mDownloading minilibx-linux from github \033[0m"; \
		git clone https://github.com/42paris/minilibx-linux.git $(EXTERNALDIR)/$(MLXDIR); \
	fi

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -shared $(MLX) -lXext -lX11 -lm -lz

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)
	@rm -rf $(EXTERNALDIR)

.PHONY: all clean fclean run re

-include $(DEPS)
