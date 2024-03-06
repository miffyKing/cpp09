NAME = btc
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm
RMFLAGS = -f

SRCS = main.cpp BitcoinExchange.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
			$(CXX) $(CXXFLAGS) $^ -o $@

clean:
			$(RM) $(RMFLAGS) $(OBJS)

fclean:
			make clean
			$(RM) $(RMFLAGS) $(NAME)

re:
			make fclean
			make all

.PHONY: all clean fclean re