export module Difficulty;
import StdCore;
import ParseException;

export namespace DanceCommon
{
	export enum class Difficulty
	{
		Novice,
		Easy,
		Medium,
		Hard,
		Expert,
		Edit,
	};

	export class Difficulties
	{
	private:
		inline static constexpr string_view DifficultyAnyName = "Any";

		static constexpr array<string_view, 6> NamesByDifficulty
		{
			"Novice",
			"Easy",
			"Medium",
			"Hard",
			"Expert",
			"Edit"
		};

		static constexpr array<string_view, 6> FormatStringsByDifficulty
		{
			"Beginner",
			"Easy",
			"Medium",
			"Hard",
			"Challenge",
			"Edit"
		};

	public:
		static constexpr Difficulty FromFormatString(string_view sv)
		{
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Novice)])
				return Difficulty::Novice;
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Easy)])
				return Difficulty::Easy;
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Medium)])
				return Difficulty::Medium;
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Hard)])
				return Difficulty::Hard;
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Expert)])
				return Difficulty::Expert;
			if (sv == FormatStringsByDifficulty[static_cast<int>(Difficulty::Edit)])
				return Difficulty::Edit;

			throw ParseException(format("Unknown difficulty: {}", string(sv)));
		}

		static constexpr string_view GetName(Difficulty difficulty)
		{
			return NamesByDifficulty[static_cast<int>(difficulty)];
		}

		static constexpr string_view GetName(optional<Difficulty> difficulty)
		{
			if (difficulty)
			{
				return NamesByDifficulty[static_cast<int>(difficulty.value())];
			}

			return DifficultyAnyName;
		}
	};
}