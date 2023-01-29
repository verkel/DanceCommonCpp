export module SongMetadata;

import StdCore;
import Parser;
import SongUtils;
import SongConstants;
import StringUtils;
import ParseException;
import SongMetadataComputations;
import NotePos;

export namespace DanceCommon
{
	class SongMetadata
	{
		SongMetadataComputations computations;

		string title;
		string subtitle;
		string artist;
		string titletranslit;
		string subtitletranslit;
		string artisttranslit;
		string genre;
		string credit;
		string banner;
		string background;
		string lyricspath;
		string cdtitle;
		string music;
		double offset = 0.0;
		double samplestart = 0.0;
		double samplelength = 0.0;
		string selectable;
		map<double, double> bpms;
		map<double, double> stops;
		string bgchanges;
		string keysounds;
		string displaybpm;

	public:
		SongMetadata(Parser& parser)
		{
			optional<pair<string_view, string_view>> dataLine;
			string linesBuffer;
			while (dataLine = SongUtils::ReadDataLine(parser, linesBuffer, true))
			{
				auto& l = dataLine->first;
				auto& c = dataLine->second;

				if (l == SongConstants::Notes)
					break;
				else
					parser.SkipPeekedLines();

				// Read a metadata entry
				if (l == SongConstants::Title) title = c;
				else if (l == SongConstants::Subtitle) subtitle = c;
				else if (l == SongConstants::Artist) artist = c;
				else if (l == SongConstants::Titletranslit) titletranslit = c;
				else if (l == SongConstants::Subtitletranslit) subtitletranslit = c;
				else if (l == SongConstants::Artisttranslit) artisttranslit = c;
				else if (l == SongConstants::Genre) genre = c;
				else if (l == SongConstants::Credit) credit = c;
				else if (l == SongConstants::Banner) banner = c;
				else if (l == SongConstants::Background) background = c;
				else if (l == SongConstants::Lyricspath) lyricspath = c;
				else if (l == SongConstants::Cdtitle) cdtitle = c;
				else if (l == SongConstants::Music) music = c;
				else if (l == SongConstants::Offset)
				{
					if (!StringUtils::TryParseDouble(c, offset))
						throw ParseException(format("Cannot parse offset: {}", c));
				}
				else if (l == SongConstants::Samplestart)
				{
					if (!StringUtils::TryParseDouble(c, samplestart))
						throw ParseException(format("Cannot parse samplestart: {}", c));
				}
				else if (l == SongConstants::Samplelength)
				{
					if (!StringUtils::TryParseDouble(c, samplelength))
						throw ParseException(format("Cannot parse samplelength: {}", c));
				}
				else if (l == SongConstants::Selectable) selectable = c;
				else if (l == SongConstants::Bpms) bpms = ParseMapFromString(c);
				else if (l == SongConstants::Stops) stops = ParseMapFromString(c);
				else if (l == SongConstants::Bgchanges) bgchanges = c;
				else if (l == SongConstants::Bgchanges2) {} // Omit ITG2 style #BGCHANGES2
				else if (l == SongConstants::Keysounds) keysounds = c;
				else if (l == SongConstants::Displaybpm) displaybpm = c;
			}
		}

		map<double, double> ParseMapFromString(string_view sv)
		{
			map<double, double> map;
			
			if (sv == "")
				return map;

			vector<string_view> tokens = StringUtils::Split(sv, ',');
			vector<string_view> subtokens;

			for (auto& token : tokens)
			{
				StringUtils::Split(token, '=', subtokens);
				if (subtokens.size() != 2)
					throw ParseException(format("Malformed subtokens: {}", token));

				double key, value;
				if (!StringUtils::TryParseDouble(subtokens[0], key))
					throw ParseException(format("Cannot parse key: {}", token));
				if (!StringUtils::TryParseDouble(subtokens[1], value))
					throw ParseException(format("Cannot parse value: {}", token));

				map[key] = value;
			}

			return map;
		}

		const string& GetTitle() const
		{
			return title;
		}

		const string& GetArtist() const
		{
			return artist;
		}

		const string& GetCredit() const
		{
			return credit;
		}

		const string& GetBanner() const
		{
			return banner;
		}

		const string& GetBackground() const
		{
			return background;
		}

		const string& GetCdtitle() const
		{
			return cdtitle;
		}

		const string& GetMusic() const
		{
			return music;
		}

		const double GetOffset() const
		{
			return offset;
		}

		const double GetSamplestart() const
		{
			return samplestart;
		}

		const double GetSamplelength() const
		{
			return samplelength;
		}

		const string& GetSelectable() const
		{
			return selectable;
		}

		auto& GetBpms() const
		{
			return bpms;
		}

		auto& GetStops() const
		{
			return stops;
		}

		auto& GetComputations()
		{
			if (!computations.IsComputed())
				computations.ComputeEvents(bpms, stops, offset);
			return computations;
		}

		double GetBpm(NotePos notePosition) const
		{
			double beats = NotePositions::ToBeatsDouble(notePosition);
		    auto it = bpms.upper_bound(beats);
		    if (it == bpms.cbegin())
		        throw invalid_argument("No bpm found");
		    return (--it)->second;
		}
	};
}