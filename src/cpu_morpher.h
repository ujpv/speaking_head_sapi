#ifndef CPU_MORPHER_H
#define CPU_MORPHER_H

#include <string>
#include <vector>

#include "abstactmorpher.h"

class CPU_Morpher : public AbstactMorpher
{
public:
    CPU_Morpher(std::string path);

    void update();
    void draw();

private:
    static const int VISEM_COUNT = 22;

    ofMesh   m_static_mesh, m_curr_mesh;

    std::array <std::vector <ofVec3f>, VISEM_COUNT + 1> m_diffs;
};

#endif // CPU_MORPHER_H
