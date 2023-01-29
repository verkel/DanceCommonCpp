export module NoSuchChartException;

import StdCore;
import ChartInfo;
import PlayStyle;
import Difficulty;

export namespace DanceCommon
{
	export class NoSuchChartException : public exception
	{
	private:
		string message;

	public:
		NoSuchChartException(const ChartMatchInfo& info)
		{
			stringstream ss;
			ss << "No such chart:";
			if (info.difficulty)
				ss << " Difficulty=" << Difficulties::GetName(info.difficulty.value());
			if (info.rating)
				ss << " Rating=" << info.rating.value();
			if (info.description)
				ss << " Description=" << info.description.value();
			this->message = ss.str();
		}

		const char* what()
		{
			return message.c_str();
		}
	};
}