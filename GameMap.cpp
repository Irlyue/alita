#include "GameMap.h"
#include <fstream>
#include <sstream>
#include <iomanip>


GameMapGrid &GameMap::operator()(int i, int j){
    if(!isInsideMap(i, j)){
        printf("(%d, %d) is outside of the map\n", i, j);
    }
    return m_maps[i*m_cols + j];
}


bool GameMap::isInsideMap(int i, int j) const {
    return 0 <= i && i < m_rows && 0 <= j && j < m_cols;
}

bool GameMap::init(std::string path){
    std::string buffer;
    std::fstream fs(path, std::fstream::in);

    if(!fs){
        printf("Can not open `%s`!\n", path.c_str());
        return false;
    }

    fs>>buffer;
    fs.close();

    char tmp;
    std::istringstream iss(buffer);
    std::getline(iss, m_name, ',');
    iss>>m_cols>>tmp;
    iss>>m_rows>>tmp;

    m_maps.resize(m_rows * m_cols);
    for(int i = 0; i < m_rows * m_cols; i++){
        int ii = i / m_rows;
        int jj = i % m_rows;
        int t = jj * m_cols + ii;

        // fields could be empty
        // reset the stream flags
        if(iss >> m_maps[t].floorID){
            m_maps[t].floorID -= 1;
        }else{
            m_maps[t].floorID = -1;
            iss.clear(); 
        }
        iss >> tmp;

        if(iss >> m_maps[t].tileID){
            m_maps[t].tileID -= 1;
        }else{
            m_maps[t].tileID = -1;
            iss.clear();
        }
        iss >> tmp;

        std::getline(iss, m_maps[t].roleID, ',');
        std::getline(iss, m_maps[t].other, ',');
    }

    // TODO there might exist extra scripts at the end.
    // For now, just ignore them.

    return true;
}

std::ostream &operator<<(std::ostream &os, GameMap &gm){

	for(int i = 0; i < gm.getRows(); i++){
		for(int j = 0; j < gm.getCols(); j++){
			GameMapGrid &gmg = gm(i, j);
			os << gmg.floorID << '*';
			os << gmg.tileID << '*';
            if(gmg.other != ""){
                os << '(' << gmg.other << ')';
            }
            if(gmg.roleID != ""){
                os << '(' << gmg.roleID << ')';
            }
            os << ',';
		}
		os<<std::endl;
	}

	return os;
}