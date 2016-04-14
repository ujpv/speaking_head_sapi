#pragma once
#include "C:\of_v0.9.3_vs_release\apps\myApps\speakingHead\src\abstractspeaker.h"
#include <sphelper.h>
class SpeakerSAPI :
    public AbstractSpeaker
{
public:
    SpeakerSAPI();
    ~SpeakerSAPI();

    virtual int get_voice_count() const override;
    virtual std::wstring get_voice_name(int i) const override;
    virtual bool set_voice(int i) override;

    // Inherited via AbstractSpeaker
    virtual void update() override;
    virtual void speak(std::wstring) override;
    virtual speaking_state init() override;

private:
    std::wstring m_text;
    std::vector<std::wstring> m_voices_names;
    std::vector<std::wstring> m_voices_ids;

    CComPtr<ISpeechMemoryStream> cpBuff;
    CComPtr<ISpVoice> cpVoice;
    CComPtr<ISpVoice> cpVoiceMute;
    CComPtr<ISpObjectToken> cpToken;
};

