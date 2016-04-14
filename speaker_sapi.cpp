#include "speaker_sapi.h"
#include <sapi.h>

SpeakerSAPI::SpeakerSAPI() {
    if (FAILED(::CoInitialize(NULL))) {
        ofLog(OF_LOG_ERROR, "SpeakerSAPI::SpeakerSAPI() FAILED(::CoInitialize(NULL))");
        m_state = SP_INVALID;
    }
}

speaking_state SpeakerSAPI::init() {
    if (m_state == SP_INVALID)
        return SP_INVALID;

    m_voices_names.clear();
    m_voices_ids.clear();

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
        return false;
    }
    cpVoice->SetVoice(cpToken);
    cpVoiceMute->SetVoice(cpToken);
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

    case SP_PREPARING:
        break;
    case SP_PREPARING_COMPLETED:
        break;
    case SP_SPEAKING:
        break;
    case SP_SPEAKING_COMPLETED:
        break;
    case SP_FAULT:
        break;
    default:
        break;
    }
}

void SpeakerSAPI::speak(std::wstring s)
{
    m_text = s;
    m_state = SP_NEW_TEXT;
}

SpeakerSAPI::~SpeakerSAPI() {
    cpVoice.Release();
    cpVoiceMute.Release();
    cpBuff.Release();
    cpToken.Release();
    ::CoUninitialize();
}