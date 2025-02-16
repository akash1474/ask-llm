#include "pch.h"
#include "ChatWindow.h"
#include "imgui.h"
#include <cstring>

// Constructor
ChatWindow::ChatWindow() : scrollToBottom(false) {}

// Destructor
ChatWindow::~ChatWindow() {}

// Render chat window
void ChatWindow::Render() {
    ImGui::Begin("Chat with LLM");

    // Display chat history
    ImGui::BeginChild("ChatHistory", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), true);
    
    for (const auto& message : chatHistory) {
        ImGui::TextWrapped("%s", message.c_str());
    }
    
    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }
    
    ImGui::EndChild();

    // User input area
    ImGui::PushItemWidth(-80);
    if (ImGui::InputText("##Input", userInput,1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
        this->SendMessage();
    }
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    if (ImGui::Button("Send")) {
        this->SendMessage();
    }

    ImGui::End();
}

// Handles sending messages
void ChatWindow::SendMessage() {
    if (strlen(userInput)==0) return;

    {
        chatHistory.push_back("You: " + std::string(userInput));
    }

    // Query LLM API
    std::string response = QueryLLM(userInput);

    {
        chatHistory.push_back("LLM: " + response);
    }

    userInput[0]='\0';
    scrollToBottom = true;
}

// Placeholder for API call (replace with actual implementation)
std::string ChatWindow::QueryLLM(const std::string& message) {
    // Simulating API response (Replace this with real API request)
    return "This is a simulated response for: " + message;
}
