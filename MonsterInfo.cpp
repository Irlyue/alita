#include "MonsterInfo.h"

std::istream &operator>>(std::istream &is, MonsterInfo &info){
    char tmp;
    std::getline(is, info.MonName, ',');
    is >> info.Type >> tmp;
    is >> info.Pic >> tmp;
    std::getline(is, info.Wav, ',');  // wav could be empty
    is >> info.HP >> tmp >> info.Dc >> tmp >> info.DcMax >> tmp >> info.Df >> tmp >> info.DfMax >> tmp;
    is >> info.Mf >> tmp >> info.MfMax >> tmp >> info.Nb >> tmp >> info.Eye >> tmp >> info.StepSpeed >> tmp;
    is >> info.DcSpeed >> tmp >> info.DcSleep >> tmp >> info.XiHP >> tmp >> info.MaBi >> tmp >> info.ZD >> tmp;
    is >> info.ActType >> tmp >> info.Ex >> tmp;
    std::getline(is, info.Items, ',');  // items could be empty
    is >> info.R >> tmp >> info.G >> tmp >> info.B >> tmp >> info.A >> tmp;
    is >> info.Event;

    return is;
}

int MonsterDB::getNbMonsters() const {
    return m_db.size();
}

bool MonsterDB::init(std::string path){
    std::fstream fs;
    fs.open(path, std::fstream::in);

    if(!fs){
        printf("Error loading file %s\n", path.c_str());
        return false;
    }

    std::string toDrop;

    // read filed names
    std::getline(fs, toDrop, '\n');
    MonsterInfo info;
    while(fs >> info){
        m_db[info.MonName] = info;
        std::getline(fs, toDrop, '\n');
    }

    printf("%d monsters loaded\n", getNbMonsters());
    return true;
}

const MonsterInfo &MonsterDB::operator[](MonsterName name){
    auto findIt = m_db.find(name);
    if(findIt == m_db.end()){
        printf("No such monster as %s\n", name.c_str());
        return MonsterInfo();

    }
    return findIt->second;
}