//---------------------------------------------------------------------------------------------------------------------
//  RGBD_TOOLS
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2018 Pablo Ramon Soria (a.k.a. Bardo91) pabramsor@gmail.com
//---------------------------------------------------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
//  and associated documentation files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge, publish, distribute,
//  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all copies or substantial
//  portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
//  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------


#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#ifdef GPU
	#include "cuda_runtime.h"
	#include "curand.h"
	#include "cublas_v2.h"
	#include <cuda_runtime_api.h>
	#include <cuda.h>
#endif

extern "C" {
    #include "darknet/network.h"
    #include "darknet/image.h"
}

class WrapperDarknet{
public:
    ///
    WrapperDarknet(std::string mModelFile, std::string mWeightsFile);


    /// [class prob left top right bottom];
    std::vector<std::vector<float> > detect(const cv::Mat& img);

private:
    list *mOptions;
    network *mNet;
    detection *mBoxes;
    float **mProbs;
    float **mMasks;

    bool mImageInit = false;
    image mIm;
};

