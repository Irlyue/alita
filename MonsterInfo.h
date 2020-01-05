#ifndef ALITA_MONSTER_INFO_H
#define ALITA_MONSTER_INFO_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>

using MonsterName = std::string;
struct MonsterInfo{
	MonsterName MonName;
	int Type;
	int Pic;
	std::string Wav;
	int HP, Dc, DcMax, Df, DfMax;
	int Mf, MfMax, Nb, Eye, StepSpeed;
	int DcSpeed, DcSleep, XiHP, MaBi, ZD;
	int ActType, Ex;
	std::string Items;
	int R, G, B, A;
	int Event;
};

std::istream &operator>>(std::istream &is, MonsterInfo &info);

class MonsterDB{
public:
	bool init(std::string path);
	
	const MonsterInfo &operator[](MonsterName name);

	int getNbMonsters() const;

private:
	std::map<MonsterName, MonsterInfo> m_db;
};


#endif