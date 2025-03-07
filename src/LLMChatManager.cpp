#include "Log.h"
#include "pch.h"
#include <cpr/cpr.h>
#include "LLMChatManager.h"
#include "nlohmann/json.hpp"

void LLMChatManager::ProcessJsonLine(const std::string& line) {
    if (line.empty()) return;
    
    try {
        nlohmann::json response_json = nlohmann::json::parse(line);

        if (response_json.contains("message") && response_json["message"].contains("content")) {
            mResponseText += response_json["message"]["content"].get<std::string>();
        }
        
        if (response_json.contains("done") && response_json["done"].get<bool>()) {
            mIsGettingResponse = false;
            std::ofstream file("./response.txt");
            file << mResponseText;
            file.close();
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON chunk: " << e.what() << std::endl;
    }
}

bool LLMChatManager::WriteCallback(const std::string_view& data, intptr_t userdata) {
    auto* client = reinterpret_cast<LLMChatManager*>(userdata);
    // GL_INFO("Response:{}",data);
    return client->HandleStreamData(data);
}

// Handle incoming stream data
bool LLMChatManager::HandleStreamData(const std::string_view& data) {
    // GL_INFO("Response:{}",mResponseText);
    mIsGettingResponse=true;
    mBuffer.append(data.data(), data.size());
    
    std::size_t pos = 0;
    std::size_t prev = 0;
    
    while ((pos = mBuffer.find('\n', prev)) != std::string::npos) {
        std::string line = mBuffer.substr(prev, pos - prev);
        ProcessJsonLine(line);
        prev = pos + 1;
    }
    
    if (prev < mBuffer.size()) {
        mBuffer = mBuffer.substr(prev);
    } else {
        mBuffer.clear();
    }
    
    return true;
}


bool LLMChatManager::CheckServer() {
    try {
        auto r = cpr::Get(cpr::Url{mBaseURL + "/api/version"});
        return r.status_code == 200;
    } catch (const std::exception& e) {
        std::cerr << "Error checking server: " << e.what() << std::endl;
        return false;
    }
}

bool LLMChatManager::StreamResponse(const std::string& model, const std::string& prompt) {
    this->mResponseText.clear();
    try {
        nlohmann::json payload = {
            {"model",model},
            {"messages", {
                {{"role", "user"}, {"content", prompt}}
            }},
            {"stream",true}
        };

        cpr::Session session;
        
        session.SetUrl(cpr::Url{mBaseURL + "/api/chat"});
        session.SetHeader({{"Content-Type", "application/json"}});
        session.SetBody(payload.dump());
        
        mBuffer.clear();
        session.SetWriteCallback(
            cpr::WriteCallback{WriteCallback, reinterpret_cast<intptr_t>(this)}
        );

        // Make the request
        auto response = session.Post();

        if (response.status_code != 200) {
            GL_CRITICAL("Error: Server returned status {}",response.status_code);
            if (!response.text.empty()) {
                GL_CRITICAL("Response: {}",response.text);
            }
            return false;
        }

        GL_INFO("Response complete!");
        return true;

    } catch (const std::exception& e) {
        GL_CRITICAL("Error making request: {}",e.what());
        return true;
    }
}