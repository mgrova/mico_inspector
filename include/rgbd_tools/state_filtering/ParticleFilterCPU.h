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

#ifndef RGBDTOOLS_OBJECTDETECTION_FEATUREBASED_FILTERCPU_H_
#define RGBDTOOLS_OBJECTDETECTION_FEATUREBASED_FILTERCPU_H_

#include <vector>
#include <map>

namespace rgbd {

#include <vector>

// Particle interface
class Particle {
  public:
	virtual void simulate() = 0;
	virtual void calcWeigh(Particle &_realParticle) = 0;

	double weigh() const { return mWeigh; };

  protected:
	double mWeigh;
}; //	 class Particle

// Particle filter class
template <typename ParticleType_>
class ParticleFilterCPU{
  public:
	ParticleFilterCPU(unsigned _nuParticles) : mNuParticles(_nuParticles){};

	void init();
	void step(ParticleType_ &_realParticle);

	unsigned nuParticles() const { return mNuParticles; };
	std::vector<ParticleType_> particles() const { return mParticles; };

  private:
	void simulate();
	void calcWeigh(ParticleType_ &_realParticle);
	void resample();

  private:
	unsigned mNuParticles;
	std::vector<ParticleType_> mParticles;

}; // class ParticleFilterCPU

} // namespace rgbd
#include "ParticleFilterCPU.inl"

#endif //	RGBDTOOLS_OBJECTDETECTION_FEATUREBASED_FILTERCPU_H_
