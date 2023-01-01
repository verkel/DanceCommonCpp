export module Song;

import StdCore;
import SongConstants;
import SongMetadata;
import SongMetadataComputations;
import Parser;
import StringUtils;
import SongUtils;
import ParseException;
import PlayStyle;
import Chart;
import ChartInfo;

export namespace DanceCommon
{
	class Song
	{
	private:
		SongMetadata metadata;
		set<shared_ptr<SinglesChart>, SinglesChart::Comparator> singlesCharts;
		set<shared_ptr<DoublesChart>, DoublesChart::Comparator> doublesCharts;
		double musicLength;

	public:
		static Song Load(istream& stream)
		{
			Parser parser{ stream, 1 };
			return Song(parser);
		}

		static shared_ptr<Song> LoadAsSharedPtr(istream& stream)
		{
			Parser parser{ stream, 1 };
			return make_shared<Song>(parser);
		}

		Song(Parser& parser) :
			metadata{parser},
			musicLength{0.0}
		{
			DoLoad(parser);
		}

		const SongMetadata& GetMetadata() const
		{
			return metadata;
		}

		const auto& GetSinglesCharts() const
		{
			return singlesCharts;
		}

		const auto& GetDoublesCharts() const
		{
			return doublesCharts;
		}

		double GetPosition(double time)
		{
			auto& computations = metadata.GetComputations();
			auto& bpmsByTime = computations.GetBpmsByTime();
			auto& stopsByTime = computations.GetStopsByTime();

			auto bpmLowerBound = bpmsByTime.lower_bound(time);
			double bpmTime = bpmLowerBound != bpmsByTime.end()
				? bpmLowerBound->first
				: bpmsByTime.rbegin()->first;
			auto bpmEvent = bpmsByTime.at(bpmTime);

			double stopElapsedSeconds = 0.0;
			auto stopLowerBound = stopsByTime.lower_bound(time);
			bool stopTimeFound = stopLowerBound != stopsByTime.end();
			double stopTime = stopTimeFound
				? stopLowerBound->first
				: 0.0;

			double eventTime;
			SongMetadataComputations::Event evt;
			if (!stopTimeFound || bpmTime >= stopTime)
			{
				evt = bpmEvent;
				eventTime = bpmTime;
			}
			else
			{
				auto stopEvent = stopsByTime.at(stopTime);
				stopElapsedSeconds = time - stopTime;
				if (stopElapsedSeconds > stopEvent.value)
					stopElapsedSeconds = stopEvent.value;

				evt = stopEvent;
				eventTime = stopTime;
			}

			double secondsAfterEvent = time - eventTime;
			double bpm = bpmEvent.value;

			// bpm/60 = beats per second, bps*48 = position increase per second
			return evt.position + (secondsAfterEvent - stopElapsedSeconds) * bpm / 60.0 * 48.0;
		}

		double GetTime(NotePos position)
		{
			return GetTime(position, true);
		}

		double GetTime(NotePos position, bool addOffset)
		{
			auto& bpms = metadata.GetBpms();
			auto& stops = metadata.GetStops();
			double givenBeat = NotePositions::ToBeatsDouble(position);
			double beat = 0.0;

			if (!bpms.contains(beat))
				throw exception("Chart does not specify initial BPM");

			double bpm = bpms.at(beat);
			double lastBeat = 0.0;
			double lastBpm = 0.0;
			double time = 0.0;

			do
			{
				lastBeat = beat;
				lastBpm = bpm;

				auto beatUpperBound = bpms.upper_bound(beat);
				bool beatFound = beatUpperBound != bpms.end();
				
				if (beatFound)
				{
					beat = beatUpperBound->first;
				}

				if (!beatFound || beat >= givenBeat)
				{
					beat = givenBeat;
				}
				
				auto bpmItr = bpms.find(beat);
				if (bpmItr != bpms.end())
					bpm = bpmItr->second;

				double beatPeriod = beat - lastBeat;
				double secondsPerBeat = 60.0 / lastBpm;
				time += beatPeriod * secondsPerBeat;
			} while (beat != givenBeat);

			time += GetStopAddition(givenBeat, stops);

			if (addOffset)
				return time - metadata.GetOffset();
			else
				return time;
		}

	private:
		void DoLoad(Parser& parser)
		{
			string line;
			string_view lineView;

			while (parser.ReadLine(line))
			{
				lineView = line;
				StringUtils::Trim(lineView);

				if (lineView == "" || line.starts_with("//"))
					continue;

				auto tokens = SongUtils::ParseDataLine(lineView);
				if (tokens.first == SongConstants::Notes)
					LoadStepchart(parser);
				else
					throw ParseException(format("Syntax error in line {}: {}", parser.LineNumber, lineView));
			}
		}

		void LoadStepchart(Parser& parser)
		{
			string line;
			string_view lineView;

			parser.ReadLine(line);
			lineView = line;
			StringUtils::Trim(lineView);

			auto type = ParseType(lineView);
			if (type)
			{
				if (type.value() == PlayStyle::Single)
				{
					auto chart = make_shared<SinglesChart>();
					chart->DoLoad(parser, ChartMatchInfo::Any, ChartReadMode::ReadFirstChart);
					singlesCharts.insert(chart);
				}
				else if (type.value() == PlayStyle::Double)
				{
					auto chart = make_shared<DoublesChart>();
					chart->DoLoad(parser, ChartMatchInfo::Any, ChartReadMode::ReadFirstChart);
					doublesCharts.insert(chart);
				}
			}
			else
			{
				// Ignore unsupported chart types
				while (parser.ReadLine(line))
				{
					if (StringUtils::Contains(line, ';'))
						break;
				}
			}
		}

		optional<PlayStyle> ParseType(const string_view& lineView)
		{
			if (lineView.starts_with(SongConstants::SinglesChartType))
				return PlayStyle::Single;
			if (lineView.starts_with(SongConstants::DoublesChartType))
				return PlayStyle::Double;
			return nullopt;
		}

		double GetStopAddition(double givenBeat, const map<double, double>& stops) {
			double addition = 0.0;
			double beat = -1.0;
			map<double, double>::const_iterator stop;
			while ((stop = stops.upper_bound(beat)) != stops.end())
			{
				beat = stop->first;

				if (beat >= givenBeat)
					break;

				double stop = stops.at(beat);
				addition += stop;
			}
			return addition;
		}
	};
}