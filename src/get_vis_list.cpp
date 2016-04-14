#include <iostream>
#include <sapi.h>


int main()
{
	ISpVoice * pVoice = NULL;

	if (FAILED(::CoInitialize(NULL)))
		return FALSE;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if (SUCCEEDED(hr))
	{
		pVoice->SetInterest(SPFEI(SPEI_VISEME), SPFEI(SPEI_VISEME));
		hr = pVoice->Speak(L"Hello world", SPF_ASYNC, NULL);

		while (true)
		{
			SPEVENT event;
			memset(&event, 0, sizeof(SPEVENT));
			if (pVoice->GetEvents(1, &event, NULL) == S_OK)
			{
				if (event.eEventId == SPEI_VISEME) {
					std::cout <<
						"wParam:" << event.wParam << '\n' <<
						"LOWORD(wParam)" << LOWORD(event.wParam) << '\n' << // Future
						"HIWORD(wParam)" << HIWORD(event.wParam) << '\n' << // Duration
						"lParam:" << event.lParam << '\n' <<			    // Current
						"offset" << event.ullAudioStreamOffset << "\n\n";	// Offset in stream
				}
			}
		}
		pVoice->Release();
		pVoice = NULL;
	}

	return TRUE;
}

