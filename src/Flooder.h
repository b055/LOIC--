/*
 * Flooder.h
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */


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
namespace loic
{
class Flooder {

protected:
	int delay;
	bool flooding;
	std::string ip;
	int port;
	bool resp;
	bool hostname; //boolean that show's whether to use hostname or ip address


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

public :
	Flooder(){}
	virtual ~Flooder(){};
	//methods that must be implemented by children
	virtual void Start();
	virtual void Stop();
	virtual void work();

};

} /* namespace loic */

