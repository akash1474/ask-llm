#pragma once
#include "string"

class LLMChatManager {
private:
    std::string mBaseURL;
    std::string mBuffer;
    std::string mResponseText;

    static bool WriteCallback(const std::string_view& data, intptr_t userdata);
    bool HandleStreamData(const std::string_view& data);
    void ProcessJsonLine(const std::string& line);

public:
    bool mIsGettingResponse=false;
    LLMChatManager(const std::string& host = "localhost", int port = 11434)
        : mBaseURL("http://" + host + ":" + std::to_string(port)) {}

    std::string GetResponse(){return this->mResponseText;};
    bool CheckServer();
    void StreamResponse(const std::string& model, const std::string& prompt);
};