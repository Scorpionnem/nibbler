NAME := nibbler

OBJ_DIR := ./obj/
INCLUDE_DIRS := ./includes/ ./src/ ./glm/glm/ ./glm/glm/gtc/
CLASSE_HEADERS := 

INCLUDE_DIRS := $(addprefix -I, $(INCLUDE_DIRS))
CLASSE_HEADERS := $(addprefix -I./src/, $(CLASSE_HEADERS))

MAKE := @make --no-print-directory

SOURCE_DIR := ./src/

OBJ_DIR := ./obj/

OBJECTS = $(SOURCES:.cpp=.o)

CPP_FILES :=	main \

CPP_FILES := $(addsuffix .cpp, $(CPP_FILES))

SOURCES := $(addprefix $(SOURCE_DIR), $(CPP_FILES))

OBJECTS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.o))
DEPS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.d))

CFLAGS = -MP -MMD -g -Wall -Wextra -Werror

all: $(NAME)

run: all
	@./$(NAME)

vrun: all
	@valgrind ./$(NAME)

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

re: fclean all

.PHONY: all clean fclean dclean re run 

-include $(DEPS)
