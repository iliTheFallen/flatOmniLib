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
* Unwrapper.cu
* 
* Author: Ilker GURCAN
*/

#include<cstring>
#include<math.h>
#include<opencv2\core\core.hpp>
#include "Unwrapper.cuh"

__host__
Unwrapper::Unwrapper(int resWidth, 
					 int resHeight, 
					 float xCenter, 
					 float yCenter, 
					 float radius, 
					 float alfa, 
					 float beta) {

	this->resWidth  = resWidth;
	this->resHeight = resHeight;
	this->xCenter   = xCenter;
	this->yCenter   = yCenter;
	this->radius    = radius;
	this->alfa      = alfa;
	this->beta      = beta;
	texture         = 0;
	cuArray         = 0;
	dUnwrappedImg   = NULL;
}

__host__
void Unwrapper::init(int imW, int imH, size_t stride, int numChannels) {

	this->imW         = imW;
	this->imH         = imH;
	this->stride      = stride;
	this->numChannels = numChannels;
	initTexture();
	//Allocate Data for Output...
	cudaMalloc(&dUnwrappedImg, resWidth * resHeight * sizeof(unsigned char) * numChannels);
}

__host__
void Unwrapper::initTexture() {

	struct cudaResourceDesc resDesc;
	struct cudaTextureDesc texDesc;
	struct cudaChannelFormatDesc channelDesc;

	if(numChannels == 4)
		channelDesc = cudaCreateChannelDesc(8, 8, 8, 8, cudaChannelFormatKindUnsigned);
	else 
		channelDesc = cudaCreateChannelDesc(8, 0, 0, 0, cudaChannelFormatKindUnsigned);
	//Allocate Texture Data on GPU...
	cudaMallocArray(&cuArray, &channelDesc, stride / numChannels, imH);
	//Specify texture Data...
	memset(&resDesc, 0, sizeof(resDesc));
	resDesc.resType         = cudaResourceTypeArray;
	resDesc.res.array.array = cuArray;
	//Specify texture Object Parameters...
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.addressMode[0]   = cudaAddressModeWrap;
	texDesc.addressMode[1]   = cudaAddressModeWrap;
	texDesc.filterMode       = cudaFilterModePoint;
	texDesc.normalizedCoords = 0;
	texDesc.readMode         = cudaReadModeElementType;
	//Create Texture Object...
	cudaCreateTextureObject(&texture, &resDesc, &texDesc, NULL);
}

__host__
Unwrapper::~Unwrapper() {

	if(texture)
		cudaDestroyTextureObject(texture);
	if(cuArray)
		cudaFreeArray(cuArray);
	if(dUnwrappedImg != NULL)
		cudaFree(dUnwrappedImg);
}

/**
* !!!!!!!!!!!!!!!!!!!!!!Texture Objects Require Compute Capability >= 3.0!!!!!!!!!!!!!!!!!!!!!
**/
template <typename T>
__global__ void unwrapImageKernel(cudaTextureObject_t texture, 
								  T *dUnwrappedImg, 
								  float radius, 
								  float xCenter, 
								  float yCenter, 
								  int width, 
								  int height, 
								  float oneOverAlfa, 
								  float oneOverBeta) { 
	
	const int bId      = blockIdx.x;
	const int tGId     = bId * MAX_BLOCK_SIZE + threadIdx.x;
	const int tWId     = tGId & (WARP_SIZE - 1);
	const int warpGId  = tGId / WARP_SIZE;
	const int gridSize = WARPS_PER_BLOCK * WARP_SIZE * gridDim.x;
	//Variables Used Within The Algorithm...
	float sourceX, sourceY;

	for(int row = warpGId; row < height; row+=gridSize) { 
		for(int i = tWId; i < width; i+=WARP_SIZE) { 
			//Scan The Oval Image In The Reverse Direction (That is Why We Multiply by -1)...
			sourceX = (xCenter + oneOverAlfa * row * __cosf(-1 * i * oneOverBeta / radius));
			sourceY = (yCenter + oneOverAlfa * row * __sinf(-1 * i * oneOverBeta / radius));
			//Origin Of The Coordinate System For The Destination Image Is Bottom-Left Corner Of It...
			//In Other Words--->(0,height)====>Origin Of The Coordinate System...
			const int curIndex = width * ((height - 1) - row) + i;
			//Nearest Neighbour...
			dUnwrappedImg[curIndex] = tex2D<T>(texture, sourceX, sourceY);
		}//End Of Inner-for Loop...
	}//End Of Outer-for Loop...
	
}//End Of flatImageKernel Method...

unsigned char* Unwrapper::unwrap(unsigned char *inputImg) {

	int numWarps  = resHeight;
	int numBlocks = DIVIDE_INTO(WARP_SIZE * numWarps, MAX_BLOCK_SIZE);
	dim3 gridDim(numBlocks, 1, 1);
	dim3 blockDim(MAX_BLOCK_SIZE, 1, 1);

	//Move Data to GPU...
	cudaMemcpyToArray(cuArray, 
		              0, 
					  0, 
					  (void*)inputImg, 
					  stride * imH, 
					  cudaMemcpyHostToDevice);
	if(numChannels == 4) 
		unwrapImageKernel<uchar4><<<gridDim, blockDim>>>(texture, 
													     reinterpret_cast<uchar4*>(dUnwrappedImg), 
														 radius, 
														 xCenter, 
														 yCenter, 
														 resWidth, 
														 resHeight, 
														 1.0f / alfa, 
														 1.0f / beta);
	else 
		unwrapImageKernel<uchar><<<gridDim, blockDim>>>(texture, 
														reinterpret_cast<uchar*>(dUnwrappedImg), 
														radius, 
														xCenter, 
														yCenter, 
														resWidth, 
														resHeight, 
														1.0f / alfa, 
														1.0f / beta);
	cudaDeviceSynchronize();
	return reinterpret_cast<unsigned char*>(dUnwrappedImg);
}
