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


#ifndef RTRACE_OPTIONS_H
#define RTRACE_OPTIONS_H

#include "../../common/options/optionset.h"
//#include "Radiance/src/rt/ray.h"

class RTraceOptions : public OptionSet {
public:
	//! Constructor
	/*!
	Creates an OptionSet and fills the corresponding 
	options and default values

	@author German Molina
	*/
	RTraceOptions();

	//! Expose all the RTRACE options to the global variables
    /*
    @author German Molina
    @todo Expose all options
    
	void exposeOptions();
     */
    
};

extern RTraceOptions rtraceoptions;
#endif
