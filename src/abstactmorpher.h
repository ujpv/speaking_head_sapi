#ifndef ABSTACTMORPHER_H
#define ABSTACTMORPHER_H

#include <string>

#include "ofMain.h"
//#include "ofxAssimpModelLoader.h"

class AbstactMorpher
{
public:
    AbstactMorpher();
    virtual void update() = 0;
    virtual void draw() = 0;
    int get_vert_num() const;
    void set_current(int v1, float w1, int v2, float w2);

    virtual ~AbstactMorpher() {}

protected:
    double  m_w1, m_w2;
    int     m_v1, m_v2;
	int	    m_vert_num;
};

#endif // ABSTACTMORPHER_H
