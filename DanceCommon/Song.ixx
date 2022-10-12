export module Song;
import <istream>;
import <format>;
import <optional>;
import <set>;
import <memory>;
import SongConstants;
import SongMetadata;
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
		std::set<std::shared_ptr<SinglesChart>> singlesCharts;
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