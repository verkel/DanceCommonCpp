export module StringUtils;
import <string_view>;
import <optional>;
import <charconv>;

export namespace DanceCommon
{
	void SubstrStartEnd(std::string_view& s, size_t start, size_t end)
	{
		s = s.substr(start, end - start);
	}

	bool TryParseInt(std::string_view input, int& result)
	{
		auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, 10);
		return ec != std::errc();
	}

	bool TryParseDouble(std::string_view input, double& result)
	{
		auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, std::chars_format::general);
		return ec != std::errc();
	}

	bool Contains(std::string_view input, char c)
	{
		return input.find(c) != std::string::npos;
	}
}