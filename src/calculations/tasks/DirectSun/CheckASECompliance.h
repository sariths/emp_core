/*****************************************************************************
 Emp
 
 Copyright (C) 2018  German Molina (germolinal@gmail.com)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 *****************************************************************************/



#pragma once


#include "./CalculateDirectSolarIlluminance.h"
#include "../../../taskmanager/cbdm_task.h"


class CheckASECompliance : public CBDMTask {
    
public:
   
    CheckASECompliance(std::string name, EmpModel * theModel, RTraceOptions * theOptions, Workplane * wp, int theMf, double theMinLux, float theEarly, float theLate, int minMonth, int maxMonth, float theMinTime);
    
    
    CheckASECompliance(std::string name, EmpModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, int theMf,double theMinLux, float theEarly, float theLate, int minMonth, int maxMonth, float theMinTime);
    
    
    GET_DEP_RESULTS(CalculateDirectSolarIlluminance);    
   
    
};

extern CheckASECompliance checkASE;




