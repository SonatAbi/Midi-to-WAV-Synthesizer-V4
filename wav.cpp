#include "wav.hpp"

// TODO

bool write_wav(Signal & sound, const std::string & filename)
{
	int32_t  SampleRate = 8000;       // samples per second, 44100
	int32_t  NUMSAMPLES = (int32_t)(sound.size());  /**************************************************/
	int32_t   ChunkID = 0x46464952;        // "RIFF"
	int32_t  Subchunk1Size = 16;    // 16
	int16_t  NumChannels = 1;      // Mono = 1
	int16_t  BitsPerSample = 16;    // 8 bits = 8, 16 bits = 16 <=
	int32_t  Subchunk2Size = NUMSAMPLES * NumChannels * BitsPerSample / 8;    // NumSamples * NumChannels * BitsPerSample/8
	int32_t  ChunkSize = 20 + Subchunk1Size + Subchunk2Size;         // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	int32_t   Format = 0x45564157;         // "WAVE"
	int32_t   Subchunk1ID = 0x20746d66;   // "fmt "
	int16_t  AudioFormat = 1;      // 1
	int32_t  ByteRate = SampleRate * NumChannels * BitsPerSample / 8;         // SampleRate * NumChannels * BitsPerSample/8
	int16_t  BlockAlign = NumChannels * BitsPerSample / 8;       // NumChannels * BitsPerSample/8
	int32_t   Subchunk2ID = 0x61746164;   // "data"

	std::string output = filename + "-0.wav";
	std::ofstream out(output, std::ios::binary);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkID), sizeof ChunkID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkSize), sizeof ChunkSize);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Format), sizeof Format);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1ID), sizeof Subchunk1ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1Size), sizeof Subchunk1Size);
	out.write(reinterpret_cast<std::fstream::char_type*>(&AudioFormat), sizeof AudioFormat);
	out.write(reinterpret_cast<std::fstream::char_type*>(&NumChannels), sizeof NumChannels);
	out.write(reinterpret_cast<std::fstream::char_type*>(&SampleRate), sizeof SampleRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ByteRate), sizeof ByteRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BlockAlign), sizeof BlockAlign);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BitsPerSample), sizeof BitsPerSample);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2ID), sizeof Subchunk2ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2Size), sizeof Subchunk2Size);

	size_t i = 0;
	while (i < sound.size()) {
		int16_t s;
		if (sound.operator[](i) > 0) {
			s = (int16_t)((sound.operator[](i)) + 0.5);
		}
		else
			s = (int16_t)((sound.operator[](i)) - 0.5);
		//cout << s << "\n";
		out.write(reinterpret_cast<std::fstream::char_type*>(&s), sizeof s);
		i++;
	}
	return true;
}
