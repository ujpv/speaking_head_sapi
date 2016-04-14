#include "stopwath.h"

Stopwath::Stopwath()
{
    m_time_begin = ofGetElapsedTimeMillis();
}

void Stopwath::reset()
{
    m_time_begin = ofGetElapsedTimeMillis();
}

uint64_t Stopwath::get_elapsed()
{
    return ofGetElapsedTimeMillis() - m_time_begin;
}
