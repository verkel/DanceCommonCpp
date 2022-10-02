export module StringUtils;
import <string_view>;
import <optional>;
import <charconv>;

export namespace DanceCommon
{
	export class StringUtils
	{
	public:
		static std::string_view SubstrStartEnd(const std::string_view& s, size_t start, size_t end)
		{
			return s.substr(start, end - start);
		}

		static bool TryParseInt(std::string_view input, int& result)
		{
			auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, 10);
			return ec == std::errc();
		}

		static bool TryParseDouble(std::string_view input, double& result)
		{
			auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, std::chars_format::general);
			return ec == std::errc();
		}

		static bool Contains(std::string_view input, char c)
		{
			return input.find(c) != std::string::npos;
		}

		static void Trim(std::string_view& s)
		{
			s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
			s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
		}
	};
}