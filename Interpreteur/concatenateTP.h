#pragma once
#include <string>
#include <vector>
#include "Executable.h"
#include "toolpath.h"

toolpath_t concatenate_toolpath(Workplan mainWorkplan); // Concatenation des toolpaths de chacun des workinsteps dans un seul pacrous d'outils pour tout le main workplan, donc de tout le projet