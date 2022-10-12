export module SongMetadata;
import <string>;
import <stdexcept>;
import <map>;
import <optional>;
import <format>;
import Parser;
import SongUtils;
import SongConstants;
import StringUtils;
import ParseException;

export namespace DanceCommon
{
	class SongMetadata
	{
		std::string title;
		std::string subtitle;
		std::string artist;
		std::string titletranslit;
		std::string subtitletranslit;
		std::string artisttranslit;
		std::string genre;
		std::string credit;
		std::string banner;
		std::string background;
		std::string lyricspath;
		std::string cdtitle;
		std::string music;
		double offset = 0.0;
		double samplestart = 0.0;
		double samplelength = 0.0;
		std::string selectable;
		std::map<double, double> bpms;
		std::map<double, double> stops;
		std::string bgchanges;
		std::string keysounds;
		std::string displaybpm;

	public:
		SongMetadata(Parser& parser)
		{
			std::optional<std::pair<std::string_view, std::string_view>> dataLine;
			std::string linesBuffer;
			while (dataLine = SongUtils::ReadDataLine(parser, linesBuffer, true))
			{
				auto& l = dataLine->first;
				auto& c = dataLine->second;

				if (l == SongConstants::Notes)
					break;
				else
					parser.SkipPeekedLines();

				// Read a metadata entry
				if (l == SongConstants::Title) title = c;
				else if (l == SongConstants::Subtitle) subtitle = c;
				else if (l == SongConstants::Artist) artist = c;
				else if (l == SongConstants::Titletranslit) titletranslit = c;
				else if (l == SongConstants::Subtitletranslit) subtitletranslit = c;
				else if (l == SongConstants::Artisttranslit) artisttranslit = c;
				else if (l == SongConstants::Genre) genre = c;
				else if (l == SongConstants::Credit) credit = c;
				else if (l == SongConstants::Banner) banner = c;
				else if (l == SongConstants::Background) background = c;
				else if (l == SongConstants::Lyricspath) lyricspath = c;
				else if (l == SongConstants::Cdtitle) cdtitle = c;
				else if (l == SongConstants::Music) music = c;
				else if (l == SongConstants::Offset)
				{
					if (!StringUtils::TryParseDouble(c, offset))
						throw ParseException(std::format("Cannot parse offset: {}", c));
				}
				else if (l == SongConstants::Samplestart)
				{
					if (!StringUtils::TryParseDouble(c, samplestart))
						throw ParseException(std::format("Cannot parse samplestart: {}", c));
				}
				else if (l == SongConstants::Samplelength)
				{
					if (!StringUtils::TryParseDouble(c, samplelength))
						throw ParseException(std::format("Cannot parse samplelength: {}", c));
				}
				else if (l == SongConstants::Selectable) selectable = c;
				else if (l == SongConstants::Bpms) bpms = ParseMapFromString(c);
				else if (l == SongConstants::Stops) stops = ParseMapFromString(c);
				else if (l == SongConstants::Bgchanges) bgchanges = c;
				else if (l == SongConstants::Bgchanges2) {} // Omit ITG2 style #BGCHANGES2
				else if (l == SongConstants::Keysounds) keysounds = c;
				else if (l == SongConstants::Displaybpm) displaybpm = c;
			}
		}

		std::map<double, double> ParseMapFromString(std::string_view sv)
		{
			std::map<double, double> map;
			
			if (sv == "")
				return map;

			std::vector<std::string_view> tokens = StringUtils::Split(sv, ',');
			std::vector<std::string_view> subtokens;

			for (auto& token : tokens)
			{
				StringUtils::Split(token, '=', subtokens);
				if (subtokens.size() != 2)
					throw ParseException(std::format("Malformed subtokens: {}", token));

				double key, value;
				if (!StringUtils::TryParseDouble(subtokens[0], key))
					throw ParseException(std::format("Cannot parse key: {}", token));
				if (!StringUtils::TryParseDouble(subtokens[1], value))
					throw ParseException(std::format("Cannot parse value: {}", token));

				map[key] = value;
			}

			return map;
		}

		const std::string& GetTitle()
		{
			return title;
		}

		const std::string& GetArtist()
		{
			return artist;
		}

		const std::string& GetCredit()
		{
			return credit;
		}

		const std::string& GetBanner()
		{
			return banner;
		}

		const std::string& GetBackground()
		{
			return background;
		}

		const std::string& GetCdtitle()
		{
			return cdtitle;
		}

		const std::string& GetMusic()
		{
			return music;
		}

		const double GetOffset()
		{
			return offset;
		}

		const double GetSamplestart()
		{
			return samplestart;
		}

		const double GetSamplelength()
		{
			return samplelength;
		}

		const std::string& GetSelectable()
		{
			return selectable;
		}
	};
}