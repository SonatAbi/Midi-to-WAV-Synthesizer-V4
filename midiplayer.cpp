#include "MIDIPlayer.hpp"

MIDIPlayer::MIDIPlayer(QWidget * parent) : QWidget(parent) {
	
	mque = new message_queue;
	sdbq = new shared_double_buffer;
	spp = true;
	stopp = false;
	t = new std::thread(&Synthesizer::synthesize, mque, sdbq);
	///////////////////LAYOUTS//////////////////////////////////////////////
	layout = new QGridLayout;
	midiInput = new QGroupBox(tr("MIDI Input"));
	audioCont = new QGroupBox(tr("Audio Controls"));
	midiInputLayout = new QGridLayout;
	audioConttLayout = new QGridLayout;
	/////////////////////MIDI INPUT////////////////////////////////////////
	input = new QLineEdit;
	input->setObjectName("inputpath");
	inpBrowse = new QPushButton("Browse");
	connect(input, SIGNAL(textChanged(QString)), this, SLOT(inputChange()));
	connect(inpBrowse, SIGNAL(clicked()), this, SLOT(getInput()));

	midiInputLayout->addWidget(input, 0, 0);
	midiInputLayout->addWidget(inpBrowse, 0, 1);
	midiInput->setLayout(midiInputLayout);
	////////////////////AUDIO CONTROL//////////////////////////////////////
	play = new QPushButton("Play");
	play->setObjectName("play");
	connect(play, SIGNAL(clicked()), this, SLOT(played()));
	pause = new QPushButton("Pause");
	pause->setObjectName("pause");
	pause->setEnabled(false);
	connect(pause, SIGNAL(clicked()), this, SLOT(paused()));
	stop = new QPushButton("Stop");
	stop->setObjectName("stop");
	stop->setEnabled(false);
	connect(stop, SIGNAL(clicked()), this, SLOT(stopped()));
	mute = new QPushButton("Mute");
	mute->setObjectName("mute");
	connect(mute, SIGNAL(clicked()), this, SLOT(muted()));
	volume = new QSlider(Qt::Horizontal, parent);
	volume->setObjectName("volume");
	volume->setRange(0, 100);
	volume->setValue(50);
	connect(volume, SIGNAL(valueChanged(int)), this, SLOT(getVol()));

	audioConttLayout->addWidget(play, 0, 0);
	audioConttLayout->addWidget(pause, 0, 1);
	audioConttLayout->addWidget(stop, 0, 2);
	audioConttLayout->addWidget(new QLabel(tr("Volume:"), this), 1, 0);
	audioConttLayout->addWidget(volume, 1, 1);
	audioConttLayout->addWidget(mute, 1, 2);
	audioCont->setLayout(audioConttLayout);
	///////////////MAIN LAYOUT////////////////////////////////////////////
	layout->addWidget(midiInput);
	layout->addWidget(audioCont);
	setLayout(layout);
	//////////////////////////////////////////////////////////////////////

	////////////////////////////AUDIO////////////////////////////////////
	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

	auto rates = info.supportedSampleRates();

	sampleRate = rates.at(0);
	for (int i = 1; i < rates.size(); ++i) {
		if (rates.at(i) < sampleRate)
			sampleRate = rates.at(i);
			cout << sampleRate << "\n";
	}

	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!info.isFormatSupported(format)) {
		qWarning()
			<< "Raw audio format not supported by backend, cannot play audio.";
	}
	else {
		audio = new QAudioOutput(format);

		connect(audio, SIGNAL(notify()), this, SLOT(handleNotify()));

		buffer_size = audio->bufferSize();
		buffer_size = buffer_size > 0 ? buffer_size : 1024;

		audio->setNotifyInterval(25);

		device = audio->start();
		handleNotify();
	}
}


void MIDIPlayer::played() {
	std::size_t period = inpstring.find_last_of(".");
	string ext = inpstring.substr(period + 1);
	if (ext == "mid" && spp == true) {
		mque->push("play");
		string sr = std::to_string(sampleRate);
		mque->push(sr);
		mque->push(inpstring);
		input->setEnabled(false);
		inpBrowse->setEnabled(false);
		play->setEnabled(false);
		pause->setEnabled(true);
		stop->setEnabled(true);
		spp = true;
	}
	else if (spp == false && stopp == true) {
		mque->push("play");
		string sr = std::to_string(sampleRate);
		mque->push(sr);
		mque->push(inpstring);
		input->setEnabled(false);
		inpBrowse->setEnabled(false);
		play->setEnabled(false);
		pause->setEnabled(true);
		stop->setEnabled(true);
		spp = true;
		stopp = false;
	}
	else if (spp == false) {
		spp = true;
	}
	else {
		QMessageBox::information(this, tr("File Error"), tr("File doesn't have the correct format"));
	}
}

void MIDIPlayer::stopped()
{
	spp = false;
	stopp = true;
	sdbq->reset();
	input->setEnabled(true);
	inpBrowse->setEnabled(true);
	play->setEnabled(true);
	pause->setEnabled(false);
	stop->setEnabled(false);
	sdbq->reset();
	
}

void MIDIPlayer::paused()
{
	play->setEnabled(true);
	spp = false;
}

void MIDIPlayer::muted()
{
	audio->setVolume(0);
	volume->setValue(0);
}

void MIDIPlayer::getVol()
{
	vol = volume->value();
	double x = (double)vol / 100;
	audio->setVolume(x);
}

void MIDIPlayer::inputChange()
{
	inpfile = input->text();
	inpstring = inpfile.toStdString();
}

void MIDIPlayer::getInput()
{
	inpfile = QFileDialog::getOpenFileName(this, QDir::currentPath());
	input->setText(inpfile);
	inpstring = inpfile.toStdString();
}

void MIDIPlayer::handleNotify()
{
	int bytesFree = audio->bytesFree();
	int bytesToWrite = bytesFree > buffer_size ? buffer_size : bytesFree;
	int samplesToWrite = bytesToWrite >> 1;
	bytesToWrite = 2 * samplesToWrite;

	buffer.reserve(bytesToWrite);

	double s = 0;

	for (int i = 0; i < samplesToWrite; ++i) {
		if (sdbq->empty()) {
			sdbq->swap();
		}
		if (spp == true) {
			sdbq->try_pop(s);
		}
		else {
			s = 0;
		}
		
		int16_t temp = static_cast<int16_t>(lround(s));
		char *byte1 = reinterpret_cast<char *>(&temp);
		char *byte2 = ++byte1;
		buffer.push_back(*byte1);
		buffer.push_back(*byte2);
	}

	if (bytesToWrite > 0) {
		device->write(buffer.data(), bytesToWrite);
	}

	buffer.clear();
}