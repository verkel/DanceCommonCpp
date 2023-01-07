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
			size_t filenameIndex = songPath.rfind('\\');
			return songPath.substr(0, filenameIndex)
				+ '\\'
				+ song.GetChartName(chart, true) + ".play";
		}

		/*static string GetChartPlayPath(const string& songPath, const Song& song, const SinglesChart& chart)
		{
			string playFilename = song.GetChartName(chart, true) + ".play";
			int dotCount = Count(playFilename, '.');
			FilterFilenameIllegalCharacters(playFilename);

			size_t extensionIndex = songPath.rfind(".sm");
			if (extensionIndex == string::npos)
				throw invalid_argument("songPath");

			size_t folderIndex = string_view(songPath).substr(extensionIndex - 1).rfind('.');

			for (size_t i = 1; i < dotCount; i++)
			{
				folderIndex = string_view(songPath).substr(folderIndex - 1).rfind('.');

				if (folderIndex == string::npos)
					throw invalid_argument("songPath");
			}

			return songPath.substr(0, folderIndex) + "." + playFilename;
		}*/

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