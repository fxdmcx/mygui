/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _5bbd7ceb_411b_45fa_8c83_dbd118de6b83_
#define _5bbd7ceb_411b_45fa_8c83_dbd118de6b83_

#include <MyGUI.h>

namespace tools
{

	class StateController
	{
	public:
		StateController() { }
		virtual ~StateController() { }

		virtual void initState() { }
		virtual void cleanupState() { }

		virtual void pauseState() { }
		virtual void resumeState() { }
	};

}

#endif
