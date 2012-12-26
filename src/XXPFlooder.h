/*
 * XXPFlooder.h
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */

#ifndef XXPFLOODER_H_
#define XXPFLOODER_H_

#include "Flooder.cpp"
namespace loic {

class XXPFlooder: public loic::Flooder{

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


	public:
		XXPFlooder(std::string ip, int port, int proto, int delay, bool resp, std::string data){
			this->ip = ip;
			this->port = port;
			this->protocol = proto;
			this->delay = delay;
			this->resp = resp;
			this->data = data;
			BUF_SIZE = 64;
		};
		virtual ~XXPFlooder(){};

		void Start();
		void Stop();
		void work();
};

} /* namespace loic */
#endif /* XXPFLOODER_H_ */
