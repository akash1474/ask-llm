#pragma once

#include <future>
#include <vector>
#include <string>
#include <mutex>
#include "LLMChatManager.h"

enum class ChatMessageType{
    User,
    Assistant
};

struct ChatMessage {
    std::string text;
    ChatMessageType type;
    size_t chatId;
    ChatMessage(){}
    ChatMessage(std::string aText,ChatMessageType aType,size_t id):text(aText),type(aType),chatId(id){}
};

class ChatWindow {
public:
    ChatWindow();
    ~ChatWindow();

    void Render();
    void SendMessage();

    static int codeBlockNumber;
    static int GetCodeBlockNumber(){
        return codeBlockNumber++;
    }
    static void ResetCodeBlockNumber(){
        codeBlockNumber=0;
    }

private:
    size_t mChatID=0;
    std::future<void> mFuture;
    LLMChatManager mChatManager;
    std::vector<ChatMessage> chatHistory;
    char userInput[1024];
    bool scrollToBottom;
    std::mutex chatMutex;
    void HandleStream();
    void MakeRequest(std::string message);
};

