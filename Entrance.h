#ifndef ALITA_ENTRANCE_H
#define ALITA_ENTRANCE_H

#include "GameObject.h"
#include <vector>

class Entrance: public GameObject{
public:
	const static ObjectType s_type;
	static GameObject *creator();

	virtual void draw();
	virtual bool init(const XMLElement *doc);

	void setEnterPos(const Vector2D &pos) {m_enterPos = pos;}
	void setToWhere(const std::string &path) {m_toWhere = path;}
	void addGrid(const Vector2D &grid);

	virtual const ObjectType &getObjectType() const {return s_type;}

	void onPlayerMove(IEventDataPtr pEvent);

	bool isUsed() const {return m_used;}

private:
	std::string m_toWhere;
	Vector2D m_enterPos;
	bool m_used = false;

	// recording the grids (j, i) occupied by the entrance
	std::vector<Vector2D> m_areas;

	bool isPlayerInsideEntrance(const Vector2D &playerPos);
	
};


#endif // !ALITA_ENTRANCE_H
