export module LimbsOnPad;

import PadPanels;
import Panel;
import Limb;
import <initializer_list>;
import <stdexcept>;

export namespace DanceCommon
{
	export template<size_t rowSize>
	struct LimbsOnPad : public PadPanels<Limb, rowSize>
	{
		using TPadPanels = PadPanels<Limb, rowSize>;
		using TPadPanels::PadPanels; // inherit constructors

		void MoveLimb(Panel panel, Limb limb)
		{
			RemoveLimb(limb);
			PlaceLimb(panel, limb);
		}

		Panel RemoveLimb(Limb limb)
		{
			Panel panel = GetOccupyingPanel(limb);
			if (panel != Panel::None)
			{
				if ((*this)[panel] == Limbs::BothLegs) (*this)[panel] = Limbs::GetPair(limb);
				else (*this)[panel] = Limb::None;
				return panel;
			}
			else
			{
				return Panel::None;
			}
		}

		void PlaceLimb(Panel panel, Limb limb)
		{
			Limb limbOnPanel = (*this)[panel];
			if (limbOnPanel == Limb::None)
			{
				(*this)[panel] = limb;
			}
			else if (Limbs::IsLeg(limb) && limb == Limbs::GetPair(limbOnPanel))
			{
				(*this)[panel] = Limbs::BothLegs;
			}
			else
			{
				throw std::logic_error("Cannot move limb into panel: the panel is already occupied with another limb");
			}
		}

		Panel GetOccupyingPanel(Limb limb)
		{
			for (int i = TPadPanels::First(); i < TPadPanels::End(); i++)
			{
				if (Limbs::Contains((*this)[i], limb))
					return Panels::ForIndex(i);
			}

			return Panel::None;
		}
	};
}