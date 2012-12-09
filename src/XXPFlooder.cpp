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


			while(this->flooding)
			{
				if(this->protocol == 1)
				{

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

					try
					{
						while(this->flooding)
						{
							floodcount++;

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
