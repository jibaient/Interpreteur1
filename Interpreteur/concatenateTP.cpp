#include "concatenateTP.h"
#include "toolpath.h"
#include "Executable.h"

#include <iostream>


toolpath_t concatenate_toolpath(Workplan mainWorkplan)
{
	toolpath_t WorkplanTP;

	for (auto WS : mainWorkplan.its_elements)
	{
		toolpath_t WS_TP = WS->getTP();
		WorkplanTP.insert(std::end(WorkplanTP), std::begin(WS_TP), std::end(WS_TP));
	}

	return WorkplanTP;
}
