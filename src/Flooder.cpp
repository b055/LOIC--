#ifndef FLOODER_H_
#define FLOODER_H_

#include <string>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <stdexcept>
#include <iostream>
#include <sstream>
//for sockets
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <errno.h>
#include <time.h>
#include <fcntl.h>
namespace loic
{
class Flooder {

protected:
	int delay;
	bool flooding;
	std::string ip;
	int port;
	bool resp;
	int BUF_SIZE;
	unsigned int numThreads;
	bool reader;
	bool debug;

public :
	//Accessors
	inline int Delay(){ return delay;}
	inline bool Resp(){return resp;}
	inline std::string IP(){return ip;}
	inline int Port(){return port;}
	inline bool isFlooding(){return flooding;}


	//mutators
	inline void Delay(int del){delay = del;}
	inline void isFlooding(bool isflood){flooding = isflood;}
	inline void IP(std::string ip){this->ip = ip;}
	inline void Port(int port){this->port = port;}
	inline void Resp(bool resp){this->resp = resp;}



	virtual ~Flooder(){};
	//methods that must be implemented by children
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void work() = 0;

};

} /* namespace loic */

#endif
