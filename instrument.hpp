/** \file The DefaultInstrument Module
 */
#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#define _USE_MATH_DEFINES
#include "event.hpp"
#include "track.hpp"
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

/** The default instrument. A simple fixed length note instrument using a
 * a sinusoid weighted by an envelope function.
 */
class DefaultInstrument{
public:

  /** No default constructor. A DefaultInstrument must have an associated 
   * track. 
   */
  DefaultInstrument() = delete;

  /** Cannot be copied. 
   */
  DefaultInstrument(const DefaultInstrument & ) = delete;

  /** Cannot be assigned.
   */
  DefaultInstrument & operator=(const DefaultInstrument & ) = delete;

  /** Construct a Default Instrument using the specified track.
   * \param track the track to read events from. 
   */
  DefaultInstrument(const Track & track, int value);

  /** Returns true of the DefaultInstrument has stopped processing events.
   * This can occu e.g. if all events have been consumed or a EndTrackEvent
   * has been processed.
   */
  bool halted();

  /** Return the sample after the (real) time increment. Samples the 
   * instrument based on its state after processing all events prior 
   * to this time.
   * \param deltaT the time increment in seconds.
   */
  double sample(double deltaT);

private:
	
	int sampvalue;
	double giveFreq(int8_t note);
	double envelope(double t);
	double realTime(intmax_t eventtime, intmax_t tempo, intmax_t tickpbeat);
	Track track_;
	bool halted_ = false;

	Track::ConstIteratorType it;
	double f_it;
	double s_it;
	intmax_t tempo;
	intmax_t M;

	bool samenote = false;
	struct Notes {
		NoteEvent note;
		double tick_time;
		int on_time;
		bool onoff;
	};
	std::vector<Notes> nv;
};


#endif
