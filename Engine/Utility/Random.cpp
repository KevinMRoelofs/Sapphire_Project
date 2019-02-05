#include "Engine/Utility/Random.h"
#include <cstdlib>
#include <ctime>

namespace Sapphire
{
	Random random_;

	Random::Random(): lastResultInt_(0), lastResultFloat_(0.0f)
	{
		seed_ = 0;
#ifdef RANDOMSEED
		//Use current time as seed
		seed_ = unsigned int(std::time(nullptr));
#endif
		std::srand(seed_);
	}

	int const& Random::GenerateInt(int const& min, int const& max)
	{
		//Generate a number with the passed min and max flipped if the passed min is bigger than max
		if(min > max)
		{
			//debug_warning("Random", "GenerateInt", "Passed Min value is larger than passed Max, returning GenerateInt() with passed Min as Max and passed Max as Min.");
			return GenerateInt(max, min);
		}
		
		//Return the passed min if the passed min and max are the same
		if (min == max)
		{
			//debug_warning("Random", "GenerateInt", "Min and Max values passed have the same value, returning passed Min value.");
			return min;
		}

		//Generate the result and store it
		lastResultInt_ = std::rand() % (max + 1 - min) + min;

		//Return the result
		return lastResultInt_;
	}

	float const& Random::GenerateFloat(float const& min, float const& max)
	{
		//Generate a number with the passed min and max flipped if the passed min is bigger than max
		if (min > max)
		{
			//debug_warning("Random", "GenerateFloat", "Passed Min value is larger than passed Max, returning GenerateInt() with passed Min as Max and passed Max as Min.");
			return GenerateFloat(max, min);
		}

		//Return the passed min if the passed min and max are the same
		if (min == max)
		{
			//debug_warning("Random", "GenerateFloat", "Min and Max values passed have the same value, returning passed Min value.");
			return min;
		}

		//Generate the result and store it
		const float shifter = 100.0f;
		lastResultFloat_ = float(std::rand() % int(floorf((max - min) * shifter))) / shifter + min;
		

		//Return the result
		return lastResultFloat_;
	}
} //namespace Common
