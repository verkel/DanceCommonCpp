export module FeetPlacement;

import StdCore;
import LimbsOnPad;
import PlayStyle;
import FacingType;
import Limb;
import Panel;

namespace DanceCommon
{
	export class FeetPlacement
	{
	private:
		int angle = -1;
		Panel leftLegPanel = Panel::None, rightLegPanel = Panel::None;
		Panel leftHandPanel = Panel::None, rightHandPanel = Panel::None;
		FacingType facingType = FacingType::FaceIn;

	public:
		constexpr FeetPlacement()
		{ }

		constexpr FeetPlacement(int angle, FacingType facingType, Limb leftLimb, Limb downLimb, Limb upLimb, Limb rightLimb)
			: angle(angle),
			facingType(facingType)
		{
			SetPanel(leftLimb, Panel::Left);
			SetPanel(downLimb, Panel::Down);
			SetPanel(upLimb, Panel::Up);
			SetPanel(rightLimb, Panel::Right);
		}

		int GetAngle() const
		{
			return angle;
		}

		Panel GetLeftLegPanel() const
		{
			return leftLegPanel;
		}

		Panel GetRightLegPanel() const
		{
			return rightLegPanel;
		}

		Panel GetLeftHandPanel() const
		{
			return leftHandPanel;
		}

		Panel GetRightHandPanel() const
		{
			return rightHandPanel;
		}

		FacingType GetFacingType() const
		{
			return facingType;
		}

	private:
		constexpr void SetPanel(Limb limb, Panel panel)
		{
			switch (limb)
			{
				case Limb::None: 
					break;
				case Limb::LeftLeg:
					leftLegPanel = panel;
					break;
				case Limb::RightLeg:
					rightLegPanel = panel;
					break;
				case Limb::LeftHand:
					leftHandPanel = panel;
					break;
				case Limb::RightHand:
					rightHandPanel = panel;
					break;
				default:;
			}
		}
	};

	export class FeetPlacements
	{
		using enum FacingType;
		using enum Limb;

	public:
		// --------------------------------------------------------------------------
		// Positions where player faces away from the center
		// --------------------------------------------------------------------------

		/**
		 *  \[R]
		 * [L] [ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement L_R_ = FeetPlacement(-45, FaceOut, LeftLeg, LeftHand, RightLeg, RightHand);

		/**
		 *   [L]/
		 * [ ] [R]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement __LR = FeetPlacement(45, FaceOut, LeftHand, RightHand, LeftLeg, RightLeg);

		/**
		 *   [ ]
		 * [ ] [L]
		 *   [R]\
		 */
		static inline constexpr FeetPlacement _R_L = FeetPlacement(135, FaceOut, RightHand, RightLeg, LeftHand, LeftLeg);

		/**
		 *   [ ]
		 * [R] [ ]
		 *  /[L]
		 */
		static inline constexpr FeetPlacement RL__ = FeetPlacement(-135, FaceOut, RightLeg, LeftLeg, RightHand, LeftHand);

		// --------------------------------------------------------------------------
		// Positions where player faces towards the center
		// --------------------------------------------------------------------------

		/**
		 *   [L]
		 * [R]\[ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement R_L_ = FeetPlacement(135, FaceIn, RightLeg, RightHand, LeftLeg, LeftHand);

		/**
		 *   [R]
		 * [ ]/[L]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement __RL = FeetPlacement(-135, FaceIn, RightHand, LeftHand, RightLeg, LeftLeg);

		/**
		 *   [ ]
		 * [ ]\[R]
		 *   [L]
		 */
		static inline constexpr FeetPlacement _L_R = FeetPlacement(-45, FaceIn, LeftHand, LeftLeg, RightHand, RightLeg);

		/**
		 *   [ ]
		 * [L]/[ ]
		 *   [R]
		 */
		static inline constexpr FeetPlacement LR__ = FeetPlacement(45, FaceIn, LeftLeg, RightLeg, LeftHand, RightHand);

		// --------------------------------------------------------------------------
		// Positions where player faces straight up, down, left or right
		// --------------------------------------------------------------------------

		/**
		 *
		 *   [|]
		 * [L] [R]
		 *   [ ]
		 *
		 */
		// Note: in this, hands might be either way
		// One way has to be picked for bot play though
		static inline constexpr FeetPlacement L__R = FeetPlacement(0, FaceToSide, LeftLeg, RightHand, LeftHand, RightLeg);

		/**
		 *   [R]
		 * [-] [ ]
		 *   [L]
		 */
		static inline constexpr FeetPlacement _LR_ = FeetPlacement(-90, FaceToSide, LeftHand, LeftLeg, RightLeg, RightHand);

		/**
		 *   [ ]
		 * [R] [L]
		 *   [|]
		 */
		// Note: in this, hands might be either way
		// One way has to be picked for bot play though
		static inline constexpr FeetPlacement R__L = FeetPlacement(180, FaceToSide, RightLeg, LeftHand, RightHand, LeftLeg);

		/**
		 *   [L]
		 * [ ] [-]
		 *   [R]
		 */
		static inline constexpr FeetPlacement _RL_ = FeetPlacement(90, FaceToSide, LeftHand, RightLeg, LeftLeg, RightHand);

		// --------------------------------------------------------------------------
		// Positions for left leg only (right on the center panel)
		// --------------------------------------------------------------------------

		/**
		 *   [ ]
		 * [L]R[ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement L___ = FeetPlacement(0, FaceToSide, LeftLeg, None, None, None);

		/**
		 *   [ ]
		 * [ ]R[ ]
		 *   [L]
		 */
		static inline constexpr FeetPlacement _L__ = FeetPlacement(-90, FaceToSide, None, LeftLeg, None, None);

		/**
		 *   [L]
		 * [ ]R[ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement __L_ = FeetPlacement(90, FaceToSide, None, None, LeftLeg, None);

		/**
		 *   [ ]
		 * [ ]R[L]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement ___L = FeetPlacement(180, FaceToSide, None, None, None, LeftLeg);

		// --------------------------------------------------------------------------
		// Positions for right leg only (left on the center panel)
		// --------------------------------------------------------------------------

		/**
		 *   [ ]
		 * [R]L[ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement R___ = FeetPlacement(180, FaceToSide, RightLeg, None, None, None);

		/**
		 *   [ ]
		 * [ ]L[ ]
		 *   [R]
		 */
		static inline constexpr FeetPlacement _R__ = FeetPlacement(90, FaceToSide, None, RightLeg, None, None);

		/**
		 *   [R]
		 * [ ]L[ ]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement __R_ = FeetPlacement(-90, FaceToSide, None, None, RightLeg, None);

		/**
		 *   [ ]
		 * [ ]L[R]
		 *   [ ]
		 */
		static inline constexpr FeetPlacement ___R = FeetPlacement(0, FaceToSide, None, None, None, RightLeg);

		// --------------------------------------------------------------------------
		// The empty position
		// --------------------------------------------------------------------------

		/**
		 *   [ ]
		 * [ ]B[ ] = both feet middle
		 *   [ ]
		 */
		static inline constexpr FeetPlacement ____ = FeetPlacement(0, FaceToSide, None, None, None, None);

		// --------------------------------------------------------------------------
		// Public API
		// --------------------------------------------------------------------------

		static constexpr array<FeetPlacement, 21> Values
		{
			// Positions where player faces away from the center
			L_R_, __LR, _R_L, RL__,
			// Positions where player faces towards the center
			R_L_, __RL, _L_R, LR__,
			// Positions where player faces straight up, down, left or right
			L__R, _LR_, R__L, _RL_,
			// Positions for left leg only (right on the center panel)
			L___, _L__, __L_, ___L,
			// Positions for right leg only (left on the center panel)
			R___, _R__, __R_, ___R,
			// The empty position
			____
		};

		const FeetPlacement& Get(Panel leftLegPanel, Panel rightLegPanel)
		{
			const auto byRightLegItr = byLeftLegAndRightLeg.find(leftLegPanel); 
			if (byRightLegItr != byLeftLegAndRightLeg.end())
			{
				const auto& byRightLeg = byRightLegItr->second;
				const auto resultItr = byRightLeg.find(rightLegPanel);
				if (resultItr != byRightLeg.end())
				{
					return resultItr->second;
				}
			}

			//return byLeftLegAndRightLeg[leftLegPanel][rightLegPanel];

			return Get(Panel::Left, Panel::Right);
		}

		FeetPlacements()
		{
			for (auto placement : Values)
			{
				// [] will insert the default value if it doesn't exist
				byLeftLegAndRightLeg[placement.GetLeftLegPanel()][placement.GetRightLegPanel()] = placement;
			}
		}

	private:
		unordered_map<Panel, unordered_map<Panel, FeetPlacement>> byLeftLegAndRightLeg;
	};
}