#include <iostream>
#include <csignal>
#include <QCoreApplication>
#include <QTimer>
#include "qpropresenttext.h"

using namespace std;

QCoreApplication *a;
QProPresentText *p;

void signalHandler(int signum) { cerr << "Terminating\n"; a->exit(0); }

class TestClass: public QObject
{
    Q_OBJECT
public:
    QString host;
public slots:
	void makeConnection() { if(p->connectionState() == QAbstractSocket::UnconnectedState) { p->connectToProPresent(host); } }
	void onPPConnected() { qDebug() << "ProPresentText Connected"; }
	void onPPDisconnected() { qDebug() << "ProPresentText Disconnected"; }
	void onPPNextSlide(QString text) { qDebug() << "Next Slide: " << text; }
	void onPPCurrentSlide(QString text)  { qDebug() << "Current Slide: " << text; }
};


int main(int argc, char *argv[])
{
  if(argc != 2) {
    cout << "usage: test ip_address";
    return 0;
  }
  
  a = new QCoreApplication(argc, argv);
  signal(SIGINT,  signalHandler); signal(SIGTERM, signalHandler);

  p = new QProPresentText(NULL);

  TestClass t;
  t.host = argv[1];
  t.makeConnection();
  
  QObject::connect(p, SIGNAL(connected()), &t, SLOT(onPPConnected()));
  QObject::connect(p, SIGNAL(disconnected()), &t, SLOT(onPPDisconnected()));
  QObject::connect(p, SIGNAL(nextSlideChanged(QString)), &t, SLOT(onPPNextSlide(QString)));
  QObject::connect(p, SIGNAL(currentSlideChanged(QString)), &t, SLOT(onPPCurrentSlide(QString)));

  // Maintain the connection:
  QTimer timer;
  QObject::connect(&timer, SIGNAL(timeout()), &t, SLOT(makeConnection()));  
  timer.start(1000);
  
  return a->exec();
}

#include "test.moc" //http://stackoverflow.com/questions/5854626/qt-signals-and-slots-error-undefined-reference-to-vtable-for