#include <locale>
#include <codecvt>

#include "bot_client.h"
#include "ofLog.h"

botClient::botClient()
    : ip("")
    , port(-1) {}

void botClient::threadedFunction()
{
    std::wstring_convert <std::codecvt_utf8 <wchar_t>> cv;
    while (isThreadRunning())
    {
        if (!client.isConnected()) {
            ofLog(OF_LOG_NOTICE, "Connection to %s:%d", ip.c_str(), port);
            client.setup(ip, port);
            client.setMessageDelimiter("\r\n");
        }
        if (client.isConnected()) {
            if (!out_mess.empty()) {
                lock();
                client.send(out_mess.front());
                out_mess.pop();
                unlock();
            }

            client.send("ping\r\n");

            std::string mes = client.receive();
            if (mes.size()) {
                std::wstring s = cv.from_bytes(mes);
                ofLog(OF_LOG_NOTICE, "Recived message: %s", s.c_str());
                lock();
                in_mess.push(s);
                unlock();
            }
        }
        else {
            ofLog(OF_LOG_NOTICE, "Connection to %s:%d failed", ip.c_str(), port);
        }
        sleep(2000);
    } 
}

void botClient::setup(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
}

void botClient::send(std::string mes)
{
    lock();
    out_mess.push(mes);
    unlock();
}

bool botClient::check() const
{
    return !in_mess.empty();
}

bool botClient::is_connected()
{
    return client.isConnected();
}

std::wstring botClient::get()
{
    std::wstring res;
    if (!in_mess.empty()) {
        res = in_mess.front();
        in_mess.pop();
    }
    return res;
}

botClient::~botClient()
{
    client.close();
}
