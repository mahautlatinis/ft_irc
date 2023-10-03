INC_DIR		= .

SRCS		=	./main.cpp \
				./IRC/ManageUsers/ManUser.cpp \
				./IRC/ExecCmd/PART.cpp \
				./IRC/ExecCmd/PASS.cpp \
				./IRC/ExecCmd/WHO.cpp \
				./IRC/ExecCmd/TIME.cpp \
				./IRC/ExecCmd/DIE.cpp \
				./IRC/ExecCmd/KICK.cpp \
				./IRC/ExecCmd/OPER.cpp \
				./IRC/ExecCmd/KILL.cpp \
				./IRC/ExecCmd/NAMES.cpp \
				./IRC/ExecCmd/TOPIC.cpp \
				./IRC/ExecCmd/PRIVMSG.cpp \
				./IRC/ExecCmd/USER.cpp \
				./IRC/ExecCmd/NICK.cpp \
				./IRC/ExecCmd/LIST.cpp \
				./IRC/ExecCmd/PING.cpp \
				./IRC/ExecCmd/AWAY.cpp \
				./IRC/ExecCmd/QUIT.cpp \
				./IRC/ExecCmd/NOTICE.cpp \
				./IRC/ExecCmd/INVITE.cpp \
				./IRC/ExecCmd/JOIN.cpp \
				./IRC/ExecCmd/MODE.cpp \
				./IRC/ExecCmd/ADMIN.cpp \
				./IRC/ExecCmd/MOTD.cpp \
				./IRC/ExecCmd/VERSION.cpp \
				./IRC/ManageChannels/ManChannel.cpp \
				./IRC/Command/Command.cpp \
				./IRC/Utils.cpp \
				./IRC/ManageCommands/NumericResponses.cpp \
				./IRC/ManageCommands/ManCmd.cpp \
				./IRC/Channel/Channel.cpp \
				./IRC/User/Bot.cpp \
				./IRC/User/User.cpp \
				./IRC/IRC.cpp \
				./network/Client.cpp \
				./network/Server.cpp

OBJS		= $(SRCS:.cpp=.o)

NAME		= ircserv

CC			= clang++
FLAGS		= -std=c++98 -Werror -Wall -Wextra

RM			= rm -rf

.cpp.o:
			@$(CC) $(FLAGS) -I$(INC_DIR) -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)
			@echo "\033[0;32mObject files compiled successfully\033[0m"
			@$(CC) $(FLAGS) $(OBJS) -I$(INC_DIR) -o $(NAME)
			@echo "\033[0;32mExecutable created successfully\033[0m"
			@echo "\033[0;32mCompilation finished successfully\033[0m"

all:		$(NAME)

clean:
			@$(RM) $(OBJS)
			@echo "Deleted all but executable"

fclean:		clean
			@$(RM) $(NAME)
			@echo "Everything deleted"

re:			fclean all

.PHONY:		all clean fclean re
