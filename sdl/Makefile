NAME := ft_vox

OBJ_DIR := ./obj/
INCLUDE_DIRS := ./includes/ ./SDL/include/ ./includes/glad/. ./src/ ./glm/glm/ ./glm/glm/gtc/
CLASSE_HEADERS := Window/ Shader/

SDLFLAGS = -L./SDL/build -lSDL2 -lGL

INCLUDE_DIRS := $(addprefix -I, $(INCLUDE_DIRS))
CLASSE_HEADERS := $(addprefix -I./src/, $(CLASSE_HEADERS))

MAKE := @make --no-print-directory

SOURCE_DIR := ./src/

OBJ_DIR := ./obj/

OBJECTS = $(SOURCES:.cpp=.o)

CPP_FILES :=	main \
				glad/glad \
				Window/Window \
				Shader/Shader \
				Shader/ShaderManager

CPP_FILES := $(addsuffix .cpp, $(CPP_FILES))

SOURCES := $(addprefix $(SOURCE_DIR), $(CPP_FILES))

OBJECTS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.o))
DEPS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.d))

CFLAGS = -MP -MMD -g -Wall -Wextra -Werror

GLAD_PATH = libs/glad

all: sdl glad glm $(NAME)

run: all
	@./$(NAME) 1000000

vrun: all
	@valgrind ./$(NAME)

SDL_VERSION = release-2.30.5

sdl:
	@if [ -d "SDL" ]; then \
		echo "\033[32;1;4mSDL Found\033[0m"; \
	else \
		echo "\033[31;1;4mSDL Not Found\033[0m"; \
		echo "\033[33;1mDownloading SDL from GitHub\033[0m"; \
		git clone https://github.com/libsdl-org/SDL.git SDL && \
		cd SDL && \
		git checkout $(SDL_VERSION) && \
		echo "\033[34;1mCompiling SDL\033[0m"; \
		mkdir build && cd build && \
		cmake .. -DCMAKE_BUILD_TYPE=Release -DSDL_STATIC=OFF -DSDL_SHARED=ON && \
		cmake --build . --config Release; \
	fi

glad:
	@if ls src | grep -q "glad"; then \
		echo "\033[32;1;4mglad Found\033[0m"; \
	else \
		echo "\033[31;1;4mglad Not Found\033[0m"; \
		echo "\033[31;1mDownloading glad from github \033[0m"; \
		pip install glad; \
		git clone https://github.com/Dav1dde/glad.git glad; \
		python -m glad --out-path=glad/build --generator=c; \
		mkdir -p glad2; \
		cp glad/build/include/glad/glad.h glad2/.; \
		cp glad/build/src/glad.c glad2/.; \
		rm -rf glad; \
		mv glad2 glad; \
		mv glad/glad.c glad/glad.cpp; \
		mkdir -p src/glad; \
		mkdir -p includes/glad; \
		mv glad/glad.cpp src/glad/; \
		mv glad/glad.h includes/glad/; \
		rm -rf glad; \
	fi

glm:
	@if ls | grep -q "glm"; then \
		echo "\033[32;1;4mGLM Found\033[0m"; \
	else \
		echo "\033[31;1;4mGLM Not Found\033[0m"; \
		echo "\033[31;1mCloning GLM from github\033[0m"; \
		git clone https://github.com/g-truc/glm.git glm; \
	fi

$(OBJECTS): $(OBJ_DIR)%.o : $(SOURCE_DIR)%.cpp
	@echo $<
	@c++ $(CFLAGS) $(CLASSE_HEADERS) $(INCLUDE_DIRS) -c $< -o $@

$(OBJ_DIR):
	@(cd $(SOURCE_DIR) && find . -type d -exec mkdir -p -- $(shell pwd)/$(OBJ_DIR){} \;)

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@c++ $(OBJECTS) $(SDLFLAGS) -o $(NAME)
	@echo "\033[0;32mCompiled $(NAME)\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;32mCleaned objects\033[0m"

fclean:
	$(MAKE) clean
	@rm -f $(NAME)
	@echo "\033[0;32mCleaned $(NAME)\033[0m"

dclean: fclean
	@rm -rf src/glad
	@rm -rf includes/glad
	@rm -rf SDL
	@rm -rf glm
	@echo "\033[0;32mCleaned external libraries\033[0m"

re: fclean all

.PHONY: all clean fclean dclean re SDL glad run glm sdl

-include $(DEPS)
