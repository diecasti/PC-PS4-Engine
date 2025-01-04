#include "render_thread.h"

#include <thread>

#include "renderer.h"

#include "sprite.h"

void RenderThread::init()
{
	messageQueue = moodycamel::BlockingReaderWriterQueue<Message>(INITIAL_QUEUE_SIZE);
	thread = new std::thread(&RenderThread::run, this);
}

void RenderThread::enqueue(Message m)
{
	// Usamos emplace en lugar de try_emplace porque try_emplace no aloca más memoria si no hay espacio en la cola.
	// Eso estaría bien para colas en las que no queremos exceder un número total de instrucciones. 
	// Este no es nuestro caso, pues lo que nos limita son las instrucciones de Present, y no podemos saber cuántas
	// instrucciones de run puede haber entre Presents.
	messageQueue.enqueue(m);
	if (m.type_ == PRESENT) 
		++enqueuedFrames;
}

void RenderThread::close()
{
	exit = true;
	// Enviamos un mensaje vacío para que despierte la cola y pueda finalizar el thread
	messageQueue.enqueue(Message());
	thread->join();
	delete thread;
}

int RenderThread::pending() const
{
	return enqueuedFrames;
}

void RenderThread::run()
{
	while (!exit)
	{
		Message message;
		messageQueue.wait_dequeue(message);
		
		switch (message.type_) {
		case CLEAR:
			Renderer::Instance()->clear(message.clearColour);
			break;
		case RENDER_SPRITE: {
			const Sprite* sprite = message.renderData.sprite;
			sprite->draw(message.renderData.x, message.renderData.y, message.renderData.w, message.renderData.h);
			break;
		}
		case RENDER_LINE: {
			Renderer::Instance()->drawLine(message.renderLine.x1, message.renderLine.y1, message.renderLine.x2, message.renderLine.y2, message.renderLine.colour);
			break;
		}
		case RENDER_FLOOR: {
			Renderer::Instance()->drawRect(0, message.renderFloor.y, Renderer::Instance()->getWidth(), Renderer::Instance()->getHeight() - message.renderFloor.y, message.renderFloor.colour);
			break;
		}
		case RENDER_CIRCLE: {
			Renderer::Instance()->drawCircle(message.renderCircle.x, message.renderCircle.y, message.renderCircle.r, message.renderCircle.colour);
			break;
		}
		case PRESENT:
			Renderer::Instance()->present();
			--enqueuedFrames;
			break;
		}

	}
}