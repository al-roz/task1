#include<WinSock2.h>
#include <thread>

#define GET_MASSEG_ERROR_MSG	"MSG error"
#define SEND_MASSEG_ERROR_MSG	"Unable to send"

enum errors
{
	Corrected = 0,
	fewArguments = 1,
	alphaInNumber = 2,
	bigInt = 3
};


class ClientModel
{
private:

public:
	ClientModel();
	static std::pair<std::string,errors> GetIterations(std::string&);
	~ClientModel();
};



