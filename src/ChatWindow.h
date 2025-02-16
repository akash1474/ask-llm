#pragma once

#include <vector>
#include <string>
#include <mutex>

class ChatWindow {
public:
    ChatWindow();
    ~ChatWindow();

    // Renders the chat window
    void Render();

    // Handles sending messages and interacting with the LLM API
    void SendMessage();

private:
    std::vector<std::string> chatHistory; // Stores messages
    char userInput[1024];                // Current input buffer
    bool scrollToBottom;                   // Scroll flag
    std::mutex chatMutex;                  // Thread safety for chat history

    // Placeholder for API call (replace with actual implementation)
    std::string QueryLLM(const std::string& message);
};
