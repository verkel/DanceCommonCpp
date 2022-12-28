export module Computations;

import FeetPlacement;

namespace DanceCommon
{
	export class Computations
	{
	public:
		FeetPlacements feetPlacements;

		static Computations& GetInstance()
		{
			static Computations instance;
			return instance;
		}

		Computations(Computations const&) = delete;
		void operator=(Computations const&) = delete;

	private:
		Computations() :
			feetPlacements{}
		{}
	};
}
