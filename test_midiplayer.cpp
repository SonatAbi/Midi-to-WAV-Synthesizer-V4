#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include "message_queue.hpp"
#include "shared_double_buffer.hpp"
#include "midiplayer.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:
  
  void initTestCase();
  void testpath();
  void testplay();
  void testpause();
  void teststop();
  void testvolume();
  void testmute();
private:
	MIDIPlayer widget;
};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QPushButton *>("play");
    QVERIFY2(w, "Could not find play button");
  }

  {
    auto w = widget.findChild<QPushButton *>("pause");
    QVERIFY2(w, "Could not find pause button");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("stop");
    QVERIFY2(w, "Could not find stop button");
  }

  {
    auto w = widget.findChild<QSlider *>("volume");
    QVERIFY2(w, "Could not find volume slider");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("mute");
    QVERIFY2(w, "Could not find mute button");
  }
}

QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

void TestMIDIPlayer::testpath()
{
	auto a = widget.findChild<QLineEdit *>("inputpath");
	a->setText("trolololololol");
	QVERIFY2(a->text() == "trolololololol", "input is wrong");
	a->setText("hohohoho");
	QVERIFY2(a->text() == "hohohoho", "input is wrong");
	a->setText("asdf");
	QVERIFY2(a->text() == "asdf", "input is wrong");
}

void TestMIDIPlayer::testplay()
{
	auto a = widget.findChild<QPushButton *>("play");
	QVERIFY2(a->isEnabled(), "play doesn't work");
}

void TestMIDIPlayer::testpause()
{
	auto a = widget.findChild<QPushButton *>("pause");
	QVERIFY2(!a->isEnabled(), "pause doesn't work");
}

void TestMIDIPlayer::teststop()
{
	auto a = widget.findChild<QPushButton *>("stop");
	QVERIFY2(!a->isEnabled(), "stop doesn't work");
}

void TestMIDIPlayer::testvolume()
{
	auto a = widget.findChild<QSlider *>("volume");
	QVERIFY2(a->value() == 50, "volume slider doesn't work");
}

void TestMIDIPlayer::testmute()
{
	auto a = widget.findChild<QPushButton *>("mute");
	QVERIFY2(a->isEnabled(), "mute doesn't work");
}
