#include "abstractspeaker.h"
#include "ofUtils.h"

#include <algorithm>

AbstractSpeaker::AbstractSpeaker()
    : m_cur_viseme (0)
    , m_pre_viseme (0)
    , m_cur_w (0.0)
    , m_pre_w (0.0)
    , m_visemes_count (0)
    , m_state (SP_FAULT)
{}

void AbstractSpeaker::set_voice(std::wstring voice_name)
{
}

int AbstractSpeaker::get_current() const
{
    return m_cur_viseme;
}

int AbstractSpeaker::get_previous() const
{
    return m_pre_viseme;
}

double AbstractSpeaker::get_curren_w() const
{
    return m_cur_w;
}

double AbstractSpeaker::get_previous_w() const
{
    return m_pre_w;
}

void AbstractSpeaker::update_values()
{
    if (m_state != SP_SPEAKING)
        ofLog(OF_LOG_WARNING, "Incorrect state. Speaker::update_values() m_state != SP_SPEAKING");
        return;

    uint64_t curr_time = m_stopwath.get_elapsed();

    int curr_i = std::lower_bound(
                m_visemes_pos.begin(),
                m_visemes_pos.end(),
                curr_time)
            - m_visemes_pos.begin();
    if (curr_i > 0) 
        --curr_i;

    if (curr_time >= m_total_dur) {
        m_cur_viseme = m_pre_viseme = 0;
        m_cur_w = m_pre_w = 0.0;
        return;
    }

    m_cur_viseme = m_visemes[curr_i];
    m_pre_viseme = curr_i ? m_visemes[curr_i - 1] : 0;

    m_cur_w = ofMap(curr_time - m_visemes_pos[curr_i], 0, m_visemes_dur[curr_i], 0, 1);
    m_pre_w = 1 - m_cur_w;
// TODO
//    m_cur_w /= 2;
//    m_pre_w /= 2;
}
