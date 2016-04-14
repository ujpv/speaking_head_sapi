#include <fstream>
#include <sstream>
#include <map>

#include "simplespeaker_sapi.h"

SimpleSpeakerSapi::SimpleSpeakerSapi()
{
//    player.load("data/irina_speach.wav");
    player.load("data\\pavel_speach.wav");

    std::ifstream in_file_phoneset("data\\phoneset_sapi.txt");
    std::map <string, int> phoneset;
    std::string phoneme;
    int viseme;
    while (in_file_phoneset >> phoneme >> viseme)
        phoneset[phoneme] = viseme;

//    std::ifstream in_file_speach("data/irina_speach.txt");
    std::ifstream in_file_speach("data\\pavel_speach.txt");

    int start_pos, duration;
    while (in_file_speach >> phoneme >> start_pos >> duration) {
        m_visemes.push_back(phoneset[phoneme]);
        m_visemes_pos.push_back(start_pos);
        m_visemes_dur.push_back(duration);
    }

    m_total_dur = m_visemes_pos.back() + m_visemes_dur.back();
    m_visemes_count = m_visemes.size();
}

void SimpleSpeakerSapi::update()
{
        update_values();
}

void SimpleSpeakerSapi::speak(std::wstring s)
{
    m_stopwath.reset();
    player.play();
}
