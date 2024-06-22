#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	PannelOrganizer::~PannelOrganizer()
	{
		for (Pannel* pannel : m_Pannels)
		{
			delete pannel;
		}
	}
}