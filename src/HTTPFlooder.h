#ifndef HTTPFLOODER_H_
#define HTTPFLOODER_H_


#include "Flooder.cpp"
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
namespace loic {

class HTTPFlooder: public loic::Flooder {
private:
		enum ReqState { readyState,connectingState,requestingState,downloadingState,completedState,failedState};
		int downloaded;
		int requested;
		int failed;
		std::string subsite;
		int timeout;
		HTTPFlooder::ReqState state;
		time_t lastAction;
		int SSL_PORT;
		typedef boost::shared_mutex Lock;
		typedef boost::unique_lock< boost::shared_mutex > WriteLock;
		typedef boost::shared_lock< boost::shared_mutex >  ReadLock;
		Lock locker;
		void checkTimeOut();
public:
		HTTPFlooder(std::string ip, int port, std::string subSite, bool resp, int delay, int timeout,int threads,bool reader,bool debug):SSL_PORT(443)
		{
			this->ip = ip;
			this->port = port;
			this->subsite = subSite;
			this->resp = resp;
			this->delay = delay;
			this->timeout = timeout;
			this->numThreads = threads;
			BUF_SIZE = 64;
			this->requested = 0;
			this->downloaded = 0;
			this->failed = 0;
			this->debug = debug;
			this->reader = reader;
			if(port == SSL_PORT )
			{
				SSL_load_error_strings();
				SSL_library_init();
			}

		};
				//destructor
		virtual ~HTTPFlooder(){};

		inline int Downloaded(){return downloaded;}
		inline int Requested(){return requested;}
		inline int Failed(){return failed;}
		inline std::string Subsite(){return subsite;}
		inline int Timeout(){return timeout;}
		inline HTTPFlooder::ReqState State(){return state;}


		inline void State(HTTPFlooder::ReqState newState){state = newState;}
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
