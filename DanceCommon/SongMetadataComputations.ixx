export module SongMetadataComputations;

import StdCore;

export namespace DanceCommon
{
	class SongMetadataComputations
	{
	public:
		struct Event
		{
			int position;
			double value;

			bool operator==(const Event& rhs) const
			{
				return (position == rhs.position)
					&& (value == rhs.value);
			}
		};

	private:
		enum class EventType
		{
			BpmChange, Stop
		};

		bool isComputed = false;
		std::map<double, Event, std::greater<double>> bpmsByTime;
		std::map<double, Event, std::greater<double>> stopsByTime;

	public:
		void ComputeEvents(const std::map<double, double>& bpms, const std::map<double, double>& stops, double offset)
		{
			bpmsByTime.clear();
			stopsByTime.clear();
			
			auto x = bpms.begin();

			std::deque<std::pair<double, double>> bpmsLeft;
			std::deque<std::pair<double, double>> stopsLeft;
			bpmsLeft.insert(bpmsLeft.end(), bpms.begin(), bpms.end());
			stopsLeft.insert(stopsLeft.end(), stops.begin(), stops.end());
			
			double time = -offset;
			double lastBeat = -1.0;
			std::optional<EventType> lastEventType;
			std::optional<double> lastBpm;
			double lastStop = -1.0;

			while (!bpmsLeft.empty() || !stopsLeft.empty())
			{
				auto bpmEntry = !bpmsLeft.empty() 
					? std::optional(bpmsLeft.front()) 
					: std::nullopt;

				auto stopEntry = !stopsLeft.empty()
					? std::optional(stopsLeft.front())
					: std::nullopt;

				// Decide whether a bpm or stop is next
				EventType eventType;
				if (bpmsLeft.empty())
				{
					eventType = EventType::Stop;
				}
				else if (stopsLeft.empty())
				{
					eventType = EventType::BpmChange;
				}
				else
				{
					double bpmBeat = bpmEntry->first;
					double stopBeat = stopEntry->first;
					eventType = bpmBeat < stopBeat 
						? EventType::BpmChange 
						: EventType::Stop;
				}

				double beat;
				if (eventType == EventType::BpmChange)
				{
					beat = bpmEntry->first;
					bpmsLeft.pop_front();
				}
				else
				{
					beat = stopEntry->first;
					stopsLeft.pop_front();
				}

				if (lastBpm.has_value())
				{
					double beatPeriod = beat - lastBeat;
					double secondsPerBeat = 60.0 / lastBpm.value();
					time += beatPeriod * secondsPerBeat;
				}

				if (lastEventType == EventType::Stop)
				{
					time += lastStop;
				}

				if (eventType == EventType::BpmChange)
				{
					double bpmBeat = bpmEntry->first;
					double bpm = bpmEntry->second;
					int bpmPos = std::lround(bpmBeat * 48.0);
					bpmsByTime[time] = Event{ bpmPos, bpm };
					lastBpm = bpm;
				}
				else
				{
					double stopBeat = stopEntry->first;
					double stop = stopEntry->second;
					int stopPos = std::lround(stopBeat * 48.0);
					stopsByTime[time] = Event{ stopPos, stop };
					lastStop = stop;
				}

				lastBeat = beat;
				lastEventType = eventType;
			}
		}

		bool IsComputed() const
		{
			return isComputed;
		}

		auto& GetBpmsByTime() const
		{
			return bpmsByTime;
		}

		auto& GetStopsByTime() const
		{
			return stopsByTime;
		}
	};
}