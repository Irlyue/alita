#ifndef ALITA_EVENT_MANAGER_H
#define ALITA_EVENT_MANAGER_H

#include "EventData.h"
#include "FastDelegate.h"
#include <memory>
#include <list>
#include <map>

using EventListenerDelegate = fastdelegate::FastDelegate1<IEventDataPtr>;

const int NB_QUEUES = 2;

class EventManager{
public:
    ~EventManager() {destroy();}
	static EventManager *getInstance();
	bool addListerner(const EventListenerDelegate &eventDelegate, const EventType &type);
	bool removeListerner(const EventListenerDelegate &eventDelegate, const EventType &type);
	bool triggerEvent(const IEventDataPtr &pEvent) const;
	bool queueEvent(const IEventDataPtr &pEvent);
	bool AbortEvent(const EventType &type, bool allOfType=false);
	bool update();
	void destroy();

private:
	static EventManager *s_pEventManager;
	EventManager() = default;

	using EventListenerList = std::list<EventListenerDelegate>;
	using EventListenerMap = std::map<EventType, EventListenerList>;
	using EventQueue = std::list<IEventDataPtr>;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[NB_QUEUES];
	
	int m_activeQueue = 0;
};


#endif
