#ifndef ACCDEC_CONTROLLER_HPP
#define ACCDEC_CONTROLLER_HPP

#include "toolpath.h"

void determineVelocityProfile(Toolpath_section* section,float feedrate);
void lineNormalBlock(Toolpath_section* section, float feedrate);
void lineSmallBlock(Toolpath_section* section);
void circleNormalBlock(Toolpath_section* section, float feedrate);
void circleSmallBlock();
bool isNormalBlock(Toolpath_section* section,float feedrate);


#endif //ACCDEC_CONTROLLER_HPP