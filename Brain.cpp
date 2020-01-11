#include "Brain.h"
#include <cstdlib>

int MonsterBrain::nextState(int state){
    int motion = state / 8;
    int orientation = state % 8;

    if(motion == MonsterState::NORMAL){
        int x = rand() % 3 - 1;
        orientation = (orientation + x + 8) % 8;
    }

    return motion * 8 + orientation;
}