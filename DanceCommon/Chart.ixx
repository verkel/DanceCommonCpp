export module Chart;
import NotePos;
import <string>;
import <memory>;
import <iostream>;
import <format>;
import SongConstants;
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

namespace DanceCommon
{
	class Song;
}

export namespace DanceCommon
{
	enum class ChartReadMode
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

	template<NoteRowSize rowSize>
	class Chart
	{
	private:
		using TNoteRow = NoteRow<rowSize>;

		NoteData<rowSize> noteData;
		std::string description;
		Difficulty difficulty;
		int rating;

		std::shared_ptr<Song> parent;

	public:
		struct Comparator
		{
			bool operator()(std::shared_ptr<Chart> a, std::shared_ptr<Chart> b) const
			{
				return *a < *b;
			}
		};


		/**
		* Public constructor for creating empty stepcharts
		*/
		Chart() :
			noteData{ },
			difficulty{ Difficulty::Easy },
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
		Chart(std::istream& stream, const ChartMatchInfo& matchInfo) : Chart{ }
		{
			constexpr auto style = PlayStyles::GetStyle(rowSize);
			auto chartStyleString = GetStyleString(style);

			std::string line;
			std::string_view lineView;

			Parser parser{ stream, 1 };

			while (parser.ReadLine(line))
			{
				lineView = line;
				StringUtils::Trim(lineView);

				if (lineView.starts_with(chartStyleString))
				{
					if (DoLoad(parser, matchInfo, ChartReadMode::ReadMatchingChart))
						return;
				}
			}

			throw NoSuchChartException(matchInfo);
		}

		bool operator<(const Chart& rhs) const
		{
			if (difficulty != rhs.difficulty)
				return difficulty < rhs.difficulty;

			if (rating != rhs.rating)
				return rating < rhs.rating;

			return description < rhs.description;
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

		void SetParent(std::shared_ptr<Song> parent)
		{
			this->parent = parent;
		}

		NotePos GetLength() const
		{
			return noteData.GetLength();
		}

		void SetNoteRow(NotePos pos, TNoteRow newRow, bool cleanupHolds)
		{
			noteData.SetNoteRow(pos, newRow, cleanupHolds);
		}

		TNoteRow GetNoteRow(NotePos pos)
		{
			return noteData.GetNoteRow(pos);
		}

		bool Contains(NotePos pos)
		{
			return noteData.Contains(pos);
		}

		NotePos NextPosition(NotePos pos)
		{
			return noteData.NextPosition(pos);
		}

		NotePos PreviousPosition(NotePos pos)
		{
			return noteData.PreviousPosition(pos);
		}

		NotePos GetLastPosition()
		{
			return noteData.GetLastPosition();
		}

		bool DoLoad(Parser& parser, const ChartMatchInfo& matchInfo, ChartReadMode readMode)
		{
			constexpr auto style = PlayStyles::GetStyle(rowSize);

			if (readMode == ChartReadMode::ReadFirstChart || readMode == ChartReadMode::ReadMatchingChart)
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

				if (readMode == ChartReadMode::ReadMatchingChart)
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
				StringUtils::Trim(lineView);

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

	private:
		static const std::string& GetStyleString(PlayStyle style) {
			if (style == PlayStyle::Single) return SongConstants::SinglesChartType;
			else if (style == PlayStyle::Double) return SongConstants::DoublesChartType;
			throw std::invalid_argument("style");
		}

		static std::string_view ReadSubDataLine(Parser& parser, std::string& line)
		{
			if (parser.ReadLine(line))
			{
				std::string_view result = line;
				StringUtils::Trim(result);
				size_t end = result.length() - 1;
				if (result[end] == ':')
					result = StringUtils::SubstrStartEnd(result, 0, end);
				return result;
			}

			throw ParseException(std::format("Cannot read sub data line: {}", line));
		}
	};

	export using SinglesChart = Chart<NoteRowSizeSingle>;
	export using DoublesChart = Chart<NoteRowSizeDouble>;
}