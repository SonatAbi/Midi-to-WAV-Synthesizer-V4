#include "synthesizer.hpp"

// TODO

void Synthesizer::synthesize(message_queue *mq, shared_double_buffer *sdb)
{
	shared_double_buffer x;
	while (true) {
		while (true) {
			string play;
			string filename;
			string sr;

			string com;
			string pors;
			mq->wait_and_pop(play);
			if (play == "play") {
				mq->wait_and_pop(sr);
				int samprate = atoi(sr.c_str());
				mq->wait_and_pop(filename);
				TrackListType tracklist = readMIDIFromFile(filename);
				Track t = tracklist.front();
				DefaultInstrument ins(t, samprate);
				double n = 0;
				double samp = 0;
				double delta = 0;

				while (!ins.halted()) {
					delta = n / (double)samprate;
					samp = ins.sample(delta);
					sdb->push(samp);
					n = n + 1;
				}
			}
		}
	}
}
