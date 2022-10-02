export module Song;
import <istream>;
import SongMetadata;

export namespace DanceCommon
{
	class Song
	{
	private:
		SongMetadata metadata;
		double musicLength;
	public:
		Song(std::istream& stream) :
			metadata{},
			musicLength{0.0}
		{

		}

		const SongMetadata& GetMetadata() const
		{
			return metadata;
		}
	};
}