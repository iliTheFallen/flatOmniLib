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
* Main.cpp
* 
* Author: Ilker GURCAN
*/

#define FLAT_OMNI_IMPORTS

#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "dll_factory.hpp"

int main(int argc, char* argv []) { 

	cv::Mat image;
	OmniOperations *omni;

	if(argc > 1) { 
		image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
		if(!image.data) {
			std::cout<<"Could not Open or Find the Image File"<<std::endl;
			return -1;
		}//End of Inner-if-Block...
		//Print Image Type...
		std::cout<<"Image Depth : "<<image.depth()
			     <<" / Image CN : "<<image.channels()
				 <<" / Stride : "<<image.step[0]<<std::endl;
		//Quadrupled------->5184.0f, 3456.0f, 3456.0f, 628.0f, 1.0f, 1.0f
		//Original------>2592.0f, 1728.0f, 1728.0f, 314.0f, 1.0f, 0.5f
		//Quartered----->1296.0f, 864.0f, 864.0f, 157.0f, 1.0f, 0.5f
		//OneOver16----->648.0f, 432.0f, 432.0f, 78.5f, 1.0f, 0.5f
		omni = createOmniOperations(2592.0f, 
								    1728.0f, 
								    1728.0f, 
								    314.0f, 
								    1.0f, 
								    1.0f);
		omni->unwrap(image, std::string("tryMeBoy.jpeg"), true);
		//Deallocate Resources...
		destroyOmniOperations(omni);
	} else {
		std::cout<<"!!Please Enter the File Name for OmniDirectional Image!!"<<std::endl;
		return -1;
	}
	
	return 0;
}