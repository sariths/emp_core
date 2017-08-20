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


#include "./stringutils.h"
#include "../../common/utilities/io.h"

#include <sstream>
#include <string>
#include <algorithm> 


bool stringInclude(std::string word, std::string substring) 
{
	return word.find(substring) != -1;
}

bool stringInclude(char * word, std::string substring) 
{
	return std::string(word).find(substring) != -1;
}

bool stringInclude(char * word, char * substring) 
{
	return std::string(word).find(substring) != -1;
}


bool stringIncludeAny(std::string word, char ** substringArray, int nItems) 
{
	for (int i = 0; i < nItems; i++) {
		if (stringInclude(word, substringArray[i])) {
			return true;
		}
	}
	return false;
}


bool stringIncludeAny(char * word, char ** substringArray, int nItems) 
{
	return stringIncludeAny(std::string(word), substringArray, nItems);
}


void utf8toASCII(char * input, size_t inputLength, char * output, size_t * outputLength) 
{
	
	size_t i = 0;
	size_t added = 0;
	while( i < inputLength && input[i] != '\0' ) {
		added++;
		unsigned char c = (unsigned char)input[i];
		if (c < 127) {
			// 1 byte... already on ASCII
			output[i] = input[i];
			i++; 		
		}
		else if( c < 2047){
			// two byte
			output[i] = '?';
			i++; i++;
		}
		else if ( c < 65535) {
			// 3 byte
			output[i] = '?';
			i++; i++;
		}
		else {
			// 4 byte
			output[i] = '?';
			i++; i++;
		}
	}
	output[added] = '\0'; // close the string
	*outputLength = added;
	

}


void fixString(char * s, size_t stringLength) 
{	
	
	for(size_t i=0; i<stringLength; i++)
	{		
		unsigned char c = (unsigned char) s[i];
		
		if (c == '\0') {
			return;
		}
		else if (c == 32 || c == 35 || c == 179) {
			// Space, #, |
			s[i] = '_';
			continue;
		}
		else if (c >= 48 && c <= 58) {
			// digits
			continue;
		}
		else if (c >= 65 && c <= 90) {
			//capital letters
			continue;
		}
		else if (c >= 97 && c <= 122) {
			//small letters
			continue;
		}
		else {
			s[i] = DEFAULT_CHAR;			
		}
	}

}


std::string size_tToString(size_t sz) 
{
	std::stringstream ss;
	ss << sz;
	return ss.str();
}