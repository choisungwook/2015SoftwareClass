#include "utility.h"
#include "carThread.h"
#include "enum.h"

void destorythread()
{
	if (t)
	{
		
		destorycarThread();
		t->detach();
		if (t->joinable())
			t->join();
		//delete t;
		
	}
}


