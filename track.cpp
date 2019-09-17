#include "track.hpp"

Track::Track(intmax_t ticks_per_beat)
{
	ticksperbeat = ticks_per_beat;
}

intmax_t Track::getTicksPerBeat() const
{
	return ticksperbeat;
}

std::list<MIDIEvent>::const_iterator Track::begin() const
{
	return trackevents.begin();
}

std::list<MIDIEvent>::const_iterator Track::end() const
{
	return trackevents.end();
}

void Track::addEndEvent(intmax_t tick_time)
{
	MIDIEvent midiend(EndTrackEvent(), tick_time);
	trackevents.push_back(midiend);
}

void Track::addTempoEvent(intmax_t tempo, intmax_t tick_time)
{
	MIDIEvent miditempo(TempoEvent(tempo), tick_time);
	trackevents.push_back(miditempo);
}

void Track::addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time)
{
	bool on;
	if (code == 8) {
		on = false;
	}
	else if (code == 9 && data2 != 0) {
		on = true;
	}
	else on = false;

	MIDIEvent midinote(NoteEvent(on, data1, data2), tick_time);
	trackevents.push_back(midinote);
}
