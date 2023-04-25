NAME = webServ

CC = c++

CPPFLAGS = -Wall -Werror -Wextra -std=c++98

HEADERS = Headers.hpp  webServer.hpp

SRC =  webServer.cpp Server.cpp RequestClass.cpp parserObjectU.cpp main2.cpp

OBJS = $(SRC:%.cpp=%.o)

all:	$(NAME) $(HEADERS)
	@echo "Meking complete"

$(NAME)	: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) -c $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
