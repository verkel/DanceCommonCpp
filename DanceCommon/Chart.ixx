export module Chart;
import Song;
import <string>;
import <memory>;
import <iostream>;
import Difficulty;
import NoteData;
import ChartInfo;
import Parser;

export class Chart
{
private:
	static inline const std::string SinglesChartType = "dance-single";
	static inline const std::string DoublesChartType = "dance-double";

	NoteData noteData;
	std::string description;
	Difficulty difficulty;
	double rating;

	std::shared_ptr<Song> parent;

public:

	/**
	* Public constructor for creating empty stepcharts
	*/
	Chart(PlayStyle style, Difficulty difficulty) : 
		noteData{style}, 
		difficulty{difficulty},
		rating{0.0},
		parent{nullptr}
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
	Chart(std::istream& stream, const ChartInfo& matchInfo) :
		Chart{matchInfo.style, matchInfo.difficulty}
	{
		Parser parser{ stream, 1 };

		for (std::string line; parser.ReadLine(line);)
		{
			std::cout << line << std::endl;
		}
	}

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
};