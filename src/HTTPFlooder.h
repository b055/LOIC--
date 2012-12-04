/*
 * HTTPFlooder.h
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */

#ifndef HTTPFLOODER_H_
#define HTTPFLOODER_H_

namespace loic {


class HTTPFlooder: public loic::Flooder {

private:
	int downloaded;
	int requested;
	int failed;
	std::string subsite;
	int timeout;


public:
	//constructor
	HTTPFlooder(std::string ip, int port, std::string subSite, bool resp, int delay, int timeout):
		ip(ip),port(port),subsite(subSite),resp(resp),delay(delay),timeout(timeout){};

	//destructor
	virtual ~HTTPFlooder();


	inline int Downloaded(){return downloaded;}
	inline int Requested(){return requested;}
	inline int Failed(){return failed;}
	inline std::string Subsite(){return subsite;}
	inline int Timeout(){return timeout;}



	inline void Downloaded(int down){downloaded = down;}
	inline void Requested(int req){requested = req;}
	inline void Failed(int fail){failed = fail;}
	inline void Subsite(std::string site){subsite= site;}
	inline void Timeout(int time){timeout= time;}

	void Start();
	void Stop();


};

} /* namespace loic */
#endif /* HTTPFLOODER_H_ */
