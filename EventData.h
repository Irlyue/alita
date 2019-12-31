#ifndef ALITA_EVENT_DATA_H
#define ALITA_EVENT_DATA_H

#include "AlitaStd.h"
#include "Vector2D.h"

class IEventData {
public:
	virtual const EventType &VGetEventType() const = 0;
	virtual float VGetTimeStamp() const = 0;
	virtual void VSerialize(std::ostream &out) const = 0;
	//virtual IEventDataPtr VCopy() const = 0;
	virtual const char *GetName() const = 0;
};

class BaseEventData: public IEventData{
public:
	explicit BaseEventData(const float timeStamp=0.0f): m_timeStamp(timeStamp){}
	virtual ~BaseEventData(){}

	float VGetTimeStamp() const {return m_timeStamp;}

	virtual void VSerialize(std::ostream &out) const {}

	virtual const EventType &VGetEventType() const = 0;

protected:
	const float m_timeStamp;
};

class ObjectMoveEventData: public BaseEventData{
public:
	static const EventType s_eventType;

	explicit ObjectMoveEventData(GameObjectID id, const Vector2D &pos): m_id(id), m_pos(pos){}

	virtual const EventType &VGetEventType(void) const {return s_eventType;}

	virtual const char *GetName() const {return "ObjectMoveEventData";}

	Vector2D &getPos() {return m_pos;}

	GameObjectID GetID() const {return m_id;}

private:
	GameObjectID m_id;
	Vector2D m_pos;
};

class DestroyObjectEventData : public BaseEventData {
public:
	static const EventType s_eventType;

	explicit DestroyObjectEventData(GameObjectID id) : m_id(id) {}

	virtual const EventType &VGetEventType(void) const { return s_eventType; }

	virtual const char *GetName() const { return "DestroyObjectEventData"; }

	GameObjectID GetID() const { return m_id; }

private:
	GameObjectID m_id;
};

class CreateLevelEventData : public BaseEventData {
public:
	static const EventType s_eventType;

	explicit CreateLevelEventData(GameMapID &mapID, Vector2D &initPos): m_mapID(mapID), m_initPos(initPos) {}

	virtual const EventType &VGetEventType(void) const { return s_eventType; }

	virtual const char *GetName() const { return "CreateLevelEventData"; }

	ObjectType &getGameMapID() {return m_mapID;}
	Vector2D &getInitPos() {return m_initPos;}

private:
	GameMapID m_mapID; 
	Vector2D m_initPos;
};
#endif // !ALITA_EVENT_DATA_H
