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


#ifndef OPTIONSET_H
#define OPTIONSET_H




#include "../utilities/io.h"

#include <vector>
#include <string>

#include "json/json.hpp"

using json = nlohmann::json;

//! A base class for sets of Options

/*!
This class allows creating derived classes that represent a set of predefined
options and their default values.

The actual options are contained within a JSON object called data

*/

class OptionSet {
private:	
    json data = json({});

public:

	//! Registers a new option in the OptionSet
	/*!
	After registering, this option may be searched easily. The idea
	is that this method is called by constructors of derived classes.

	@author German Molina
	@param opt The name of the option
	@param defaultValue The default value for such option
	*/
	template<typename T>
    void addOption(std::string opt, T defaultValue)
    {
      data[opt] = defaultValue;
    }

    //! Returns the number of options
    /*!
    @author German Molina
    @return the number of options
    */
    size_t size() const;

    //! Checks wether an option exists in an OptionSet
    /*!
    @author German Molina
    @param[in] opt The option name
    @return true or false
    */
    bool hasOption(std::string opt) const;

	//! Sets an option value
	/*!
	Modifies the value of an option. Returns True if the 
	option was found and False if not.

	@author German Molina
	@param opt The name of the option
	@param value The new value for the option
	*/
    template<typename T>
    bool setOption(std::string opt, T value)
    {
      if (!hasOption(opt)) {
          std::string er = "OptionSet has no " + opt + " option... impossible to set such value";
          FATAL(errorMessage,&er[0]);
          return false;
      }
      

      // set the new data, with the type of the original
      data[opt] = value;
      return true;
    }

   
	//! Retrieves an option from the OptionSet
    /*!
    @author German Molina
    @param[in] opt The option to retrieve
    */
    template<typename T>
    T getOption(std::string opt) const
    {
      return data.at(opt).get<T>();
    }

    //! Retrieves an option from the OptionSet
    /*!
    @author German Molina
    @param[in] opt The option to retrieve
    */
    template<typename T>
    T getOption(size_t i) const
    {            
      return data.at(i).get<T>();
    }

	//! Prints the options
	/*!
    Prints a set of options in a file. If NULL is passed as a name, 
    they will be printed to the STDOUT

	@author German Molina
	@param file The file name to print the data to 
	*/
	void print(char * file);

    //! Checks if all the options in two OptionSet objects are the same
    /*
    @author German Molina
    @param other A pointer to another OptionSet object
    @return is equal?
    */
    bool isEqual(OptionSet * other);

  
    //! Gets the inline version of the options (i.e. for Radiance commands)
    /*!
    @author German Molina
    */
    std::string getInlineVersion();
    
    //! Retrieves the option name by index
    /*!
     @author German Molina
     @return the name of the option
     @param i [size_t] the index of the option
     */
    std::string getOptionName(size_t i);
    
    json::iterator begin();
    
    json::iterator end();

};

#endif
