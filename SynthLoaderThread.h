#ifndef SYNTHLOADERTHREAD_H
#define SYNTHLOADERTHREAD_H

#include <QThread>
#include "synth.h"

class SynthLoadThread : public QThread
{
    Q_OBJECT;

public:
    void run();

signals:
    void ready();
};

#endif // SYNTHLOADERTHREAD_H
