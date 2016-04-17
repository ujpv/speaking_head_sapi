#include "ofLog.h"

#include "ini_loader.h"

INILoader::INILoader()
{
}


INILoader::~INILoader()
{
}

bool INILoader::load(std::wstring file_name)
{
    ofLog(OF_LOG_NOTICE, "Loading %s...", wstring2string(file_name).c_str());
    const size_t BUFFER_SIZE = 1024;
    TCHAR buffer[BUFFER_SIZE];
    GetModuleFileName(NULL, buffer, BUFFER_SIZE);
    std::wstring s(buffer);
    size_t p = s.find_last_of(L"\\/");
    m_curr_dir = s.substr(0, p + 1);

    std::wstring full_file_name_w = m_curr_dir + file_name;
    std::string full_file_name = wstring2string(full_file_name_w);
    int count = GetPrivateProfileString(L"common", NULL, NULL, buffer, 32000, full_file_name_w.c_str());
    int i = 0;
    while (i < count)
    {
        std::string key = wstring2string(std::wstring(buffer + i));
        char buffer_val[512];
        GetPrivateProfileStringA("common", key.c_str(), NULL, buffer_val, 512, full_file_name.c_str());
        m_keys[key] = std::string(buffer_val);
        i += (key.size() + 1);
    }
    return true;
}

std::string INILoader::get_value(const std::string & key) const
{
    if (m_keys.count(key))
        return m_keys.at(key);
    else
        return "";
}

std::string INILoader::wstring2string(const std::wstring &ws)
{
    return std::string(ws.begin(), ws.end());
}
