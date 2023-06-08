NAME = webServ

CC = c++

SANI = -fsanitize=address

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g $(SANI)

OBJ_FILE = OBJ/
FOBJ = OBJ

HEADERS = $(addprefix Headers/, Headers.hpp  webServer.hpp Server.hpp RequestClass.hpp parserObjectU.hpp CGI.hpp RESP.hpp)

SRC_FILE = SRC/
SRC =  webServer.cpp Server.cpp RequestClass.cpp parserObjectU.cpp CGI.cpp RESP.cpp main.cpp mimeTypes.cpp

OBJS = $(addprefix $(OBJ_FILE),  $(SRC:%.cpp=%.o))

all:	$(NAME)
	@echo "Making complete"

$(NAME)	: $(FOBJ) $(OBJS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@

$(OBJ_FILE)%.o: $(SRC_FILE)%.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) -o $@ -c $<

$(FOBJ) :
	@mkdir $@

clean:
	rm -rf $(OBJS)
	rm -rf $(FOBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
