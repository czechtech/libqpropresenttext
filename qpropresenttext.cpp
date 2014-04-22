#include "qpropresenttext.h"

QProPresentText::QProPresentText(QObject* parent)
	: QObject(parent)
{
	m_socket = new QTcpSocket(this);
	m_port = 0;
	m_hostName = "";
	m_password = "";
	m_buffer = "";
	m_currentSlide = "";
	m_nextSlide = "";
	m_instream = new QTextStream(m_socket);

	connect(m_socket, SIGNAL(connected()), this, SLOT(onSocketConnect()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnect()));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()), Qt::QueuedConnection);
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}


QProPresentText::~QProPresentText()
{
	m_socket->disconnectFromHost();
	delete m_instream;
	delete m_socket;
}

// public:
void
QProPresentText::connectToProPresent(const QString & hostName, quint16 port, const QString & password)
{
	m_port = port;
	m_hostName = hostName;
	m_password = password;

	m_socket->connectToHost(hostName, port);
}

// public:
void
QProPresentText::disconnectFromProPresent()
{
	m_socket->disconnectFromHost();
}

// publis slot:
void
QProPresentText::onSocketConnect()
{
	QString login = "<StageDisplayLogin>" + m_password + "</StageDisplayLogin>\r\n";
	m_socket->write(login.toUtf8());
	// start a login re-try timer?
}

//
void
QProPresentText::onSocketReadyRead()
{
	m_buffer = m_instream->readAll();
	m_buffer.remove(0, m_buffer.indexOf("<?xml")); // this sometimes needs to cut off DisplayLayouts
	//qDebug() << "Received: " << m_buffer;
	QXmlStreamReader xml(m_buffer);
	while(!xml.atEnd() && !xml.hasError()) {
		QXmlStreamReader::TokenType token = xml.readNext();
		if(token == QXmlStreamReader::StartDocument) {
			continue;
		}
		if(token == QXmlStreamReader::StartElement) {
			if(xml.name() == "StageDisplayLoginSuccess") {
				// end any login re-attempts
				emit connected();
			}
			//if(xml.name() == "DisplayLayouts") {
				// skip over this element
			//}
			if(xml.name() == "StageDisplayData") {
				continue; // don't skip over this element
			}
			if(xml.name() == "Fields") {
				continue; // don't skip over this element
			}
			if(xml.name() == "Field") {
				QXmlStreamAttributes a = xml.attributes();
				if(a.value("identifier") == "CurrentSlide") {
					QString text = xml.readElementText();
					if(m_currentSlide != text) {
						emit currentSlideChanged(text);
						m_currentSlide = text;
					}
				}
				if(a.value("identifier") == "NextSlide") {
					QString text = xml.readElementText();
					if(m_nextSlide != text) {
						emit nextSlideChanged(text);
						m_nextSlide = text;
					}
				}
				continue;
			}
		}
		xml.skipCurrentElement();
	}
	if(xml.hasError()) {
		if(!xml.error() == QXmlStreamReader::PrematureEndOfDocumentError) {
			qDebug() << "ProPresentText: XML Parse Error(" << xml.errorString() << ")";
		}
	}
	xml.clear();
}

//
void
QProPresentText::onSocketDisconnect()
{
	emit disconnected();
}

//
void
QProPresentText::onSocketError(QAbstractSocket::SocketError)
{
	qDebug() << "ProPresentText: Socket Error(" << m_socket->errorString() << ")";
}