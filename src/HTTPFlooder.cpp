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
			this->state = readyState;


			this->state = connectingState;

			/* socket work */


			this->state = downloadingState;
			requested++;

			if(resp)
			{

			}

			this->state = completedState;

			downloaded++;
			if(delay>0)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
			}
		}
		catch(std::exception & exe)
		{

		}
		flooding = false;
	}

} /* namespace loic */
