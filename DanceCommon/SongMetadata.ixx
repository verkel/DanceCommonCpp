export module SongMetadata;
import <string>;
import <stdexcept>;
import Parser;

export namespace DanceCommon
{
	class SongMetadata
	{
	public:
		SongMetadata(const Parser& parser)
		{

		}

		const std::string& GetTitle()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetArtist()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetCredit()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetBanner()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetBackground()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetCdtitle()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetMusic()
		{
			throw std::runtime_error("not implemented");
		}

		const double GetOffset()
		{
			throw std::runtime_error("not implemented");
		}

		const double GetSamplestart()
		{
			throw std::runtime_error("not implemented");
		}

		const double GetSamplelength()
		{
			throw std::runtime_error("not implemented");
		}

		const std::string& GetSelectable()
		{
			throw std::runtime_error("not implemented");
		}
	};
}