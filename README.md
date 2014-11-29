<h1>Unwrapping OmniDirectional Images</h1>

    Copyright 2010-2014 METU, Middle East Technical University, Informatics Institute
    This file is part of "Unwrapping OmniDirectional Images" Library.
    "Unwrapping OmniDirectional Images" is free software: 
    you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    "Unwrapping OmniDirectional Images" is distributed in the hope 
    that it will be useful, but WITHOUT ANY WARRANTY; without 
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License.
    If not, see <http://www.gnu.org/licenses/>. 
    Please contact Alptekin TEMIZEL for more info about 
    licensing atemizel@metu.edu.tr.
  
<h2>Introduction</h2>

<p>The transformation of Omni-directional (OD) images into perspective-view images is an intensive task which takes significant time especially when the image resolution is high. Recently, Graphics Processing Units (GPU) have been started to be used for process intensive tasks, including various image processing algorithms, and significant speed-ups against CPU counterparts have been reported. In this paper, we propose parallel implementation of un-warping OD images on GPU, implemented using Compute Unified Device Architecture (CUDA). The performance of the proposed parallel algorithm has been analysed on two different GPU devices against the performance of its sequential version executed on the CPU. Our test results indicate a speed-up of up to 32 times by the execution of the parallel algorithm on the GPU. The performance analysis also indicates that the memory copy operations take significant amount of time on the GPU implementation. Hence we further analysed these copy operations and suggested optimizations for them.</p>

<p>Even though the uploaded word document had been written down in the year 2010; all parameters passed to the new API remained the same. Please check out that file for construction parameters.</p>

<h2>System Requirements</h2>

<p> Although your requirements may vary (due to many reasons such as the quality of the image your recorder device outputs), following table illustrates the platform "Unwrapping OmniDirectional Images" had been tested. Running it on a Platform with "System Requirements" equal to the one given in the table below or higher is recommended. Only important thing that should be taken into consideration is that Kepler Specific instructions and keywords are used in "Unwrapping OmniDirectional Images" library; hence  using a CUDA-powered GPU with version of 3.0+ is a mandotary.</p>

|  Id  |  Requirement  | Test Platform |
|:----:|:-------------:|:-------------:|
| 1    | OS            | Windows 7 x64 |
| 2    | CPU           | Intel i7 4770K 3.5 GHz<br/> |
| 3    | GPU           | NVIDIA Tesla K40c Compute Capability 3.5,<br/> 15 SMs 192 Cores per SM, 2 Async Copy Engine,<br/> Hyper-Q Enabled |
| 4    | RAM           | 32GB DDR3 |
| 5    | Serial Computer <br/>Expansion Bus| PCIe 2.1 |
| 6    | CUDA Toolkit  | 6.0 |
| 7    | CUDA Driver   | 6.0 |
| 8    | CUDA Runtime  | 6.0 |
| 9    | OpenCV        | 2.4.9 |

<h2>Installation Details</h2>

<p>
There is no specific installer for now. All you require (except the ones defined in "Systems Requirements" section) are Visual Studio, CUDA Toolkit and OpenCV for generating dll and executable files. All project specific files in uploaded solution were created by Visual Studio 2012. Please ensure that you installed NVIDIA Nsight, right after you'd installed Visual Studio IDE; otherwise your projects will not be detected as CUDA projects.<br/> 
There are two separate projects in the solution : <br/>
<ul>
 <li>One for "Unwrapping OmniDirectional Images" Library which produces dll and lib files.</li>
 <li>One for Testing the Library which produces an executable file.</li>
</ul>

</p>

<p>
Paths to "Unwrapping OmniDirectional Images" Library's header and library files are defined with respect to the project's root folder. However, for OpenCV repeat the steps given below for both projects :
<ul>
 <li>Select the Project.</li>
 <li>Click on Project->Properties->Configuration Properties->C/C++->General->Additional Include Directories.</li>
 <li>Specify where the include folder for OpenCV Library is.</li>
 <li>Click on Project->Properties->Configuration Properties->Linker->General->Additional Library Directories.</li>
 <li>Specify where lib folder for OpenCV Library is. All Required library files are already defined in Input sub-section.</li>
</ul>
</p>

<p>
 As for CUDA support : 
 <ul>
  <li>Select the Project for "Unwrapping OmniDirectional Images" Library which is "FlatOmniLib".</li>
  <li>Click on Project->Properties->Configuration Properties->CUDA C/C++->Device->Code Generation.</li>
  <li>You should specify compute capability and SM architecture versions to optimize your code in accord with the device you have. For us, they were both equal to 3.5</li>
 </ul>
</p>

<h2>Usage</h2>

<p>Four different samples (varying in size) of the same omnidirectional image are uploaded for testing. Files are as follows (Image was taken by Ladybug Omnidirectional Camera System) : <br/>
<ul>
 <li>39,894222,32,815392_Original.jpg : Original Image 5184x3456</li>
 <li>39,894222,32,815392_Quartered.jpg : Original Image 2592x1728</li>
 <li>39,894222,32,815392_OneOver16.jpg : Original Image 1296x864</li>
 <li>39,894222,32,815392_Quadrupled.jpg : Original Image 10368x6912</li>
</ul>
</p>

<p>
To run the application : 
<ul>
 <li>Open a Command Line.</li>
 <li>Change Directory to $(SolutionDir)\x64\Release.</li>
 <li>Type : TestOmniApp.exe "39,894222,32,815392_Original.jpg"</li>
 <li>Press on Enter.</li>
</ul>
</p>

<p>You should be able to see that "tryMeBoy.jpeg" file has just been generated in exactly the same folder as where the executable file resides in.</p>

<p>
In the Main.cpp file, there you should see two parameters were passed to the method "unwrap".
<ul>
<li>If the first parameter is an empty string; then it is user's responsibility to save the file using the C pointer returned by this method.</li>
<li>Second parameter is a boolean value. Since there might be some distortions due to unwrapping; you should tell the API whether it should run a smoothing filter on the resulting image or not (smoothing is run on the GPU as well).</li>
</ul>
</p>

<h2>Notes</h2>
<p>
<ul>
 <li>Keep in mind that a Tesla card was used during the test phase for it eliminates the overhead brought by display drivers like WDDM. This is valid for Windows Platforms only in the case you run H-TLD on Windows Vista and beyond. There might be some temporal solutions proposed on NVIDIA's web site to breach WDDM like drivers.</li>
 <li>Note that, CUDA Texture Objects made running multiple instances of the new API in separate threads possible. Now users may create several instances of the "OmniOperations" class for which they may unwrap different images from different resources in separate threads (such as via OpenMP) concurrently.</li>
 <li>As of yet, only CV_8UC1 (gra level), CV_8UC3 (bgr), and CV_8UC4 (bgra) image types are supported by the API.</li>
</ul>
</p>
