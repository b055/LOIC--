#include "HTTPFlooder.h"
#include "XXPFlooder.h"
#include <boost/program_options.hpp>

using namespace loic;
namespace po = boost::program_options;
int main(int argc,char *argv [])
{
	po::options_description desc("Allowed options");
	int port;
	std::string resp;
	int delay;
	int timeout;
	std::string subsite;
	int nthreads;
	bool rp;
	std::string url;
	std::string read;
	bool reader;
	std::string debugger;
	bool debug;
	try
	{
		desc.add_options()
			("help,h","produce help message")
			("target,T",po::value<std::string>(), "set target url")
			("nthreads,n",po::value<int>(&nthreads),"set the number of threads")
			("port,p",po::value<int>(&port)->default_value(80),"set the port number. Defaults to 80")
			("response,R",po::value<std::string>(&resp)->default_value("true"),"block and wait for server response. Defaults to true")
			("delay,d", po::value<int>(&delay)->default_value(0),"set the delay between requests(milliseconds). Defaults to 0")
			("timeout,t",po::value<int>(&timeout)->default_value(3),"sets the time out period(seconds). Defaults to 3 seconds")
			("subsite,s",po::value<std::string>(&subsite)->default_value("/"),"sets the location on the target to fetch. Defaults to the index, '/'")
			("read",po::value<std::string>(&read)->default_value("false"),"sets whether to read the first 64 characters of the incoming message or not. Defaults to false")
			("debug,d",po::value<std::string>(&debugger)->default_value("false"),"sets into debugging mode. Defaults to false")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc,argv,desc),vm);
		po::notify(vm);

		if(vm.count("help"))
		{
			std::cout<<desc<<std::endl;
			return 1;
		}
	
		if(argc<3)
		{
			std::cout<<"Too few arguments"<<std::endl;
			std::cout<<"Try 'loic++ --help' for help\n\n";
			return 1;
		}


		if(vm.count("nthreads"))
		{
			nthreads  = vm["nthreads"].as<int>();
		}
		else
		{
			std::cout<<"Required argument 'nthreads'"<<std::endl<<std::endl;
			return 1;
		}
		
		if(vm.count("response"))
		{
			if(vm["response"].as<std::string>().compare("true") ==0 || vm["response"].as<std::string>().compare("T") ==0
					|| vm["response"].as<std::string>().compare("t") ==0 )
			{
				rp = true;
			}
			else if(vm["response"].as<std::string>().compare("false")  ==0 || vm["response"].as<std::string>().compare("F") ==0
					|| vm["response"].as<std::string>().compare("f") ==0)
			{
				rp = false;
			}
			else
			{
				std::cout<<"Do not understand the argument "<<vm["response"].as<std::string>()<<std::endl<<std::endl;
				return 0;
			}
		}

		if(vm.count("read"))
		{
			if(vm["read"].as<std::string>().compare("true") ==0 || vm["read"].as<std::string>().compare("T") ==0
					|| vm["read"].as<std::string>().compare("t") ==0 )
			{
				reader = true;
			}
			else if(vm["read"].as<std::string>().compare("false")  ==0 || vm["read"].as<std::string>().compare("F") ==0
					|| vm["read"].as<std::string>().compare("f") ==0)
			{
				reader = false;
			}
			else
			{
				std::cout<<"Do not understand the argument "<<vm["read"].as<std::string>()<<std::endl<<std::endl;
				return 0;
			}
		}

		if(vm.count("debug"))
		{
			if(vm["debug"].as<std::string>().compare("true") ==0 || vm["debug"].as<std::string>().compare("T") ==0
					|| vm["debug"].as<std::string>().compare("t") ==0 )
			{
				debug = true;
			}
			else if(vm["debug"].as<std::string>().compare("false")  ==0 || vm["debug"].as<std::string>().compare("F") ==0
					|| vm["debug"].as<std::string>().compare("f") ==0)
			{
				debug = false;
			}
			else
			{
				std::cout<<"Do not understand the argument "<<vm["debug"].as<std::string>()<<std::endl<<std::endl;
				return 0;
			}
		}

		if(vm.count("target"))
		{
			url = vm["target"].as<std::string>();
		}
		else
		{
			std::cout<<"Required argument '-target' missing\n\n";
			std::cout<<"Try 'loic++ -h' for help\n\n";
			return 0;
		}
	}
	catch(std::exception& exe)
	{
		std::cout<<exe.what()<<std::endl<<std::endl;
		return 0;
	}
	
	loic::HTTPFlooder * flood = new loic::HTTPFlooder(url,port,subsite,rp,delay,timeout,nthreads,reader,debug);
	flood->Start();

	while(flood->isFlooding())
		boost::this_thread::sleep(boost::posix_time::minutes(5));

	flood->Stop();
	return 0;
}
