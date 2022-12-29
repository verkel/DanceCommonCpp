export module NoSuchChartException;

import StdCore;
import ChartInfo;

export namespace DanceCommon
{
	export class NoSuchChartException : public std::exception
	{
	private:
		std::string message;

	public:
		NoSuchChartException(const ChartMatchInfo& info)
		{
			std::stringstream ss;
			ss << "No such chart:";
			if (info.style)
				ss << " Style=" << PlayStyles::GetName(info.style.value());
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