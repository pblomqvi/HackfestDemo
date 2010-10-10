#include "SynthLoaderThread.h"

void SynthLoadThread::run()
{
    qDebug("Rendering music to buffer");
    synth_init();

    emit ready();
}
