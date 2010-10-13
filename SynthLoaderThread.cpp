#include "SynthLoaderThread.h"

void SynthLoadThread::run()
{
    qDebug("SynthLoadThread: Rendering music to buffer");
    synth_init();

    emit ready();
}
