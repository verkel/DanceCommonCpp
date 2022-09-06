export module Parser;
import <fstream>;
import <queue>;
import <string>;

export class Parser
{
private:
	std::istream& stream;
	std::queue<std::string> peeks;

public:
	int LineNumber;

	Parser(std::istream& stream, int lineNumber) :
		stream{stream},
		LineNumber{lineNumber}
	{
	}

	bool ReadLine(std::string& result)
	{
		if (!peeks.empty()) 
		{
			result = peeks.front();
			peeks.pop();
			LineNumber++;
			return true;
		}

		if (std::getline(stream, result))
		{
			LineNumber++;
			return true;
		}
		
		return false;
	}

	bool PeekLine(std::string& result)
	{
		std::string line;
		if (std::getline(stream, line))
		{
			peeks.push(line);
			result = line;
			return true;
		}
		return false;
	}

	bool ReadOrPeekLine(bool peek, std::string& result)
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