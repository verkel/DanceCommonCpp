export module Chart;
import NotePos;
import Song;
import <string>;
import <memory>;
import <iostream>;
import Difficulty;
import NoteData;
import ChartInfo;
import Parser;
import StringUtils;

export namespace DanceCommon
{
	export class Chart
	{
	private:
		enum class ReadMode
		{
			/**
			 * Mode for reading first chart in the input
			 */
			ReadFirstChart,
			/**
			 * Mode for reading chart with the matching difficulty (and optionally
			 * description) in the input
			 */
			ReadMatchingChart,
			/**
			 * Mode for reading bare noteRows without any header
			 */
			ReadNotes
		};

		static inline const std::string SinglesChartType = "dance-single";
		static inline const std::string DoublesChartType = "dance-double";

		NoteData noteData;
		std::string description;
		Difficulty difficulty;
		int rating;

		std::shared_ptr<Song> parent;

	public:
		/**
		* Public constructor for creating empty stepcharts
		*/
		Chart(PlayStyle style, Difficulty difficulty) :
			noteData{ style },
			difficulty{ difficulty },
			rating{ 0 },
			parent{ nullptr }
		{
		}

		/**
		* Load a lone stepchart from a simfile.
		* <p>
		* Loads a chart matching given info from the stream pointing to a simfile.
		* If no such chart is found, ParseException is thrown.
		*
		* @param stream The stream from which to read the chart
		* @param matchInfo The description of the stepchart to match
		* @throws ParseException If parsing fails for some reason
		* @throws NoSuchChartException If no chart with the given difficulty is
		*            found
		*/
		Chart(std::istream& stream, const ChartMatchInfo& matchInfo) :
			Chart{ matchInfo.style.value(), matchInfo.difficulty.value() }
		{
			Parser parser{ stream, 1 };
			std::string line;
			std::string_view lineView;

			while (parser.ReadLine(line))
			{
				lineView = line;
				Trim(lineView);

				auto chartStyleString = GetStyleString(matchInfo.style.value());

				if (line.starts_with(chartStyleString))
				{
					DoLoad(parser, matchInfo, ReadMode::ReadMatchingChart);
				}
			}

			/*for (std::string line; parser.ReadLine(line);)
			{
				std::cout << line << std::endl;
			}*/
		}

		bool DoLoad(Parser parser, const ChartMatchInfo& matchInfo, ReadMode readMode)
		{
			auto style = noteData.GetStyle();

			if (readMode == ReadMode::ReadFirstChart || readMode == ReadMode::ReadMatchingChart)
			{
				// Read chart metadata
				std::string descriptionStr, difficultyClassStr, ratingStr;
				std::string_view descriptionStrView, difficultyClassStrView, ratingStrView;
				
				if (!ReadSubDataLine(parser, descriptionStr, descriptionStrView))
					return false;

				if (!ReadSubDataLine(parser, difficultyClassStr, difficultyClassStrView))
					return false;

				if (!ReadSubDataLine(parser, ratingStr, ratingStrView))
					return false;

				std::string skippedLine;
				parser.ReadLine(skippedLine); // Skip groove radar data

				if (!TryParseInt(ratingStrView, rating))
					return false;

				ChartInfo info{ style, difficulty, rating, std::string(descriptionStrView) };

				if (readMode == ReadMode::ReadMatchingChart)
				{
					if (!info.Matches(matchInfo))
						return false;
				}

				this->description = description;
				this->difficulty = difficulty;
				this->rating = rating;
			}

			// Now read the actual steps data
			// TODO

			return false;
		}

		std::string GetDescription() const
		{
			return description;
		}

		Difficulty GetDifficulty() const
		{
			return difficulty;
		}

		int GetRating() const
		{
			return rating;
		}

		std::shared_ptr<Song> GetParent() const
		{
			return parent;
		}

		NotePos GetLength() const
		{
			return -1;
		}

	private:

		static void Trim(std::string_view& s)
		{
			s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
			s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
		}

		std::string GetStyleString(PlayStyle style) {
			if (style == PlayStyle::Single) return SinglesChartType;
			else if (style == PlayStyle::Double) return DoublesChartType;
			throw std::invalid_argument("style");
		}

		static bool ReadSubDataLine(Parser& parser, std::string& line, std::string_view& result)
		{
			if (parser.ReadLine(line))
			{
				result = line;
				Trim(result);
				size_t end = result.length() - 1;
				if (result[end] == ':')
					SubstrStartEnd(result, 0, end);
				return true;
			}

			return false;
		}
	};
}