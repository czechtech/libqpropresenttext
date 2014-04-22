#include <iostream>
#include <csignal>
#include <QCoreApplication>
#include "qpropresenttext.h"

using namespace std;

QCoreApplication *a;

void signalHandler(int signum) { cerr << "Terminating\n"; a->exit(0); }

class TestClass: public QObject
{
    Q_OBJECT
public slots:
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

  TestClass t;
  QProPresentText p(NULL);
  p.connectToProPresent(argv[1], 45578, "password");
  QObject::connect(p, SIGNAL(connected()), &t, SLOT(onPPConnected()));
  QObject::connect(p, SIGNAL(disconnected()), &t, SLOT(onPPDisconnected()));
  QObject::connect(p, SIGNAL(nextSlideChanged(QString)), &t, SLOT(onPPNextSlide(QString)));
  QObject::connect(p, SIGNAL(currentSlideChanged(QString)), &t, SLOT(onPPCurrentSlide(QString)));
  
  return a->exec();
}

#include "test.moc" //http://stackoverflow.com/questions/5854626/qt-signals-and-slots-error-undefined-reference-to-vtable-for