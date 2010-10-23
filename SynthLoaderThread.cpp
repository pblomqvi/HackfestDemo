#include "SynthLoaderThread.h"

SynthLoadThread::SynthLoadThread(): waitForPlayCommand(true) {}

void SynthLoadThread::run()
{
    qDebug("SynthLoadThread: Rendering music to buffer");
    synth_init();

    emit ready();

	//while(waitForPlayCommand) sleep(10);

	qDebug("SynthLoadThread: wait over");
	synth_play();

}

void SynthLoadThread::startMusic()
{
	qDebug("SynthLoadThread::startMusic()");
	waitForPlayCommand = false;
}
