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
* OmniOperations.h
* 
* Author: Ilker GURCAN
*/

#ifndef OMNI_OPERATIONS_H_
#define OMNI_OPERATIONS_H_

#include<opencv2\core\core.hpp>
#include<opencv2\gpu\gpu.hpp>
#include<string>
#include "ProjectVersion.h"
#include "Unwrapper.cuh"

class OmniOperations { 
public : 
	OmniOperations(float xCenter, 
				   float yCenter, 
				   float radius, 
				   float innerRadius, 
				   float alfa, 
				   float beta,
				   double sigma);
	virtual ~OmniOperations(void);
	//alfa&beta values should be defined between [0.0 < (alfa|beta) <= 1.0]
	FLAT_OMNI_API cv::Mat* unwrap(cv::Mat& inputImg, 
		                          std::string& outPath,
								  bool isSmoothOnGPU);
private:
	//Fields...
	bool isInitialized;
	double sigma;
	int resWidth;//Result Image Width
	int resHeight;//Result Image Height
	int savingWidth;
	int savingHeight;
	cv::Mat *inputRef;
	cv::Mat *flatImg;
	Unwrapper *unwrapper;
	cv::Ptr<cv::gpu::FilterEngine_GPU> gaussianFilterGpu;/*Why Not Simple GaussianBlur : http://docs.opencv.org/modules/gpu/doc/image_filtering.html#gpu-filterengine-gpu */
	cv::gpu::GpuMat *dBlurredImg;
	//Methods...
	void init(cv::Mat& inputImg);
};

#endif