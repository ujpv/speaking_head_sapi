#ifndef ABSTRACTSPEAKER_H
#define ABSTRACTSPEAKER_H

#include "stopwath.h"

#include <vector>

enum speaking_state {SP_READY, SP_NEW_TEXT, SP_PREPARING, SP_PREPARING_COMPLETED, SP_SPEAKING, SP_SPEAKING_COMPLETED, SP_FAULT, SP_INVALID};

class AbstractSpeaker
{
public:
    AbstractSpeaker();
    virtual ~AbstractSpeaker() {}

    virtual void update() = 0;
    virtual void speak(std::wstring) = 0;
    virtual void set_voice(std::wstring);
    virtual speaking_state init() = 0;
    virtual int get_voice_count() const = 0;
    virtual std::wstring get_voice_name(int i) const = 0;
    virtual bool set_voice(int i) = 0;

    int     get_current() const;
    int     get_previous() const;
    double  get_curren_w() const;
    double  get_previous_w() const;

protected:
    int     m_cur_viseme;
    int     m_pre_viseme;
    double  m_cur_w;
    double  m_pre_w;
    double  m_tot_w;

    int m_visemes_count;
    int m_total_dur;

    std::vector<int> m_visemes;
    std::vector<int> m_visemes_pos;
    std::vector<int> m_visemes_dur;

    speaking_state m_state;

    Stopwath m_stopwath;

    void update_values();
};

#endif // ABSTRACTSPEAKER_H
