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
* dll_factory.cpp
* 
* Author: Ilker GURCAN
*/

#include "dll_factory.hpp"

OmniOperations* createOmniOperations(float xCenter, 
									 float yCenter, 
									 float radius, 
									 float innerRadius, 
									 float alfa, 
									 float beta,
									 double sigma) {

	OmniOperations* omni = new OmniOperations(xCenter, 
											  yCenter, 
											  radius, 
											  innerRadius, 
											  alfa, 
											  beta, 
											  sigma);
	return omni;
}

void destroyOmniOperations(OmniOperations *omniOps) {

	delete omniOps;
}
