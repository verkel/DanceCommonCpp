export module NoteType;

export namespace DanceCommon
{
	export enum class NoteType
	{
		Unknown,
		Empty,
		Tap,
		HoldStart,
		HoldEnd,
		RollStart,
		Mine,
		VirtualTap
	};
}