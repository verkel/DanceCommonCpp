export module Difficulty;
import <array>;
import <string>;
import <string_view>;
import <format>;
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
	public:
		static constexpr std::array NamesByDifficulty
		{
			"Novice",
			"Easy",
			"Medium",
			"Hard",
			"Expert",
			"Edit"
		};

		static constexpr std::array FormatStringsByDifficulty
		{
			"Beginner",
			"Easy",
			"Medium",
			"Hard",
			"Challenge",
			"Edit"
		};

		static Difficulty FromFormatString(const std::string_view& sv)
		{
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Novice])
				return Difficulty::Novice;
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Easy])
				return Difficulty::Easy;
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Medium])
				return Difficulty::Medium;
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Hard])
				return Difficulty::Hard;
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Expert])
				return Difficulty::Expert;
			if (sv == FormatStringsByDifficulty[(int)Difficulty::Edit])
				return Difficulty::Edit;

			throw ParseException(std::format("Unknown difficulty: {}", std::string(sv)));
		}
	};
}