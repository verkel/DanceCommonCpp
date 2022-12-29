export module Computations;
import StdCore;
import FeetPlacement;
import Panel;

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

		static const FeetPlacement& GetFeetPlacement(Panel leftLegPanel, Panel rightLegPanel)
		{
			return GetInstance().feetPlacements.Get(leftLegPanel, rightLegPanel);
		}

	private:
		Computations() :
			feetPlacements{}
		{}
	};
}
