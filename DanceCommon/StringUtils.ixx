export module StringUtils;

import StdCore;

export namespace DanceCommon
{
	export class StringUtils
	{
	public:
		static string_view SubstrStartEnd(const string_view& s, size_t start, size_t end)
		{
			return s.substr(start, end - start);
		}

		static bool TryParseInt(string_view input, int& result)
		{
			auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, 10);
			return ec == std::errc();
		}

		static bool TryParseDouble(string_view input, double& result)
		{
			auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, std::chars_format::general);
			return ec == std::errc();
		}

		static bool Contains(string_view input, char c)
		{
			return input.find(c) != string::npos;
		}

		static bool Contains(string_view input, const string& str)
		{
			return input.find(str) != string::npos;
		}

		static void Trim(string_view& s)
		{
			s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
			s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
		}

		static void Split(string_view sv, char delim, vector<string_view>& tokens)
		{
			tokens.clear();
			size_t start = 0;
			size_t end = sv.find(delim);
			while (end != string::npos)
			{
				tokens.push_back(sv.substr(start, end - start));
				start = end + 1;
				end = sv.find(delim, start);
			}

			tokens.push_back(sv.substr(start));
		}

		static vector<string_view> Split(string_view sv, char delim)
		{
			vector<string_view> tokens;
			Split(sv, delim, tokens);
			return tokens;
		}

		static string_view ToString(bool value)
		{
			return value
				? "True"
				: "False";
		}
	};
}