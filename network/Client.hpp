#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../includes/Headers.hpp"

#define BUFFER_SIZE	512		// standard buffer size

class	Client
{
	
private:
	char		_buffer[BUFFER_SIZE + 1];
	int const	_fd;
	string	_cmdBuilder;

	Client();		// default constructor is not allowed
	Client(int fd);

	// Get the command sent to client's socket and build it until delimiter is reached,
	// then it will be put into cmd (passed into argument). Return true when a command
	// (partial or fully constructed) is received, false when the client disconnects and
	// is no longer able to send command
	bool	receiveCommand(string &cmd);

	// Send the server's response to the client via its socket
	void	sendResponse(string const &resp);

public:
	~Client();

	friend class Server;	// only lets server handle clients
};

#endif
