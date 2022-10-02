export module Song;
import <istream>;
import <format>;
import SongMetadata;
import Parser;
import StringUtils;
import SongUtils;
import ParseException;

export namespace DanceCommon
{
	class Song
	{
	private:
		inline static const std::string Notes = "#NOTES";

		SongMetadata metadata;
		double musicLength;

	public:
		static Song Load(std::istream& stream)
		{
			Parser parser{ stream, 1 };
			return Song(parser);
		}

		const SongMetadata& GetMetadata() const
		{
			return metadata;
		}

	private:
		Song(Parser& parser) :
			metadata{parser},
			musicLength{0.0}
		{
			DoLoad(parser);
		}

		void DoLoad(Parser& parser)
		{
			std::string line;
			std::string_view lineView;

			while (parser.ReadLine(line))
			{
				lineView = line;
				StringUtils::Trim(lineView);

				if (lineView == "" || line.starts_with("//"))
					continue;

				auto tokens = SongUtils::ParseDataLine(lineView);
				if (tokens.first == Notes)
					LoadStepchart(parser);
				else
					throw ParseException(std::format("Syntax error in line {}: {}", parser.LineNumber, lineView));
			}
		}

		void LoadStepchart(Parser& parser)
		{
			// TODO implement
		}
	};
}