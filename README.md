libqpropresenttext
==================

Qt Library for ProPresenter's Textstream


Installation
------------

Download the source and compile it.

    wget https://github.com/czechtech/libqpropresenttext/archive/master.zip
    gunzip master.zip
    cd libqpropresenttext-master
    qmake && make && su -c 'make install'

Usage
-----

Include QProPresentText header file:

    #include <qpropresenttext.h>

Declare and define slots for the signals.

    void onPPConnected() { ... }
    void onPPDisconnected() { ... }
    void onPPNextSlide(QString text) { ... }
    void onPPCurrentSlide(QString text)  { ... }

Instantiate a QProPresentText object.

    QProPresentText m_ppt;

Connect the signal/slot in the constructor of your main class.

    connect(&m_ppt, SIGNAL(connected()), this, SLOT(onPPConnected()));
    connect(&m_ppt, SIGNAL(disconnected()), this, SLOT(onPPDisconnected()));
    connect(&m_ppt, SIGNAL(nextSlide(QString)), this, SLOT(onPPNextSlide(QString)));
    connect(&m_ppt, SIGNAL(currentSlide(QString)), this, SLOT(onPPCurrentSlide(QString)));

Instantiate a QProPresentText object.

    m_ppt.connectToProPresent("localhost", 45578, "password");

Now, when the connection to ProPresenter is established the connected() signal is added to the event queue, and from then on, whenever the current slide or next slide changes in ProPresent, the corresponding signal is added to the event queue.


Example
-------

Included in the "test" directory is an example that connects to ProPresenter on its default port with the default password.

To see the demo:

    cd libqpropresenttext-master/test
    qmake && make

Start the demo with the ip address of the ProPresenter computer on the commandline.

    ./test 169.254.185.203


Notes
-----

At present, you need to know the IP address of the ProPresenter computer. Because ProPresenter's service is announced using mDNS, perhaps in the future QPrePresentText can query for this.

Only the text of the current slide and next slide is provided. QProPresentText could easily be extended to access the DisplayLayouts, 


To Do
-----

- Explore using mDNS to find computers with ProPresenter,
- Attempt a couple logins if unsuccessful,
- Providing a mechanism for reestablishing the connection if it's lost, and
- A couple "TODO" notes within the code.

The code functions well, but it could use some enhancements and bullet-proofing.

Uses
----

- [ProPresentTextDisplay]()

If you use this in a project drop, a line to have your project linked here.
