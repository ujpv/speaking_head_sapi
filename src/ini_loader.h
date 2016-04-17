#pragma once
#include <string>
#include <map>
#include <Windows.h>
class INILoader
{
public:
    INILoader();
    ~INILoader();
    bool load(std::wstring file_name);
    std::string get_value(const std::string &key) const;

private:
    std::wstring                       m_curr_dir;
    std::map<std::string, std::string> m_keys;

    std::string wstring2string(const std::wstring &);
};

