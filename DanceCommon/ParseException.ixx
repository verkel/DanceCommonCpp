export module ParseException;

import StdCore;

export namespace DanceCommon
{
	export class ParseException : public std::exception
	{
	private:
		std::string message;

	public:
		ParseException(std::string message) : message{ message }
		{ }

		const char* what()
		{
			return message.c_str();
		}
	};
}