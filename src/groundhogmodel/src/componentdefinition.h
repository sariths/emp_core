/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

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

#include "./componentdefinition.h"
#include "./componentinstance.h"
#include "./face.h"

#include <vector>

class ComponentInstance;

//! Groups of Face (or other geometry) meant to be placed several times in a GroudhogModel

/*!
They are referenced within other ComponentInstance or Layers by mean of ComponentInstance
*/
class ComponentDefinition {

private:
	std::string name; //!< The name of the ComponentDefinition
	std::vector <Face *> faces; //!< All the Face within the ComponentDefinition
	//Workplanes // **
	//Sensors // **
	//std::vector <Material *> materials;	
	std::vector <ComponentInstance *> instances; //!< All the ComponentInstance within the ComponentDefinition
	//WindowGroups
	//Observers // **	

public:
	//! Creates a ComponentDefinition object
	/*!

	Assigns the name. All other things are empty

	@author German Molina
	@param[in] componentName The name of the ComponentDefinition
	*/
	ComponentDefinition(std::string * componentName);

	//! Destroys a ComponentDefinition object
	/*!
	@author German Molina
	*/
	~ComponentDefinition();

	//! Add a Face to the ComponentDefinition
	/*!
	@author German Molina
	@param[in] face The Face to add
	*/
	void addFace(Face * face);

	//! Counts the number of Face in the ComponentDefinition
	/*!
	@author German Molina
	@return The number of faces in the ComponentDefinition
	*/
	size_t getNumFaces();

	//! Retrieves the name of the ComponentDefinition
	/*!
	@author German Molina
	@return The name of the ComponentDefinition
	*/
	std::string getName();
	
	//! Retrieves a Face reference of the ComponentDefinition
	/*!
	@author German Molina
	@param[in] i The index of the Face to retrieve
	@return The reference to the Face
	*/
	Face * getFaceRef(size_t i);

	//! Retrieves a ComponentInstance reference of the ComponentDefinition
	/*!
	@author German Molina
	@param[in] i The index of the ComponentInstance to retrieve
	@return The reference to the ComponentInstance
	*/
	ComponentInstance * getComponentInstanceRef(size_t i);

	//! Retrieves the reference to the Face vector
	/*!
	@author German Molina
	@return The reference to the Face vector
	*/
	std::vector <Face * > * getFacesRef();

	//! Retrieves the reference to the ComponentInstance vector
	/*!
	@author German Molina
	@return The reference to the ComponentInstance vector
	*/
	std::vector <ComponentInstance * > * getComponentInstancesRef();
};