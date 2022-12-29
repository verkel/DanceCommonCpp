export module Parser;

import StdCore;

export class Parser
{
private:
	istream& stream;
	queue<string> peeks;

public:
	int LineNumber;

	Parser(istream& stream, int lineNumber) :
		stream{stream},
		LineNumber{lineNumber}
	{
	}

	bool ReadLine(string& result)
	{
		if (!peeks.empty()) 
		{
			result = peeks.front();
			peeks.pop();
			LineNumber++;
			return true;
		}

		if (getline(stream, result))
		{
			LineNumber++;
			return true;
		}
		
		return false;
	}

	bool PeekLine(string& result)
	{
		string line;
		if (getline(stream, line))
		{
			peeks.push(line);
			result = line;
			return true;
		}
		return false;
	}

	bool ReadOrPeekLine(bool peek, string& result)
	{
		if (peek)
			return PeekLine(result);
		else
			return ReadLine(result);
	}

	void SkipPeekedLines()
	{
		peeks = {};
	}
};