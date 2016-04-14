#include "ofxObjLoader.h"

#include "cpu_morpher.h"


CPU_Morpher::CPU_Morpher(string path)
{
    ofLog(OF_LOG_NOTICE, "Loading SVP_00.obj...");

    //static mesh
    ofxObjLoader::load(path + "SVP_00.obj", m_static_mesh);

    //rescale texCoord
    for (int i = 0; i < m_static_mesh.getNumVertices(); ++i) {
        m_static_mesh.setTexCoord(i, ofVec2f(1024, 1024) - m_static_mesh.getTexCoord(i) * 1024);
    }

    m_vert_num = m_static_mesh.getNumVertices();

    //blenshapes
    for (int i = 0; i <= VISEM_COUNT; ++i) {
        std::string file_name = "SVP_" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i)) + ".obj";
        ofLog(OF_LOG_NOTICE, "Loading %s...", file_name.c_str());

        ofxObjLoader::load(path + file_name, m_curr_mesh);

        m_diffs[i].resize(m_vert_num);
        for (int j = 0; j < m_vert_num; ++j) {
            m_diffs[i][j] = m_curr_mesh.getVertex(j) - m_static_mesh.getVertex(j);
        }
    }

    m_curr_mesh = m_static_mesh;
}

void CPU_Morpher::update()
{
    for (int i = 0; i < m_vert_num; ++i) {
        m_curr_mesh.setVertex(i, m_static_mesh.getVertex(i) + m_diffs[m_v1][i] * m_w1 + m_diffs[m_v2][i] * m_w2);
    }
}

void CPU_Morpher::draw()
{
    m_curr_mesh.draw();
}
