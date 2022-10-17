export module Song;
import <istream>;
import <format>;
import <optional>;
import <set>;
import <memory>;
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
		std::set<std::shared_ptr<SinglesChart>, SinglesChart::Comparator> singlesCharts;
		std::set<std::shared_ptr<DoublesChart>, DoublesChart::Comparator> doublesCharts;
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

		const auto GetSinglesCharts() const
		{
			return singlesCharts;
		}

		const auto GetDoublesCharts() const
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
				if (tokens.first == SongConstants::Notes)
					LoadStepchart(parser);
				else
					throw ParseException(std::format("Syntax error in line {}: {}", parser.LineNumber, lineView));
			}
		}

		void LoadStepchart(Parser& parser)
		{
			std::string line;
			std::string_view lineView;

			parser.ReadLine(line);
			lineView = line;
			StringUtils::Trim(lineView);

			auto type = ParseType(lineView);
			if (type)
			{
				if (type.value() == PlayStyle::Single)
				{
					auto chart = std::make_shared<SinglesChart>();
					chart->DoLoad(parser, ChartMatchInfo::Any, ChartReadMode::ReadFirstChart);
					//chart->SetParent(std::shared_ptr<Song>(this)); // TOOD need to split implementation to talk about the same Song
					singlesCharts.insert(chart);
				}
				else if (type.value() == PlayStyle::Double)
				{
					auto chart = std::make_shared<DoublesChart>();
					chart->DoLoad(parser, ChartMatchInfo::Any, ChartReadMode::ReadFirstChart);
					//chart->SetParent(std::shared_ptr<Song>(this)); // TOOD need to split implementation to talk about the same Song
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

		std::optional<PlayStyle> ParseType(const std::string_view& lineView)
		{
			if (lineView.starts_with(SongConstants::SinglesChartType))
				return PlayStyle::Single;
			if (lineView.starts_with(SongConstants::DoublesChartType))
				return PlayStyle::Double;
			return std::nullopt;
		}
	};
}