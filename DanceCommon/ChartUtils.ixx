export module ChartUtils;
import StdCore;
import Song;
import Chart;

namespace DanceCommon
{
	export class ChartUtils
	{
	public:
		static string GetChartPlayPath(const string& songPath, const Song& song, const SinglesChart& chart)
		{
			string chartName = song.GetChartName(chart, true);
			FilterFilenameIllegalCharacters(chartName);
			size_t filenameIndex = songPath.rfind('\\');
			return songPath.substr(0, filenameIndex)
				+ '\\'
				+ chartName + ".play";
		}

		static void FilterFilenameIllegalCharacters(string& filename) {
			std::ranges::replace(filename, '/', ' ');
			std::ranges::replace(filename, '\\', ' ');
		}

		static int Count(const string& str, char chr)
		{
			int count = 0;
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == chr)
					count++;
			}
			return count;
		}
	};
}