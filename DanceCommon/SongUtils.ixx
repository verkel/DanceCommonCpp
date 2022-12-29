export module SongUtils;

import StdCore;
import Parser;
import ParseException;
import StringUtils;
import SongConstants;

export namespace DanceCommon
{
	class SongUtils
	{
	public:
		static std::optional<std::pair<std::string_view, std::string_view>> ReadDataLine(Parser& parser, std::string& linesBuffer, bool peek)
		{
			std::string line;
			std::string_view lineView;
			bool eof = true;
			linesBuffer.clear();
			while (parser.ReadOrPeekLine(peek, line))
			{
				eof = false;
				lineView = line;
				StringUtils::Trim(lineView);
				
				if (lineView == "")
					return std::nullopt;
				if (StringUtils::Contains(lineView, SongConstants::Notes))
					return std::make_pair(SongConstants::Notes, "");
				if (lineView.starts_with("//"))
					continue;

				linesBuffer.append(lineView);

				if (StringUtils::Contains(lineView, ';'))
					break;
			}

			if (eof)
				return std::nullopt;
			else
				return ParseDataLine(linesBuffer);
		}

		static std::pair<std::string_view, std::string_view> ParseDataLine(const std::string_view& lineView)
		{
			size_t dividerPos = lineView.find(':');
			if (dividerPos == std::string::npos)
				throw ParseException(std::format("Syntax error in line: {}", lineView));

			std::string_view label = StringUtils::SubstrStartEnd(lineView, 0, dividerPos);
			std::string_view data = StringUtils::SubstrStartEnd(lineView, dividerPos + 1, lineView.size());
			
			if (data.ends_with(';'))
				data = data.substr(0, data.size() - 1);

			return std::make_pair(label, data);
		}
	};
}