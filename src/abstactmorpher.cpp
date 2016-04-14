#include "abstactmorpher.h"

AbstactMorpher::AbstactMorpher()
{}

int AbstactMorpher::get_vert_num() const
{
    return m_vert_num;
}

void AbstactMorpher::set_current(int v1, float w1, int v2, float w2) {
    m_v1 = v1;
    m_v2 = v2;
    m_w1 = w1;
    m_w2 = w2;
}