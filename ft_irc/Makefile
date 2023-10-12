INC_DIR		= .

SRCS		=	./main.cpp \
				./IRC/ManageUsers/ManageUsers.cpp \
				./IRC/ExecuteCommands/part.cpp \
				./IRC/ExecuteCommands/pass.cpp \
				./IRC/ExecuteCommands/who.cpp \
				./IRC/ExecuteCommands/time.cpp \
				./IRC/ExecuteCommands/die.cpp \
				./IRC/ExecuteCommands/kick.cpp \
				./IRC/ExecuteCommands/oper.cpp \
				./IRC/ExecuteCommands/kill.cpp \
				./IRC/ExecuteCommands/names.cpp \
				./IRC/ExecuteCommands/topic.cpp \
				./IRC/ExecuteCommands/privmsg.cpp \
				./IRC/ExecuteCommands/user.cpp \
				./IRC/ExecuteCommands/nick.cpp \
				./IRC/ExecuteCommands/list.cpp \
				./IRC/ExecuteCommands/ping.cpp \
				./IRC/ExecuteCommands/away.cpp \
				./IRC/ExecuteCommands/quit.cpp \
				./IRC/ExecuteCommands/notice.cpp \
				./IRC/ExecuteCommands/invite.cpp \
				./IRC/ExecuteCommands/join.cpp \
				./IRC/ExecuteCommands/mode.cpp \
				./IRC/ExecuteCommands/admin.cpp \
				./IRC/ExecuteCommands/motd.cpp \
				./IRC/ExecuteCommands/version.cpp \
				./IRC/ManageChannels/ManageChannels.cpp \
				./IRC/Command/Command.cpp \
				./IRC/Utils.cpp \
				./IRC/ManageCommands/NumericResponses.cpp \
				./IRC/ManageCommands/ManageCommands.cpp \
				./IRC/Channel/Channel.cpp \
				./IRC/User/Bot.cpp \
				./IRC/User/User.cpp \
				./IRC/IRC.cpp \
				./network/Client.cpp \
				./network/Server.cpp

OBJS		= $(SRCS:.cpp=.o)

NAME		= ircserv

CC			= clang++
FLAGS		= -Werror -Wall -Wextra -std=c++98

RM			= rm -rf

.cpp.o:
			$(CC) $(FLAGS) -I$(INC_DIR) -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)
			@ echo "\033[0;32mObject files compiled successfully\033[0m"
			$(CC) $(FLAGS) $(OBJS) -I$(INC_DIR) -o $(NAME)
			@ echo "\033[0;32mExecutable created successfully\033[0m"
			@ echo "\033[0;32mCompilation finished successfully\033[0m"

all:		$(NAME)

clean:
			$(RM) $(OBJS)
			@ echo "Deleted all but executable"

fclean:		clean
			$(RM) $(NAME)
			@ echo "Everything deleted"

re:			fclean all

.PHONY:		all clean fclean re
