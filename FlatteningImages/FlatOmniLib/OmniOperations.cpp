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
* OmniOperations.cpp
* 
* Author: Ilker GURCAN
*/

#include "OmniOperations.h"
#include "Unwrapper.cuh"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

OmniOperations::OmniOperations(float xCenter, 
							   float yCenter, 
							   float radius, 
							   float innerRadius, 
							   float alfa, 
							   float beta,
							   double sigma) { 

	isInitialized = false;
	this->sigma   = sigma;
	//Find Required Values for The Result Image...
	resWidth      = (int)(2 * CV_PI * radius * beta);
	resHeight     = (int)(radius * alfa);
	savingWidth   = resWidth;
	savingHeight  = (int)(resHeight - innerRadius * alfa);
	flatImg       = NULL;
	dBlurredImg   = NULL;
	inputRef      = NULL;
	unwrapper     = new Unwrapper(resWidth,
							      resHeight,
								  xCenter,
								  yCenter,
								  radius,
								  alfa,
								  beta);
}

void OmniOperations::init(cv::Mat& inputImg) {

	//That is Because gpu::createGaussianFilter_GPU Accepts Only CV_8UC1 & CV_8UC4
	//(Provided That the Data type is Equal to 8U = unsigned char, of course!)
	int numChannels = inputImg.channels();
	size_t stride   = inputImg.step[0];
	if(numChannels >= 3) {
		//After Conversion...
		if(numChannels == 3) {
			numChannels = 4;
			stride      = (stride / 3) * 4;
			inputRef    = new cv::Mat(inputImg.cols, inputImg.rows, CV_8UC4);
		}//End of Innermost-if-Block...
	}//End of Outer-if-Block...
	flatImg     = new cv::Mat(resHeight, resWidth, CV_MAKETYPE(CV_8U, numChannels));
	dBlurredImg = new cv::gpu::GpuMat(resHeight, resWidth, CV_MAKETYPE(CV_8U, numChannels));
	//Check Out http://docs.opencv.org/modules/imgproc/doc/filtering.html#getgaussiankernel
	gaussianFilterGpu = cv::gpu::createGaussianFilter_GPU(CV_MAKETYPE(CV_8U, numChannels), 
														  cv::Size((int)(2 * sigma) + 1, (int)(2 * sigma) + 1), 
														  sigma, 
														  0.0,
														  cv::BORDER_CONSTANT,
														  cv::BORDER_CONSTANT);

	unwrapper->init(inputImg.cols, inputImg.rows, stride, numChannels);
	isInitialized = true;
}

OmniOperations::~OmniOperations(void) { 

	if(flatImg != NULL) 
		delete flatImg;
	if(unwrapper)
		delete unwrapper;
	if(inputRef != NULL)
		delete inputRef;
	if(dBlurredImg != NULL)
		delete dBlurredImg;
}

cv::Mat* OmniOperations::unwrap(cv::Mat& inputImg,
								std::string& outPath,
								bool isSmoothOnGPU) { 

	unsigned char* dUnwrappedImg = NULL;
	int type;

	if(!inputImg.empty()) { 
		if(!isInitialized)
			init(inputImg);
		//*****************************************************************************
		//*************************Invoke The Kernel...********************************
		//*****************************************************************************
		if(inputRef != NULL) {
			cv::cvtColor(inputImg, *inputRef, CV_BGR2BGRA, 4);
			dUnwrappedImg = unwrapper->unwrap(inputRef->data);
			type          = CV_8UC4;
		} else {
			dUnwrappedImg = unwrapper->unwrap(inputImg.data);
			type          = CV_8UC1;
		}//End of else-Block...
		if(dUnwrappedImg != NULL) {
			//*****************************************************************************
			//***********************Smoothing Image on GPU...*****************************
			//*****************************************************************************
			cv::gpu::GpuMat dImg(resHeight,
								 resWidth,
								 type,
								 (void*)dUnwrappedImg);
			if(isSmoothOnGPU) {
				gaussianFilterGpu->apply(dImg,
										 *dBlurredImg,
										 cv::Rect(0, 0, resWidth, resHeight));
				dBlurredImg->download(*flatImg);
			} else
				dImg.download(*flatImg);
			//*****************************************************************************
			//****************************Save the Image...********************************
			//*****************************************************************************
			if(!outPath.empty()) 
				cv::imwrite(outPath, 
				            (*flatImg)(cv::Rect(0, 0, savingWidth, savingHeight)));
			return flatImg;
		}//End of if(dUnwrappedImg != NULL)...
	}//End of Outermost-if-Block...

	return NULL;
}
