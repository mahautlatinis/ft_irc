#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../../includes/Headers.hpp"

class	Command
{
	User	*_user;
	string	_type;
	std::vector<string>	_params;

	void	deduceType(string &cmd);
	void	buildParams(string const &strParams);
	
	void	treatTOPIC(string const &rawCmd);
	void	treatMODE();
	void	treatPRIVMSG_NOTICE();

public:
	static void	InitCommandList();
	Command(User *user, string const &cmd);
	virtual ~Command();

	bool	IsValid() const;
	bool	IsExecutable() const;

	void	Print() const;	// for debugging

	friend class IRC;
};

#endif
