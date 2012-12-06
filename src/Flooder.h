/*
 * Flooder.h
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */


#include <string>
#include <thread>
#include <exception>
#include <chrono>
namespace loic
{
class Flooder {

protected:
	int delay;
	bool flooding;
	std::string ip;
	int port;
	bool resp;
	std::thread thread;
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
	virtual ~Flooder();
	//methods that must be implemented by children
	virtual void Start();
	virtual void Stop();
	virtual void work();

};

} /* namespace loic */

