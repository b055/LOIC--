/*
 * XXPFlooder.h
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */

#ifndef XXPFLOODER_H_
#define XXPFLOODER_H_

#include "Flooder.h"

namespace loic {

class XXPFlooder: public loic::Flooder {
public:
	XXPFlooder(std::string ip, int port, int proto, int delay, bool resp, std::string data):
		ip(ip),port(port),protocol(proto),delay(delay),resp(resp),data(data){};
	virtual ~XXPFlooder();

	void Start();
	void Stop();


private:
	int floodcount;
	int protocol;
	std::string data;

	inline int FloodCount(){return floodcount;}
	inline int Protocol(){return protocol;}
	inline std::string Data(){return data;}

	inline void FloodCount(int count){floodcount = count;}
	inline void Protocol(int prot){protocol = prot;}
	inline void Data(std::string data){this->data = data;}
};

} /* namespace loic */
#endif /* XXPFLOODER_H_ */
