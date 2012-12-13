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

int main() {
	loic::HTTPFlooder * flood = new loic::HTTPFlooder("127.0.0.1",2345,"gibberish",false,0,2);
	flood->Start();

	boost::this_thread::sleep(boost::posix_time::seconds(60));

	flood->Stop();
	return 0;
}
