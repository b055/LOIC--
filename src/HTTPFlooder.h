/*
 * HTTPFlooder.h
 *
 *  Created on: Dec 7, 2012
 *      Author: ivan
 */

#ifndef HTTPFLOODER_H_
#define HTTPFLOODER_H_

#include "Flooder.h"

namespace loic {

class HTTPFlooder: public loic::Flooder {
private:
		enum ReqState { readyState,connectingState,requestingState,downloadingState,completedState,failedState};
		int downloaded;
		int requested;
		int failed;
		std::string subsite;
		int timeout;
		ReqState state;

public:
		HTTPFlooder(std::string ip, int port, std::string subSite, bool resp, int delay, int timeout)
		{
			this->ip = ip;
			this->port = port;
			this->subsite = subSite;
			this->resp = resp;
			this->delay = delay;
			this->timeout = timeout;
		};

				//destructor
		virtual ~HTTPFlooder();

		inline int Downloaded(){return downloaded;}
		inline int Requested(){return requested;}
		inline int Failed(){return failed;}
		inline std::string Subsite(){return subsite;}
		inline int Timeout(){return timeout;}
		inline ReqState State(){return state;}


		inline void State(ReqState newState){state = newState;}
		inline void Downloaded(int down){downloaded = down;}
		inline void Requested(int req){requested = req;}
		inline void Failed(int fail){failed = fail;}
		inline void Subsite(std::string site){subsite= site;}
		inline void Timeout(int time){timeout= time;}

		void Start();
		void Stop();
		void work();
};

} /* namespace loic */
#endif /* HTTPFLOODER_H_ */
