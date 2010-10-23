#ifndef SYNTHLOADERTHREAD_H
#define SYNTHLOADERTHREAD_H

#include <QThread>
#include "synth.h"

class SynthLoadThread : public QThread
{
    Q_OBJECT;

public:
	SynthLoadThread();

    void run();
	void startMusic();

signals:
    void ready();

private:
	bool waitForPlayCommand;
};

#endif // SYNTHLOADERTHREAD_H
