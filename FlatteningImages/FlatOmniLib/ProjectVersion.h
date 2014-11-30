/*
* Copyright 2010-2014 METU, Middle East Technical University, Informatics Institute
*
* This file is part of "Unwrapping OmniDirectional Images" Library.
*
* "Unwrapping OmniDirectional Images" is free software: 
* you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* "Unwrapping OmniDirectional Images" is distributed in the hope 
* that it will be useful, but WITHOUT ANY WARRANTY; without 
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
* PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License.
* If not, see <http://www.gnu.org/licenses/>. 
* Please contact Alptekin TEMIZEL for more info about 
* licensing atemizel@metu.edu.tr.
*
*/

/*
* ProjectVersion.h
* 
* Author: Ilker GURCAN
*/

#ifndef FLAT_OMNI_IMPORTS
	#define FLAT_OMNI_API __declspec(dllexport) 
#else
	#define FLAT_OMNI_API __declspec(dllimport) 
#endif

#ifndef _PROJECT_VERSION_H_
#define _PROJECT_VERSION_H_

//This Class Stands For Recording The Version Of The Project...
class FLAT_OMNI_API ProjectVersion { 
	public : 
		static char * getProjectVersion();
	private : 
		static char * version;
};

#endif
