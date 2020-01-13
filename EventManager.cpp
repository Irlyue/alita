#include "EventManager.h"

EventManager *EventManager::s_pEventManager = nullptr;

EventManager *EventManager::getInstance(){
    if(!s_pEventManager){
        s_pEventManager = GCC_NEW EventManager;
    }
    return s_pEventManager;
}

bool EventManager::addListerner(const EventListenerDelegate &eventDelegate, const EventType &type){
    EventListenerList &eventListenerList = m_eventListeners[type];

    // avoid adding duplicate listener
    for(auto it = eventListenerList.begin(); it != eventListenerList.end(); it++){
        if(eventDelegate == (*it)){
            printf("Attempting to double-register a delegate\n");
            return false;
        }
    }

    eventListenerList.push_back(eventDelegate);
    return true;
}

bool EventManager::removeListerner(const EventListenerDelegate &eventDelegate, const EventType &type){
    bool success = false;
    auto it = m_eventListeners.find(type);
    if(it != m_eventListeners.end()){
        EventListenerList &listeners = it->second;
        for(auto it = listeners.begin(); it != listeners.end(); it++){
            if(eventDelegate == (*it)){
                listeners.erase(it);
                success = true;
                break;
            }
        }
    }
    return success;
}

bool EventManager::triggerEvent(const IEventDataPtr &pEvent) const{
    bool processed = true;

    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if(findIt != m_eventListeners.end()){
        const EventListenerList &listeners = findIt->second;
        for(auto it = listeners.begin(); it != listeners.end(); it++){
            EventListenerDelegate listener = (*it);
            listener(pEvent);
            processed = true;
        }
    }

    return processed;
}

bool EventManager::queueEvent(const IEventDataPtr &pEvent) {
    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if(findIt != m_eventListeners.end()){
        m_queues[m_activeQueue].push_back(pEvent);
        return true;
    }else{
        return false;
    }
}

bool EventManager::update(){
    EventQueue &eq = m_queues[m_activeQueue];
    m_activeQueue = (m_activeQueue + 1) % NB_QUEUES;
    m_queues[m_activeQueue].clear();

    while(!eq.empty()){
        IEventDataPtr pEvent = eq.front();
        eq.pop_front();

        const EventType &eventType = pEvent->VGetEventType();
        auto findIt = m_eventListeners.find(eventType);
        if(findIt != m_eventListeners.end()){
            EventListenerList listeners = findIt->second;
            for(auto it = listeners.begin(); it != listeners.end(); it++){
                auto listener = *it;
                listener(pEvent);
            }
        }
    }

    return true;
}

void EventManager::destroy(){
    printf("EventManager destroyed!\n");
}