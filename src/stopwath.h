#ifndef STOPWATH_H
#define STOPWATH_H

#include "ofUtils.h"

class Stopwath
{
public:
    Stopwath();
    void reset();
    uint64_t get_elapsed();

private:
    uint64_t m_time_begin;
};

#endif // STOPWATH_H
