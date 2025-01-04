#pragma once
#ifndef RENDERERTHREAD_H
#define RENDERERTHREAD_H

#include <thread>

#include "readerwriterqueue.h"
#include "messages.h"

#define INITIAL_QUEUE_SIZE 100


class RenderThread
{
public:
	void init();
	void enqueue(Message m);
	void close();
	int pending() const;

private:
	moodycamel::BlockingReaderWriterQueue<Message> messageQueue;
	std::thread* thread;

	volatile bool exit = false;
	volatile int enqueuedFrames;

	void run();
};
#endif // RENDERERTHREAD_H