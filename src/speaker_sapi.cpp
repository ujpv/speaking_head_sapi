#include "speaker_sapi.h"

#include <sapi.h>
#include <sstream>

SpeakerSAPI::SpeakerSAPI() {
    if (FAILED(::CoInitialize(NULL))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::SpeakerSAPI() FAILED(::CoInitialize(NULL))");
        m_state = SP_INVALID;
    }
}

speaking_state SpeakerSAPI::init() {
    ofLog(OF_LOG_NOTICE, "SpeakerSAPI init() started");

    if (m_state == SP_INVALID)
        return SP_INVALID;

    m_voices_names.clear();
    m_voices_ids.clear();
    cpVoice.Release();
    cpVoiceMute.Release();

    CComPtr<IEnumSpObjectTokens> cpEnum;
    HRESULT hResult = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
    if (FAILED(hResult)) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum)");
        return m_state = SP_FAULT;
    }

    ULONG ulNumTokens;
    hResult = cpEnum->GetCount(&ulNumTokens);
    if (FAILED(hResult)) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(cpEnum->GetCount(&ulNumTokens))");
        return m_state = SP_FAULT;
    }

    if (!ulNumTokens) {
        ofLog(OF_LOG_ERROR, "There are no installed voices");
        return m_state = SP_FAULT;
    }

    ISpObjectToken *pToken = NULL;

    PWSTR pszDescription = NULL;
    PWSTR pszTokenID = NULL;
    for (int i = 0; cpEnum->Next(1, &pToken, NULL) == S_OK; ++i) {
        if (FAILED(SpGetDescription(pToken, &pszDescription)) || FAILED(pToken->GetId(&pszTokenID))) {
            pToken->Release();
            pToken = NULL;
            ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(SpGetDescription(pToken, &pszDescription)) || FAILED(pToken->GetId(&pszTokenID))");
            return m_state = SP_FAULT;
        }
        m_voices_names.push_back(pszDescription);
        m_voices_ids.push_back(pszTokenID);
        pToken->Release();
        pToken = NULL;
    }

    if (FAILED(cpVoice.CoCreateInstance(CLSID_SpVoice))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(cpVoice.CoCreateInstance(CLSID_SpVoice))");
        return m_state = SP_FAULT;
    }

    if (FAILED(cpVoiceMute.CoCreateInstance(CLSID_SpVoice))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(cpVoiceMute.CoCreateInstance(CLSID_SpVoice))");
        return m_state = SP_FAULT;
    }

    hResult = cpVoice->SetInterest(SPFEI(SPEI_START_INPUT_STREAM) | SPFEI(SPEI_END_INPUT_STREAM),
        SPFEI(SPEI_START_INPUT_STREAM) | SPFEI(SPEI_END_INPUT_STREAM));
    hResult |= cpVoiceMute->SetInterest(SPFEI(SPEI_VISEME) | SPFEI(SPEI_END_INPUT_STREAM),
        SPFEI(SPEI_VISEME) | SPFEI(SPEI_END_INPUT_STREAM));

    if (FAILED(hResult)) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() SetInterest");
        return m_state = SP_FAULT;
    }

    m_state = SP_READY;

    std::wstringstream ss;
    ss << L"SpeakerSAPI init() completed. Found %d voices. Voices list:\n";
    for (int i = 0; i < m_voices_ids.size(); ++i) {
        ss << m_voices_names[i] << ':' << m_voices_ids[i] << '\n';
    }
    std::wstring voice_list(ss.str());

    ofLog(OF_LOG_NOTICE, std::string(voice_list.begin(), voice_list.end()).c_str(), m_voices_ids.size());
}

int SpeakerSAPI::get_voice_count() const
{
    return m_voices_names.size();
}

std::wstring SpeakerSAPI::get_voice_name(int i) const
{
    if (i >= 0 && i << m_voices_names.size())
        return m_voices_names[i];
    else
        return std::wstring();
}

bool SpeakerSAPI::set_voice(int i)
{
    if (i < 0 || i >= m_voices_ids.size())
        return false;

    cpToken.Release();
    if (FAILED(SpGetTokenFromId(m_voices_ids[i].c_str(), &cpToken, FALSE))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(SpGetTokenFromId(m_voices_ids[i].c_str(), &cpToken, FALSE))");
        m_state = SP_FAULT;
        return false;
    }
    if (FAILED(cpVoice->SetVoice(cpToken)) || FAILED(cpVoiceMute->SetVoice(cpToken))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::init() FAILED(SpGetTokenFromId(m_voices_ids[i].c_str(), &cpToken, FALSE))");
        m_state = SP_FAULT;
        return false;
    }
    return true;
}

void SpeakerSAPI::update()
{
    switch (m_state)
    {
    case SP_READY:
        break;

    case SP_NEW_TEXT:
        if (!m_text.length()) {
            m_state = SP_READY;
            return;
        }

        m_visemes.clear();
        m_visemes_dur.clear();
        m_visemes_pos.clear();
        m_visemes_count = 0;
        m_total_dur = 0;

        ofLog(OF_LOG_NOTICE, "Preaparing started. Text to speech %s", m_text.c_str());

        cpBuff.Release();
        if (FAILED(cpBuff.CoCreateInstance(CLSID_SpMemoryStream))) {
            ofLog(OF_LOG_ERROR, "SpeakerSAPI::update() FAILED(cpBuff.CoCreateInstance(CLSID_SpMemoryStream)");
            m_state = SP_FAULT;
            return;
        }

        if (FAILED(cpVoiceMute->SetOutput(cpBuff, FALSE))) {
            ofLog(OF_LOG_ERROR, "SpeakerSAPI::update() cpVoiceMute->SetOutput(cpBuff, FALSE))");
            m_state = SP_FAULT;
            return;
        }

        if (FAILED(cpVoiceMute->Speak(m_text.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL))) {
            ofLog(OF_LOG_ERROR, "SpeakerSAPI::update() cpVoiceMute->Speak(m_text.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL)");
            m_state = SP_FAULT;
            return;
        }

        m_state = SP_PREPARING;
        return;

    case SP_PREPARING:
        SPEVENT event;
        memset(&event, 0, sizeof(SPEVENT));
        if (cpVoiceMute->GetEvents(1, &event, NULL) == S_OK) {
            if (event.eEventId == SPEI_VISEME) {
                m_visemes.push_back(event.lParam);
                m_visemes_pos.push_back(m_visemes_pos.empty() ? 0 : m_visemes_pos.back() + m_visemes_dur.back());
                m_visemes_dur.push_back(HIWORD(event.wParam));
            }
            else if (event.eEventId == SPEI_END_INPUT_STREAM) {
                m_visemes_count = m_visemes.size();
                if (m_visemes_count > 0)
                    m_total_dur = m_visemes_pos.back() + m_visemes_dur.back();
                ofLog(OF_LOG_NOTICE, "Preaparing completed. Total visemes: %d. Total duration: %d ms", m_visemes_count, m_total_dur);
                m_state = SP_PREPARING_COMPLETED;
            }
        }
        return;

    case SP_PREPARING_COMPLETED:
        //Delete ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {std::stringstream sss;
    for (int i = 0; i < m_visemes.size(); ++i) {
        sss << "viseme:" << m_visemes[i] << "\tpos:" << m_visemes_pos[i] << "\tdur:" << m_visemes_dur[i] << '\n';
    }
    ofLog(OF_LOG_NOTICE, sss.str()); }

        if (FAILED(cpVoice->Speak(m_text.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL))) {
            ofLog(OF_LOG_ERROR, "FAILED(cpVoice->Speak(m_text.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL))");
            m_state = SP_FAULT;
            return;
        }
        m_stopwath.reset();
        m_state = SP_SPEAKING;
        return;

        /*
     //   SPEVENT event;
        memset(&event, 0, sizeof(SPEVENT));
        if (cpVoiceMute->GetEvents(1, &event, NULL) == S_OK) {
            if (event.eEventId == SPEI_START_INPUT_STREAM) {
                m_state = SP_SPEAKING;
                m_stopwath.reset();
            }
        }
        return;
        */

    case SP_SPEAKING:
        if (m_stopwath.get_elapsed() >= m_total_dur) {
            m_cur_viseme = m_pre_viseme = 0;
            m_cur_w = m_pre_w = 0.0;
            m_state = SP_SPEAKING_COMPLETED;
            return;
        }
        update_values();
        return;

    case SP_SPEAKING_COMPLETED:
        ofLog(OF_LOG_NOTICE, "Speaking completed.");
        m_state = SP_READY;
        return;

    case SP_FAULT:
        break;

    default:
        ofLog(OF_LOG_ERROR, "Incorrect state.");
        break;
    }
}

void SpeakerSAPI::speak(std::wstring s)
{
    if (m_state == SP_READY) {
        m_text = s;
        m_state = SP_NEW_TEXT;
    }
}

SpeakerSAPI::~SpeakerSAPI() {
    cpVoice.Release();
    cpVoiceMute.Release();
    cpBuff.Release();
    cpToken.Release();
    ::CoUninitialize();
}