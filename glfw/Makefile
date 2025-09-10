CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fPIC

INCLUDES = -I includes/ -I ../includes/ -I ./GLFW/include/GLFW/ -I ./includes/glad/. -I ./glm/glm/ -I ./glm/glm/gtc/

NAME = glfw.so

SRCS = src/entrypoint.cpp src/GLFWGraphicsHandler.cpp src/glad/glad.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

GLFWARCHIVE = GLFW/build/src/libglfw3.a

all: glfw glad glm $(NAME)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(GLFWARCHIVE) -shared

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

run: $(NAME)
	./$(NAME) avm/sample.avm

glfw:
	@if ls | grep -q "GLFW"; then \
		echo "\033[32;1;4mGLFW Found\033[0m"; \
	else \
		echo "\033[31;1;4mGLFW Not Found\033[0m"; \
		echo "\033[31;1mDownloading GLFW from github \033[0m"; \
		git clone https://github.com/glfw/glfw.git GLFW; \
		echo "\033[31;1mCompiling GLFW\033[0m"; \
		cmake -S GLFW -B GLFW/build; \
		cmake --build GLFW/build; \
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

.PHONY: all clean fclean run re glfw glad glm

-include $(DEPS)
