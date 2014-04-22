#ifndef QPROPRESENTTEXT_H
#define QPROPRESENTTEXT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTextStream>
#include <QXmlStreamReader>

using namespace std;

class QProPresentText : public QObject // perhaps inherit from both QTcpSocket and QXMLStream
{
	Q_OBJECT

public:
	explicit QProPresentText(QObject* parent = NULL);
	~QProPresentText();

	void connectToProPresent(const QString & hostName, quint16 port = 45578, const QString & password = "password");
	void disconnectFromProPresent();

protected:
	QTcpSocket *m_socket;
	QTextStream *m_instream;
	QString m_currentSlide;
	QString m_nextSlide;
	QString m_password;
	quint16 m_port;
	QString m_hostName;
	QString m_buffer;
	
private:
	//

public:
	QAbstractSocket::SocketState connectionState() { return m_socket->state(); }
	QString currentSlide()        { return m_currentSlide; }
	QString nextSlide()           { return m_nextSlide; }
	QString getHostName()         { return m_hostName; }
	quint16 getPort()             { return m_port; }

public slots:
	void onSocketConnect();
	void onSocketDisconnect();
	void onSocketError(QAbstractSocket::SocketError);
	void onSocketReadyRead();

signals:
	void connected();
	void disconnected();
	void currentSlideChanged(QString text);
	void    nextSlideChanged(QString text);

};

#endif // QPROPDISPLAY_H
