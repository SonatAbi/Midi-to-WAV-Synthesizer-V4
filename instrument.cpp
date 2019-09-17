#include "instrument.hpp"

// TODO

DefaultInstrument::DefaultInstrument(const Track & track, int samprate)
{
	sampvalue = samprate;
	track_ = track;
	it = track_.begin();
}

bool DefaultInstrument::halted()
{
	if (halted_ == true)
		return true;
	else
		return false;
}

double DefaultInstrument::sample(double deltaT)
{
	if (it->isTempoEvent()) {
		tempo = it->asTempoEvent().getTempo();
		M = track_.getTicksPerBeat();
		it++;
		intmax_t x = it->getTickTime();
		it--;
		s_it = realTime(x, tempo, M);

		if (deltaT < s_it) {
			return 0;
		}
		else {
			it++;
			return 0;
		}
	}
	else if (it->isNoteEvent())
	{
		f_it = realTime(it->getTickTime(), tempo, M);
		it++;
		s_it = realTime(it->getTickTime(), tempo, M);
		it--;

		if (samenote == false) {
			if (it->asNoteEvent().getNoteOn()) {
				double tickt = (double)(it->getTickTime());
				Notes a = { it->asNoteEvent(), tickt, 0, true };
				nv.push_back(a);
				samenote = true;
			}
		}
		double sample = 0;
		double snote;
		if (deltaT <= s_it)
		{
			size_t k = 0;
			while (k < nv.size()) {
				double f = giveFreq(nv.at(k).note.getNoteNumber());

				int vel = nv.at(k).note.getNoteVelocity();

				int time = nv.at(k).on_time;
				double tx = time;
				double sto = (double)(tx / sampvalue);

				double env = envelope(sto);
				if (env != 0) {
					snote = 200 * vel * env * sin(2 * M_PI * f * sto);
				}
				else {
					snote = 0;
				}

				time++;
				nv.at(k).on_time = time;
				sample = sample + snote;
				k++;
			}
			return sample;
			//cout << sample << "\n";
		}
		else {
			it++;
			samenote = false;
			if (it->isEndTrackEvent()) {
				halted_ = true;
			}

			size_t k = 0;
			while (k < nv.size()) {
				double f = giveFreq(nv.at(k).note.getNoteNumber());

				int vel = nv.at(k).note.getNoteVelocity();

				int time = nv.at(k).on_time;
				double tx = time;
				double sto = (double)(tx / sampvalue);

				double env = envelope(sto);
				if (env != 0) {
					snote = 200 * vel * env * sin(2 * M_PI * f * sto);
				}
				else {
					snote = 0;
				}

				time++;
				nv.at(k).on_time = time;
				sample = sample + snote;
				k++;
			}
			return sample;
		}
	}
	else
		return 0;
}

///////////////////////////////////////////////////////////////
//Pushing and checking the active notes
/*int j = 0;
while (j < nv.size()) {
if (nv.at(j).onoff == false && nv.at(j).on_time > 0.25) {
nv.erase(nv.begin() + j);
}
j++;
}*/
/*
if (!it->asNoteEvent().getNoteOn()) {
int i = 0;
while (i < nv.size()) {
if (nv.at(i).note.getNoteNumber() == it->asNoteEvent().getNoteNumber()) {
nv.at(i).onoff = false;
}
i++;
}
}*/
///////////////////////////////////////////////////////////////
//cout << deltaT << "       " << s_it << "       " << nv.size() << "\n";
double DefaultInstrument::giveFreq(int8_t note)
{
	double freq = 0;

	double c = 261.63;
	double cp = 277.18;
	double d = 293.66;
	double dp = 311.13;
	double e = 329.63;
	double f = 349.63;
	double fp = 369.994;
	double g = 392.0;
	double gp = 415.305;
	double a = 440.0;
	double ap = 466.164;
	double b = 493.88;

	if (note == 0)  freq = c / (2 * 2 * 2 * 2 * 2);
	if (note == 1)  freq = cp / (2 * 2 * 2 * 2 * 2);
	if (note == 2)  freq = d / (2 * 2 * 2 * 2 * 2);
	if (note == 3)  freq = dp / (2 * 2 * 2 * 2 * 2);
	if (note == 4)  freq = e / (2 * 2 * 2 * 2 * 2);
	if (note == 5)  freq = f / (2 * 2 * 2 * 2 * 2);
	if (note == 6)  freq = fp / (2 * 2 * 2 * 2 * 2);
	if (note == 7)  freq = g / (2 * 2 * 2 * 2 * 2);
	if (note == 8)  freq = gp / (2 * 2 * 2 * 2 * 2);
	if (note == 9)  freq = a / (2 * 2 * 2 * 2 * 2);
	if (note == 10)  freq = ap / (2 * 2 * 2 * 2 * 2);
	if (note == 11)  freq = b / (2 * 2 * 2 * 2 * 2);

	if (note == 12)  freq = c / (2 * 2 * 2 * 2);
	if (note == 13)  freq = cp / (2 * 2 * 2 * 2);
	if (note == 14)  freq = d / (2 * 2 * 2 * 2);
	if (note == 15)  freq = dp / (2 * 2 * 2 * 2);
	if (note == 16)  freq = e / (2 * 2 * 2 * 2);
	if (note == 17)  freq = f / (2 * 2 * 2 * 2);
	if (note == 18)  freq = fp / (2 * 2 * 2 * 2);
	if (note == 19)  freq = g / (2 * 2 * 2 * 2);
	if (note == 20)  freq = gp / (2 * 2 * 2 * 2);
	if (note == 21)  freq = a / (2 * 2 * 2 * 2);
	if (note == 22)  freq = ap / (2 * 2 * 2 * 2);
	if (note == 23)  freq = b / (2 * 2 * 2 * 2);

	if (note == 24)  freq = c / (2 * 2 * 2);
	if (note == 25)  freq = cp / (2 * 2 * 2);
	if (note == 26)  freq = d / (2 * 2 * 2);
	if (note == 27)  freq = dp / (2 * 2 * 2);
	if (note == 28)  freq = e / (2 * 2 * 2);
	if (note == 29)  freq = f / (2 * 2 * 2);
	if (note == 30)  freq = fp / (2 * 2 * 2);
	if (note == 31)  freq = g / (2 * 2 * 2);
	if (note == 32)  freq = gp / (2 * 2 * 2);
	if (note == 33)  freq = a / (2 * 2 * 2);
	if (note == 34)  freq = ap / (2 * 2 * 2);
	if (note == 35)  freq = b / (2 * 2 * 2);

	if (note == 36)  freq = c / (2 * 2);
	if (note == 37)  freq = cp / (2 * 2);
	if (note == 38)  freq = d / (2 * 2);
	if (note == 39)  freq = dp / (2 * 2);
	if (note == 40)  freq = e / (2 * 2);
	if (note == 41)  freq = f / (2 * 2);
	if (note == 42)  freq = fp / (2 * 2);
	if (note == 43)  freq = g / (2 * 2);
	if (note == 44)  freq = gp / (2 * 2);
	if (note == 45)  freq = a / (2 * 2);
	if (note == 46)  freq = ap / (2 * 2);
	if (note == 47)  freq = b / (2 * 2);

	if (note == 48)  freq = c / 2;
	if (note == 49)  freq = cp / 2;
	if (note == 50)  freq = d / 2;
	if (note == 51)  freq = dp / 2;
	if (note == 52)  freq = e / 2;
	if (note == 53)  freq = f / 2;
	if (note == 54)  freq = fp / 2;
	if (note == 55)  freq = g / 2;
	if (note == 56)  freq = gp / 2;
	if (note == 57)  freq = a / 2;
	if (note == 58)  freq = ap / 2;
	if (note == 59)  freq = b / 2;

	//////////////////////////////////////////////
	if (note == 60)  freq = 261.63;
	if (note == 61)  freq = 277.18;
	if (note == 62)  freq = 293.66;
	if (note == 63)  freq = 311.13;
	if (note == 64)  freq = 329.63;
	if (note == 65)  freq = 349.63;
	if (note == 66)  freq = 369.994;
	if (note == 67)  freq = 392.0;
	if (note == 68)  freq = 415.305;
	if (note == 69)  freq = 440.0;
	if (note == 70)  freq = 466.164;
	if (note == 71)  freq = 493.88;
	//////////////////////////////////////////////
	if (note == 72)  freq = c * 2;
	if (note == 73)  freq = cp * 2;
	if (note == 74)  freq = d * 2;
	if (note == 75)  freq = dp * 2;
	if (note == 76)  freq = e * 2;
	if (note == 77)  freq = f * 2;
	if (note == 78)  freq = fp * 2;
	if (note == 79)  freq = g * 2;
	if (note == 80)  freq = gp * 2;
	if (note == 81)  freq = a * 2;
	if (note == 82)  freq = ap * 2;
	if (note == 83)  freq = b * 2;

	if (note == 84)  freq = c * 2 * 2;
	if (note == 85)  freq = cp * 2 * 2;
	if (note == 86)  freq = d * 2 * 2;
	if (note == 87)  freq = dp * 2 * 2;
	if (note == 88)  freq = e * 2 * 2;
	if (note == 89)  freq = f * 2 * 2;
	if (note == 90)  freq = fp * 2 * 2;
	if (note == 91)  freq = g * 2 * 2;
	if (note == 92)  freq = gp * 2 * 2;
	if (note == 93)  freq = a * 2 * 2;
	if (note == 94)  freq = ap * 2 * 2;
	if (note == 95)  freq = b * 2 * 2;

	if (note == 96)  freq = c * 2 * 2 * 2;
	if (note == 97)  freq = cp * 2 * 2 * 2;
	if (note == 98)  freq = d * 2 * 2 * 2;
	if (note == 99)  freq = dp * 2 * 2 * 2;
	if (note == 100)  freq = e * 2 * 2 * 2;
	if (note == 101)  freq = f * 2 * 2 * 2;
	if (note == 102)  freq = fp * 2 * 2 * 2;
	if (note == 103)  freq = g * 2 * 2 * 2;
	if (note == 104)  freq = gp * 2 * 2 * 2;
	if (note == 105)  freq = a * 2 * 2 * 2;
	if (note == 106)  freq = ap * 2 * 2 * 2;
	if (note == 107)  freq = b * 2 * 2 * 2;

	if (note == 108)  freq = c * 2 * 2 * 2 * 2;
	if (note == 109)  freq = cp * 2 * 2 * 2 * 2;
	if (note == 110)  freq = d * 2 * 2 * 2 * 2;
	if (note == 111)  freq = dp * 2 * 2 * 2 * 2;
	if (note == 112)  freq = e * 2 * 2 * 2 * 2;
	if (note == 113)  freq = f * 2 * 2 * 2 * 2;
	if (note == 114)  freq = fp * 2 * 2 * 2 * 2;
	if (note == 115)  freq = g * 2 * 2 * 2 * 2;
	if (note == 116)  freq = gp * 2 * 2 * 2 * 2;
	if (note == 117)  freq = a * 2 * 2 * 2 * 2;
	if (note == 118)  freq = ap * 2 * 2 * 2 * 2;
	if (note == 119)  freq = b * 2 * 2 * 2 * 2;

	if (note == 120)  freq = c * 2 * 2 * 2 * 2 * 2;
	if (note == 121)  freq = cp * 2 * 2 * 2 * 2 * 2;
	if (note == 122)  freq = d * 2 * 2 * 2 * 2 * 2;
	if (note == 123)  freq = dp * 2 * 2 * 2 * 2 * 2;
	if (note == 124)  freq = e * 2 * 2 * 2 * 2 * 2;
	if (note == 125)  freq = f * 2 * 2 * 2 * 2 * 2;
	if (note == 126)  freq = fp * 2 * 2 * 2 * 2 * 2;
	if (note == 127)  freq = g * 2 * 2 * 2 * 2 * 2;

	return freq;
}

double DefaultInstrument::envelope(double t)
{
	if ((0 <= t) && (t <= 0.0625)) {
		return (t / 0.0625);
	}
	else if ((0.0625 < t) && (t <= 0.1125)) {
		return (1.0625 - t);
	}
	else if ((0.1125 < t) && (t <= 0.2)) {
		return 0.8;
	}
	else if ((0.2 < t) && (t <= 0.25)) {
		return (4 - (16*t));
	}
	else
		return 0;
}

double DefaultInstrument::realTime(intmax_t eventtime, intmax_t tempo, intmax_t tickpbeat)
{
	double pay = (double)(eventtime * tempo);
	double x = pay / 1000000;
	double time = x / tickpbeat;
	return time;
}
