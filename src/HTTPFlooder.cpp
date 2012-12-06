/*
 * HTTPFlooder.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: ivan
 */

#include "HTTPFlooder.h"

namespace loic {



	void HTTPFlooder::Start()
	{

	}


	void HTTPFlooder::Stop()
	{


	}

	void HTTPFlooder::work()
	{
		try
		{
			this->state = ReqState::readyState;


			this->state = ReqState::connectingState;

			/* socket work */


			this->state = ReqState::downloadingState;
			requested++;

			if(resp)
			{

			}

			this->state = ReqState::completedState;

			downloaded++;
			if(delay>0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			}
		}
		catch(std::exception & exe)
		{

		}
		flooding = false;
	}

} /* namespace loic */
