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
* Unwrapper.cuh
* 
* Author: Ilker GURCAN
*/

#ifndef UNWRAPPER_CUH_
#define UNWRAPPER_CUH_

#include<stdlib.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>

#define DIVIDE_INTO(x,y) ((x + y - 1)/y)

#define WARP_SIZE 32
#define MAX_BLOCK_SIZE 512
#define WARPS_PER_BLOCK (MAX_BLOCK_SIZE / WARP_SIZE)

class Unwrapper {

public:
	__host__
	Unwrapper(int resWidth, 
			  int resHeight, 
			  float xCenter, 
			  float yCenter, 
			  float radius, 
			  float alfa, 
			  float beta);
	__host__
	virtual ~Unwrapper();
	__host__
	void init(int imW, int imH, size_t stride, int numChannels);
	__host__
	unsigned char* unwrap(unsigned char *inputImg);
private:
	cudaTextureObject_t texture;
	cudaArray *cuArray;
	void *dUnwrappedImg;
	int imW;
	int imH;
	size_t stride;
	int numChannels;
	int resWidth;
	int resHeight;
	float xCenter;
	float yCenter;
	float radius;
	float alfa;
	float beta;

	__host__
	void initTexture();
};

#endif