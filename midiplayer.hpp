#ifndef MIDIPlayer_HPP
#define MIDIPlayer_HPP

#include <thread>
#include <QWidget>
#include <QState>
#include <QObject>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QSlider>
#include <QMessageBox>
#include <QAudioOutput>
#include <fstream>
#include "midi.hpp"
#include "message_queue.hpp"
#include "shared_double_buffer.hpp"
#include "synthesizer.hpp"


using namespace std;

class MIDIPlayer : public QWidget {
	Q_OBJECT
public:
	MIDIPlayer(QWidget * parent = nullptr);

private slots:
	void played();
	void stopped();
	void paused();
	void muted();
	void getVol();
	void inputChange();
	void getInput();
	void handleNotify();
private:
	std::thread *t;
	message_queue *mque;
	shared_double_buffer *sdbq;
	QGridLayout *layout;
	QGroupBox * midiInput;
	QGroupBox * audioCont;
	QGridLayout * midiInputLayout;
	QGridLayout * audioConttLayout;
	QLineEdit *input;
	QPushButton *inpBrowse;
	QPushButton *play;
	QPushButton *stop;
	QPushButton *pause;
	QPushButton *mute;
	QSlider *volume;
	
	QString inpfile;
	std::string inpstring;

	std::size_t buffer_size;
	std::vector<char> buffer;

	QAudioOutput *audio;
	QIODevice *device;
	int vol;
	int sampleRate;
	bool spp;
	bool stopp;
};


#endif