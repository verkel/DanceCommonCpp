export module Chart;
import StdCore;
import NotePos;
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

	template<size_t rowSize>
	class Chart
	{
	private:
		using TNoteRow = NoteRow<rowSize>;

		NoteData<rowSize> noteData;
		ChartInfo info;

		shared_ptr<Song> parent;

	public:
		struct Comparator
		{
			bool operator()(shared_ptr<Chart> a, shared_ptr<Chart> b) const
			{
				return *a < *b;
			}
		};


		/**
		* Public constructor for creating empty stepcharts
		*/
		Chart() :
			noteData{ },
			info { Difficulty::Easy, 0, "" }
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
		Chart(istream& stream, const ChartMatchInfo& matchInfo) : Chart{ }
		{
			constexpr auto style = PlayStyles::GetStyle(rowSize);
			auto chartStyleString = GetStyleString(style);

			string line;
			string_view lineView;

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
			return info < rhs.info;
		}

		const ChartInfo& GetInfo() const
		{
			return info;
		}

		const string& GetDescription() const
		{
			return info.description;
		}

		Difficulty GetDifficulty() const
		{
			return info.difficulty;
		}

		int GetRating() const
		{
			return info.rating;
		}

		bool Matches(const ChartMatchInfo& matchInfo) const
		{
			return info.Matches(matchInfo);
		}

		shared_ptr<Song> GetParent() const
		{
			return parent;
		}

		void SetParent(shared_ptr<Song> parent)
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

		TNoteRow GetNoteRow(NotePos pos) const
		{
			return noteData.GetNoteRow(pos);
		}

		bool Contains(NotePos pos) const
		{
			return noteData.Contains(pos);
		}

		NotePos NextPosition(NotePos pos) const
		{
			return noteData.NextPosition(pos);
		}

		NotePos NextPosition(NotePos position, int skipEmptyPanel) const
		{
			return noteData.NextPosition(position, skipEmptyPanel);
		}

		NotePos NextPosition(NotePos position, int skipEmptyPanel, NotePos upperBound) const
		{
			return noteData.NextPosition(position, skipEmptyPanel, upperBound);
		}

		NotePos PreviousPosition(NotePos pos) const
		{
			return noteData.PreviousPosition(pos);
		}

		NotePos GetLastPosition() const
		{
			return noteData.GetLastPosition();
		}

		NotePos FindHoldEnd(NotePos notePosition, bool includeThisPosition, int panel) const
		{
			return noteData.FindHoldEnd(notePosition, includeThisPosition, panel);
		}

		bool DoLoad(Parser& parser, const ChartMatchInfo& matchInfo, ChartReadMode readMode)
		{
			constexpr auto style = PlayStyles::GetStyle(rowSize);

			if (readMode == ChartReadMode::ReadFirstChart || readMode == ChartReadMode::ReadMatchingChart)
			{
				// Read chart metadata
				string descriptionStr, difficultyClassStr, ratingStr;
				auto descriptionStrView = ReadSubDataLine(parser, descriptionStr);
				auto difficultyClassStrView = ReadSubDataLine(parser, difficultyClassStr);
				auto ratingStrView = ReadSubDataLine(parser, ratingStr);

				string skippedLine;
				parser.ReadLine(skippedLine); // Skip groove radar data

				int rating;
				if (!StringUtils::TryParseInt(ratingStrView, rating))
					throw ParseException(format("Cannot parse rating: {}", ratingStrView));

				Difficulty difficulty = Difficulties::FromFormatString(difficultyClassStrView);

				ChartInfo info{ difficulty, rating, string(descriptionStrView) };

				if (readMode == ChartReadMode::ReadMatchingChart)
				{
					if (!info.Matches(matchInfo))
						return false;
				}

				this->info = info;
			}

			// Now read the actual steps data
			string line;
			string_view lineView;
			size_t measureCount = 0;
			vector<TNoteRow> measureRows;
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
					throw ParseException(format("Malformed note row at line {}", parser.LineNumber));

				// If this row is a measure terminator, continue onward to next row
				if (isMeasureTerminator || isChartTerminator)
				{
					size_t rowsCount = measureRows.size();

					if (rowsCount > 0)
					{
						NotePos posOffset = NoteLength::Measure * (int)measureCount;
						NoteLength noteLength = NoteLengths::FromResolution((int)rowsCount);
						if (noteLength == NoteLength::None)
							throw ParseException(format("Unsupported measure resolution {}", rowsCount));

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
		static const string& GetStyleString(PlayStyle style) {
			if (style == PlayStyle::Single) return SongConstants::SinglesChartType;
			else if (style == PlayStyle::Double) return SongConstants::DoublesChartType;
			throw invalid_argument("style");
		}

		static string_view ReadSubDataLine(Parser& parser, string& line)
		{
			if (parser.ReadLine(line))
			{
				string_view result = line;
				StringUtils::Trim(result);
				size_t end = result.length() - 1;
				if (result[end] == ':')
					result = StringUtils::SubstrStartEnd(result, 0, end);
				return result;
			}

			throw ParseException(format("Cannot read sub data line: {}", line));
		}
	};

	export using SinglesChart = Chart<NoteRowSize::Single>;
	export using DoublesChart = Chart<NoteRowSize::Double>;
}