#include <queue>

#include "ofThread.h"
#include "ofxNetwork.h"


class botClient: public ofThread
{
public:
    botClient();
    void threadedFunction() override;
    void setup(std::string ip, int port);
    void send(std::string mes);
    bool check() const;
    bool is_connected();
    std::wstring get();
    ~botClient();

private:
    ofxTCPClient client;
    std::queue<std::wstring> in_mess;
    std::queue<std::string> out_mess;
    std::string ip;
    int port;
};

