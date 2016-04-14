#include <fstream>
#include <sstream>
#include <map>

#include "simplespeaker.h"

SimpleSpeaker::SimpleSpeaker()
{
    player.load("data\\speach.wav");

    std::ifstream in_file_phoneset("data\\phoneset.txt");
    std::map <string, int> phoneset;
    std::string phoneme;
    int viseme;
    while (in_file_phoneset >> phoneme >> viseme)
        phoneset[phoneme] = viseme;

    std::ifstream in_file_speach("data\\speach.txt");
    std::string s;
    std::getline(in_file_speach, s);

    std::stringstream ss;
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (*it == '(' || *it == ')')
            continue;
        ss << *it;
    }

    float start_pos, duration;
    while (ss >> phoneme >> start_pos >> duration) {
        m_visemes.push_back(phoneset[phoneme]);
        m_visemes_pos.push_back(start_pos * 1000);
        m_visemes_dur.push_back(duration * 1000);
    }

    m_total_dur = m_visemes_pos.back() + m_visemes_dur.back();
    m_visemes_count = m_visemes.size();
}

void SimpleSpeaker::update()
{
        update_values();
}

void SimpleSpeaker::speak(std::wstring s)
{
    m_stopwath.reset();
    player.play();
}
