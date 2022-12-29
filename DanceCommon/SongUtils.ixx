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
		static optional<pair<string_view, string_view>> ReadDataLine(Parser& parser, string& linesBuffer, bool peek)
		{
			string line;
			string_view lineView;
			bool eof = true;
			linesBuffer.clear();
			while (parser.ReadOrPeekLine(peek, line))
			{
				eof = false;
				lineView = line;
				StringUtils::Trim(lineView);
				
				if (lineView == "")
					return nullopt;
				if (StringUtils::Contains(lineView, SongConstants::Notes))
					return make_pair(SongConstants::Notes, "");
				if (lineView.starts_with("//"))
					continue;

				linesBuffer.append(lineView);

				if (StringUtils::Contains(lineView, ';'))
					break;
			}

			if (eof)
				return nullopt;
			else
				return ParseDataLine(linesBuffer);
		}

		static pair<string_view, string_view> ParseDataLine(const string_view& lineView)
		{
			size_t dividerPos = lineView.find(':');
			if (dividerPos == string::npos)
				throw ParseException(format("Syntax error in line: {}", lineView));

			string_view label = StringUtils::SubstrStartEnd(lineView, 0, dividerPos);
			string_view data = StringUtils::SubstrStartEnd(lineView, dividerPos + 1, lineView.size());
			
			if (data.ends_with(';'))
				data = data.substr(0, data.size() - 1);

			return make_pair(label, data);
		}
	};
}