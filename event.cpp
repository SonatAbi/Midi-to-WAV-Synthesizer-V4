#include "event.hpp"

TempoEvent::TempoEvent(intmax_t t)
{
	tempo = t;
}

intmax_t TempoEvent::getTempo() const
{
	return tempo;
}

NoteEvent::NoteEvent(bool on, int8_t notenum, int8_t notevelocity)
{
	on_off = on;
	notenumber = notenum;
	notevel = notevelocity;
}

bool NoteEvent::getNoteOn()
{
	if (on_off == true) {
		return true;
	}
	else {
		return false;
	}
}

int8_t NoteEvent::getNoteNumber() const
{
	return notenumber;
}

int8_t NoteEvent::getNoteVelocity() const
{
	return notevel;
}

MIDIEvent::MIDIEvent(TempoEvent e, intmax_t ticktime)
{
	event_.tempoEvent = e;
	tickTime = ticktime;
	eventType_ = tempo;
}

MIDIEvent::MIDIEvent(EndTrackEvent e, intmax_t ticktime)
{
	event_.endTrackEvent = e;
	tickTime = ticktime;
	eventType_ = endtrack;
}

MIDIEvent::MIDIEvent(NoteEvent e, intmax_t ticktime)
{
	event_.noteEvent = e;
	tickTime = ticktime;
	eventType_ = note;
}

bool MIDIEvent::isTempoEvent() const
{
	if (eventType_ == tempo)
		return true;
	else
		return false;
}

bool MIDIEvent::isEndTrackEvent() const
{
	if (eventType_ == endtrack)
		return true;
	else
		return false;
}

bool MIDIEvent::isNoteEvent() const
{
	if (eventType_ == note)
		return true;
	else
		return false;
}

intmax_t MIDIEvent::getTickTime() const
{
	return tickTime;
}

TempoEvent MIDIEvent::asTempoEvent() const
{
	return event_.tempoEvent;
}

NoteEvent MIDIEvent::asNoteEvent() const
{
	return event_.noteEvent;
}