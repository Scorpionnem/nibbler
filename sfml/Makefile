CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fPIC
SDLFLAGS = -L./SDL/build -lSDL2 -lGL

INCLUDES =	-I includes/\
			-I ../includes/shared\
			-I ./$(EXTERNALDIR)/SDL/include/

EXTERNALDIR = external

NAME = sfml.so

SRCS = src/entrypoint.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: sdl
	@make -j compile --no-print-directory

compile: $(NAME)

SDL_VERSION = release-2.30.5

sdl:
	@if [ -d "$(EXTERNALDIR)/SDL" ]; then \
		echo "\033[32;1;4mSDL Found\033[0m"; \
	else \
		echo "\033[31;1;4mSDL Not Found\033[0m"; \
		echo "\033[33;1mDownloading SDL from GitHub\033[0m"; \
		git clone https://github.com/libsdl-org/SDL.git $(EXTERNALDIR)/SDL && \
		cd $(EXTERNALDIR)/SDL && \
		git checkout $(SDL_VERSION) && \
		echo "\033[34;1mCompiling SDL\033[0m"; \
		mkdir build && cd build && \
		cmake .. -DCMAKE_BUILD_TYPE=Release -DSDL_STATIC=OFF -DSDL_SHARED=ON && \
		cmake --build . --config Release; \
	fi

$(EXTERNALDIR):
	@mkdir -p $(EXTERNALDIR)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(SDLFLAGS) $(INCLUDES) -o $@ $^ -shared

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
# 	@rm -rf $(EXTERNALDIR)

.PHONY: all clean fclean run re

-include $(DEPS)
