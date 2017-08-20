
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

#include "../../groundhogmodel/groundhogmodel.h"
#include "../../common/geometry/polygon.h"
#include "../../common/geometry/loop.h"

#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/attribute_dictionary.h>

#include <vector>



#define SKP_GROUNDHOG_DICTIONARY "Groundhog"
#define SKP_LABEL "Label"
#define SKP_WINDOW "window"
#define SKP_WORKPLANE "workplane"
#define SKP_ILLUM "illum"
#define SKP_NAME "Name"
#define SKP_VALUE "Value"

#define TO_M(x) x*0.0254
#define TO_M2(x) x*0.00064516
#define TO_DEGREE(x) x*180.0/3.141592654 


//! Object that reads a a SketchUp model and fills a GroundhogModel
/*!
This object has a short life, and it is only meant to parse a SKP file
into a GroundhogModel.

The main function used by the Glare object is the parseSKPModel function.
*/

class SKPReader {
private:
	SUModelRef suModel; //!< The SketchUp model to be read.
	SUStringRef groundhogDictionaryName; //!< The Groundhog Dictionary name (in SketchUp format)

public:

	//! Creates a SKPReader object.
	/*!
	It assigns a SU_INVALID value to the suModel member, creates the SUStringRef
	object with the correct name to find the information and initializes the SketchUp API

	@author German Molina
	*/
	SKPReader();

	//! Destroys a SKPReader object
	/*!
	Destroys the suModel as well as the groundhogDictionaryName.
	It also terminates the SketchUp API

	@author German Molina
	*/
	~SKPReader();

	//! Check if a returned SUResult has a certain error or not.
	/*!
	This function intends to help verify that the SketchUp API is trated apropriately.

	@author German Molina
	@param[in] res The SUResult to check
	@param[in] functionName The name of the function
	@param[in] location The 'place' where this is called
	*/
	bool checkSUResult(SUResult res, std::string functionName, std::string location);

	//! Reads the SKP model and fills a GroundhogModel
	/*!
	@author German Molina
	@param[in] inputFile The name of the SKP file to read
	@param[out] model The GroundhogModel to be filled
	@param verbose Should we inform progress?

	@return success
	*/
	bool parseSKPModel(std::string inputFile, GroundhogModel * model, bool verbose);

	//! Retrieves a string from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[in] key The key of the object to read
	@param[out] value The string where the value will be placed
	@return success
	*/
	bool getStringFromShadowInfo(SUShadowInfoRef shadowInfo, char * key, char * value);

	//! Retrieves a double from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[in] key The key of the object to read
	@param[out] value The double where the value will be placed
	@return success
	*/
	bool getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo, char * key, double * value);

	//! Retrieves a time (epoch) from a SUShadowInfoRef object.
	/*!
	@author German Molina
	@param[in] shadowInfo The SUShadowInfoRef object to be read
	@param[out] value The object where the value will be placed
	@return success
	*/
	bool getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value);

	//! Loads North Correction and Location of the model
	/*!
	Loads the North Correction, Longitude, Latitude, Time Zone,
	City Name, Country Name and Current Date

	@author German Molina
	@param[out] model The GroundhogModel to fill with the info
	@param verbose Should we inform progress?
	@return success
	*/
	bool loadModelInfo(GroundhogModel * model, bool verbose);

	//! Transforms a SUCameraRef into a View
	/*!
	Assigns the given name to the View, and retrieves the FOV, View Point,
	View Direction, etc. from the SUCameraRef

	@author German Molina
	@param[in] viewName The name of the view to create
	@param[in] suCamera The SUCameraRef object to parse
	@param[out] view The View object to fill
	@return success
	*/
	bool SUCameraToView(std::string viewName, SUCameraRef suCamera, View * view);

	//! Transforms a SUSceneRef object into a View object
	/*!
	Similar to SKPReader::SUCameraToView(), but the name is obtained
	directly from the SUSceneRef object name

	@author German Molina
	@param[in] suView The SUSceneRef object to be read
	@param[out] view The View object to be filled
	@return success
	*/
	bool SUViewToView(SUSceneRef suView, View * view);

	//! Loads all the SUSceneRef objects into the GroundhogModel object
	/*!
	Loads all the SUSceneRef in the suModel, including the 'current one'
	(i.e. that view that was not saved, but that is the last used in the model)

	Every SUSceneRef will lead to a View with the corresponding name, and the
	'Current View' will be stored as 'view'

	@author German Molina
	@param[out] model The GroundhogModel object to fill
	@param verbose Should we inform progress?
	@return success
	*/
	bool loadViews(GroundhogModel * model, bool verbose);

	//! Loads the information in the SULayerRef objects into the model
	/*!
	Reads the layers, and loads them into the GroundhogModel. All the
	resulting Layer will be empty after this is done. After this function,
	the SKPReader::loadLayersContent() has to be called.

	@author German Molina
	@param[out] model The GroundhogModel to fill
	@param verbose Inform progress?
	@return success
	@note After this function, the SKPReader::loadLayersContent() has to be called.
	*/
	bool loadLayers(GroundhogModel * model, bool verbose);

	//! Retrieves a SUComponentDefinitionRef name
	/*!
	@author German Molina
	@param[in] definition The SUComponentDefinitionRef object to retrieve the name from
	@param[out] name The object to fill with the name
	@return success
	*/
	bool getSUComponentDefinitionName(SUComponentDefinitionRef definition, std::string * name);

	//! Creates a ComponentInstance from a SUComponentInstanceRef and adds it to the destination vector
	/*!
	Creates a ComponentInstance from a SUComponentInstanceRef and adds it to the destination vector.
	The destination is a vector within a ComponentDefinition or a Layer; and the GroundhogModel provided
	should contain the ComponentDefinition that contains the parent (checked by name).

	@author German Molina
	@param[out] dest The ComponentInstance vector to which the instance will be added
	@param[in] suComponentInstance The SUComponentInstanceRef object to add
	@param[in] model The GroundhogModel where the ComponentDefinition of the instance is located
	@return success
	*/
	bool addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance, GroundhogModel * model);

	//! Creates a Face from a SUFaceRef and adds it to the destination vector
	/*!
	Creates a Face from a SUFaceRef and adds it to the destination vector.
	The destination is a vector within a ComponentDefinition or a Layer.

	@author German Molina
	@param[out] dest The Face vector to which the instance will be added
	@param[in] face The SUFaceRef object to add
	@return success
	*/
	bool addFaceToVector(std::vector <Face * > * dest, SUFaceRef face);

	//! Retrieves all the SUFaceRef objects in entities and loads them into a Face vector
	/*!
	@author German Molina
	@param[out] dest The Face vector where all the SUFaceRef in entities will be bulked
	@param[in] entities the SUEntitiesRef object containing the SUFacesRef
	@return success
	*/
	bool bulkFacesIntoVector(std::vector <Face * > * dest, SUEntitiesRef  entities);

	//! Transforms a SUComponentDefinitionRef into a ComponentDefinition and adds it to a GroundhogModel
	/*!
	@author German Molina
	@param[in] definition The SUComponentDefinitionRef to transform and import to model
	@param[out] model The GroundhogModel where the ComponentDefinition will be added
	@return success
	*/
	bool loadComponentDefinition(SUComponentDefinitionRef definition, GroundhogModel * model);

	//! Loads all the SUComponentDefinitionRef in the suModel into a GroundhogModel
	/*!
	@author German Molina
	@param[in] model The GroundhogModel to be filled
	@param verbose Inform progress?
	@return success
	*/
	bool loadComponentDefinitions(GroundhogModel * model, bool verbose);

	//! Loads the contents of the SULayerRef in the suModel
	/*!
	Starts by importing all the SUFaceRef objects (as Face) and continues
	by loading all the SUComponentInstanceRef (as ComponentInstance)

	@author German Molina
	@param[in] model The GroundhogModel to be filled
	@param verbose Inform progress?
	@return success
	*/
	bool loadLayersContent(GroundhogModel * model, bool verbose);

	//! Transforms a SUFaceRef into a Polygon3D
	/*!
	Creates a polygon with the same vertex (Point3D) and Loop as the
	SUFaceRef object, except that it cleans them (see Loop::clean()). 
	It also fills the area of the face.

	The SketchUp API is in inches, so everything is transformed into
	meters and squared meters.

	@author German Molina
	@param[in] face The SUFaceRef to transform
	@param[out] polygon The Polygon3D object to fill
	@return success
	*/
	bool SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon);

	//! Transforms an SULoopRef into a Loop
	/*!
	The SketchUp API is in inches, so everything is transformed into meters.

	@author German Molina
	@param[in] suLoop The SULoopRef to transform
	@param[out] loop The Loop object to fill
	@return success
	*/
	bool SULoopToLoop(SULoopRef suLoop, Loop * loop);

	//! Retrieves the name of the SUFaceRef object
	/*!
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUFaceName(SUFaceRef face, std::string * name);

	//! Retrieves the Layer Name from a SUFaceRef object
	/*!
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the Layer Name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUFaceLayerName(SUFaceRef face, std::string * name);

	//! Retrieves the name of the SUDrawingElementRef object
	/*!
	@author German Molina
	@param[in] element The SUDrawingElementRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name);

	//! Retrieves the name of the SUEntityRef object
	/*!
	@author German Molina
	@param[in] entity The SUEntityRef to retrieve the name from
	@param[out] name The name to be filled
	@return success
	*/
	bool getSUEntityName(SUEntityRef entity, std::string * name);

	//! Retrieves all the SUComponentInstanceRef objects in entities and loads them into a ComponentInstance vector
	/*!
	Retrieves all the SUComponentInstanceRef objects in entities and loads them into a
	ComponentInstance vector. The GroundhogModel inputed should contain the required
	ComponentDefinitions (which are checked by name).

	@author German Molina
	@param[out] dest The ComponentInstance vector where all the SUComponentInstanceRef in entities will be bulked
	@param[in] entities The SUEntitiesRef object containing the SUComponentInstanceRef
	@param[in] model The GroundhogModel where the required ComponentDefinitions are located
	@return success
	*/
	bool bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * dest, SUEntitiesRef  entities, GroundhogModel * model);

	//! Fills the location (rotation, translation and scale) of a ComponentInstance based on a suInstance
	/*!
	@author German Molina
	@param[out] instance The ComponentInstance to fill with location
	@param[in] suInstance The SUComponentInstanceRef object from which the location is obtained
	@return success
	*/
	bool fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance);

	//! Retrieves the label of a SUFaceRef
	/*!
	Will return TRUE if the face does have a label; false if not.
	
	@author German Molina
	@param[in] face The SUFaceRef to retrieve the label from
	@param[out] name The object to put the name into
	@return false if the face does not have a label, true otherwise
	*/
	bool getSUFaceLabel(SUFaceRef face, std::string * name);

	//! Retrieves the label of a SUEntityRef
	/*!
	Will return TRUE if the entity does have a label; false if not.

	@author German Molina
	@param[in] entity The SUEntityRef to retrieve the label from
	@param[out] name The object to put the name into
	@return false if the entity does not have a label, true otherwise
	@todo Change all labels to numbers.
	*/
	bool getSUEntityLabel(SUEntityRef entity, std::string * name);

	//! Adds a workplane face to a model.
	/*!
	Gets the name of the face and adds it to the model. The model
	automatically creates a workplane if it does not already exist.

	@author German Molina
	@param[in] face The face to treat as a workplane	
	@param[out] model The model to add the workplane to
	@return success
	*/
	bool addWorkplaneToModel(SUFaceRef face, GroundhogModel * model);

};