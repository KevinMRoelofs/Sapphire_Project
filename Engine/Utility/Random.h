#pragma once
#include <memory>

//#define RANDOMSEED

namespace Sapphire
{
	/// <summary>
	/// This class provides random nummber generation of ints and floats withing the passed boundaries
	/// </summary>
	class Random
	{
	public:
		/// <summary>
		/// Initializes the Seed based on the time here
		/// </summary>
		Random();

		/// <summary>
		/// Generate a number within the passed boundaries
		/// </summary>
		/// <param name="min">The minimum number to be generated</param>
		/// <param name="max">The maximum number to be generated</param>
		/// <returns>Returns a number from min to max</returns>
		int const& GenerateInt(int const& min, int const& max);

		/// <summary>
		/// Generate a number within the passed boundaries
		/// </summary>
		/// <param name="min">The minimum number to be generated</param>
		/// <param name="max">The maximum number to be generated</param>
		/// <returns>Returns a number from min to max</returns>
		float const& GenerateFloat(float const& min, float const& max);
	
		void myFunction();

		unsigned int seed_;

	private:

		int lastResultInt_;
		float lastResultFloat_;
	};

	extern Random random_;
} //namespace Common

