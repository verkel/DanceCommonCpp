export module SongUtils;
import <string_view>;
import <utility>;
import <format>;
import ParseException;
import StringUtils;

export namespace DanceCommon
{
	class SongUtils
	{
	public:
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