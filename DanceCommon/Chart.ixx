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
import ParseException;
import NoSuchChartException;

export namespace DanceCommon
{
	template<NoteRowSize rowSize>
	class Chart
	{
	private:
		typedef NoteRow<rowSize> TNoteRow;

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

		NoteData<rowSize> noteData;
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

				if (lineView.starts_with(chartStyleString))
				{
					if (DoLoad(parser, matchInfo, ReadMode::ReadMatchingChart))
						return;
				}
			}

			throw NoSuchChartException(matchInfo);
		}

		bool DoLoad(Parser parser, const ChartMatchInfo& matchInfo, ReadMode readMode)
		{
			constexpr auto style = PlayStyles::GetStyle(rowSize);

			if (readMode == ReadMode::ReadFirstChart || readMode == ReadMode::ReadMatchingChart)
			{
				// Read chart metadata
				std::string descriptionStr, difficultyClassStr, ratingStr;
				auto descriptionStrView = ReadSubDataLine(parser, descriptionStr);
				auto difficultyClassStrView = ReadSubDataLine(parser, difficultyClassStr);
				auto ratingStrView = ReadSubDataLine(parser, ratingStr);

				std::string skippedLine;
				parser.ReadLine(skippedLine); // Skip groove radar data

				int rating;
				if (!StringUtils::TryParseInt(ratingStrView, rating))
					throw ParseException(std::format("Cannot parse rating: {}", ratingStrView));

				Difficulty difficulty = Difficulties::FromFormatString(difficultyClassStrView);

				ChartInfo info{ style, difficulty, rating, std::string(descriptionStrView) };

				if (readMode == ReadMode::ReadMatchingChart)
				{
					if (!info.Matches(matchInfo))
						return false;
				}

				this->description = std::string(descriptionStrView);
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
				bool isChartTerminator = StringUtils::Contains(lineView, ';');
				bool isMeasureTerminator = StringUtils::Contains(lineView, ',');

				if (len != expectedRowLength && !isMeasureTerminator && !isChartTerminator)
					throw ParseException(std::format("Malformed note row at line {}", parser.LineNumber));

				// If this row is a measure terminator, continue onward to next row
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

						measureRows.clear();
						measureCount++;
					}

					if (isChartTerminator) break;
					else continue;
				}

				// Now it should be a note row. Add the row into the current measure.
				measureRows.push_back(TNoteRow{ lineView });
			}

			// Chart parsed successfully
			return true;
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

		inline NotePos GetLength() const
		{
			return noteData.GetLength();
		}

		inline void SetNoteRow(NotePos pos, TNoteRow newRow, bool cleanupHolds)
		{
			noteData.SetNoteRow(pos, newRow, cleanupHolds);
		}

		inline TNoteRow GetNoteRow(NotePos pos)
		{
			return noteData.GetNoteRow(pos);
		}

		inline bool Contains(NotePos pos)
		{
			return noteData.Contains(pos);
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

		static std::string_view ReadSubDataLine(Parser& parser, std::string& line)
		{
			if (parser.ReadLine(line))
			{
				std::string_view result = line;
				Trim(result);
				size_t end = result.length() - 1;
				if (result[end] == ':')
					StringUtils::SubstrStartEnd(result, 0, end);
				return result;
			}

			throw ParseException(std::format("Cannot read sub data line: {}", line));
		}
	};

	export typedef Chart<NoteRowSizeSingle> SinglesChart;
	export typedef Chart<NoteRowSizeDouble> DoublesChart;
}