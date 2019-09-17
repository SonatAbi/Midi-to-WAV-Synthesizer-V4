#include <QApplication>
#include <QDebug>
#include <thread>
#include "midiplayer.hpp"
#include "message_queue.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MIDIPlayer x;
	x.show();
	return app.exec();
}
