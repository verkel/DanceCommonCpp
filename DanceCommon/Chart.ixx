export module Chart;
import NotePos;
import Song;
import <string>;
import <memory>;
import <iostream>;
import <format>;
import Difficulty;
import NoteData;
import NoteRow;
import NoteLength;
import ChartInfo;
import Parser;
import StringUtils;
import PlayStyle;

export namespace DanceCommon
{
	export class Chart
	{
	private:
		typedef NoteRow<SizeSingle> TNoteRow;

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

		class ParseException : public std::exception
		{
		private:
			std::string message;

		public:
			ParseException(std::string message) : message{message}
			{ }

			const char* what()
			{
				return message.c_str();
			}
		};

		static inline const std::string SinglesChartType = "dance-single";
		static inline const std::string DoublesChartType = "dance-double";

		NoteData<SizeSingle> noteData;
		std::string description;
		Difficulty difficulty;
		int rating;

		std::shared_ptr<Song> parent;

	public:
		/**
		* Public constructor for creating empty stepcharts
		*/
		Chart(PlayStyle style, Difficulty difficulty) :
			noteData{ },
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
			constexpr auto style = PlayStyles::GetStyle(SizeSingle);

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
			std::string line;
			std::string_view lineView;
			size_t measureCount = 0;
			std::vector<TNoteRow> measureRows;
			int expectedRowLength = PlayStyles::ButtonCount(style);

			while (parser.ReadLine(line))
			{
				lineView = line;
				Trim(lineView);
				
				if (lineView.empty() || lineView.starts_with("//"))
					continue;

				size_t len = lineView.size();
				bool isChartTerminator = Contains(lineView, ';');
				bool isMeasureTerminator = Contains(lineView, ',');

				if (len != expectedRowLength && !isMeasureTerminator && !isChartTerminator)
					throw ParseException(std::format("Malformed note row at line {}", parser.LineNumber));

				if (isMeasureTerminator || isChartTerminator)
				{
					size_t rowsCount = measureRows.size();

					if (rowsCount > 0)
					{
						NotePos posOffset = NoteLength::Measure * (int)measureCount;
						NoteLength noteLength = NoteLengths::FromResolution((int)rowsCount);
						if (noteLength == NoteLength::None)
							throw ParseException(std::format("Unsupported measure resolution {}", rowsCount));

						for (int i = 0; i < rowsCount; i++)
						{
							NotePos pos = posOffset + noteLength * i;
							SetNoteRow(pos, measureRows[i], false);
						}
					}
				}
			}

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

		void SetNoteRow(NotePos pos, TNoteRow newRow, bool cleanupHolds)
		{
			noteData.SetNoteRow(pos, newRow, cleanupHolds);
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