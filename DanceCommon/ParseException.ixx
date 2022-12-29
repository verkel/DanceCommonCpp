export module ParseException;

import StdCore;

export namespace DanceCommon
{
	export class ParseException : public exception
	{
	private:
		string message;

	public:
		ParseException(string message) : message{ message }
		{ }

		const char* what()
		{
			return message.c_str();
		}
	};
}