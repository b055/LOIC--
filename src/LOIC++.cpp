//============================================================================
// Name        : LOIC++.cpp
// Author      : Ivan Nelson Togboa Cadri
// Version     :
// Copyright   : 
// Description : Linux c++ version oif LOIC
//============================================================================

#include "HTTPFlooder.h"

using namespace loic;

int main() {
	HTTPFlooder * flood = new loic::HTTPFlooder("127.0.0.1",8080,"gibberish",false,0,2);
	flood->Start();
	return 0;
}
