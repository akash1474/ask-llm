#include "pch.h"
#include "ChatWindow.h"
#include "imgui.h"
#include <cstring>
#include <future>
#include <mutex>
#include <sstream>
#include <string>
#include "imgui_md.h"

ChatWindow::ChatWindow() : scrollToBottom(false) {}
ChatWindow::~ChatWindow() {}
int ChatWindow::codeBlockNumber=0;

struct my_markdown : public imgui_md 
{
    ImVec2 prevPos;
    ImFont* get_font() const override
    {
        auto& fonts=ImGui::GetIO().Fonts->Fonts;
        if (m_is_table_header) {
            return fonts[4];
        }

        if (m_is_em) { // Italic text
            return fonts[6];
        }

        if(m_is_code){
            return fonts[0];
        }

        switch (m_hlevel)
        {
        case 0:
            return m_is_strong ? fonts[4] : fonts[3];
        case 1:
            return fonts[5];
        default:
            return fonts[4];
        }
    };

    void open_url() const override
    {
        //platform dependent code
        // SDL_OpenURL(m_href.c_str());
    }

    // bool get_image(image_info& nfo) const override
    // {
    //     //use m_href to identify images
    //     nfo.texture_id = g_texture1;
    //     nfo.size = {40,20};
    //     nfo.uv0 = { 0,0 };
    //     nfo.uv1 = {1,1};
    //     nfo.col_tint = { 1,1,1,1 };
    //     nfo.col_border = { 0,0,0,0 };
    //     return true;
    // }
    
    void html_div(const std::string& dclass, bool e) override
    {
        if (dclass == "red") {
            if (e) { // Opening <div class="red">
                m_table_border = false;
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            } else { // Closing </div>
                ImGui::PopStyleColor();  // Ensure PopStyleColor is always called
                m_table_border = true;
            }
        }
    }
    void SPAN_CODE(bool e) override
    {
        if (e)
        {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            ImGui::SameLine(0.0f,0.0f);  // Prevent new line
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 165, 0, 255));  // Orange text
        }
        else 
        {

            ImGui::PopStyleColor();
            ImGui::PopFont();
        }
    }

    void BLOCK_CODE(const MD_BLOCK_CODE_DETAIL* details, bool e) override
    {
        if (e)
        {
            ImGui::Text("");
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // Green for block code
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 255)); // Dark gray background
            std::string id=std::string("##CodeBlock"+std::to_string(ChatWindow::GetCodeBlockNumber()));
            ImGui::BeginChild(id.c_str(),ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
        }
        else
        {
            ImGui::EndChild();
            ImGui::PopStyleColor(2);
            ImGui::PopFont();
        }
    }
     bool m_in_table = false;
    bool m_in_table_row = false;
    int m_column_count = 0;
    int m_current_column = 0;
    std::string m_current_table_id;
    static int table_counter;

    void BLOCK_TABLE(const MD_BLOCK_TABLE_DETAIL* detail, bool enter) override {
            if (enter) {
                // GL_INFO("BLOCK_TABLE");
                m_in_table = true;
                m_current_table_id = "MarkdownTable_" + std::to_string(table_counter++);
                
                m_column_count = detail->col_count; // Default to 3 columns if not specified
                
                ImGui::BeginTable(m_current_table_id.c_str(), m_column_count, 
                    ImGuiTableFlags_Borders | 
                    ImGuiTableFlags_RowBg);
            } else {
                // GL_INFO("BLOCK_TABLE [X]");
                ImGui::EndTable();
                m_in_table = false;
                m_column_count = 0;
            }
        }

        void BLOCK_TR(bool enter) override {
            if (enter) {
                // GL_INFO("BLOCK_TR");
                m_in_table_row = true;
                m_current_column = 0;
                ImGui::TableNextRow();
            } else {
                // GL_INFO("BLOCK_TR [X]");
                m_in_table_row = false;
            }
        }

        void BLOCK_TH(const MD_BLOCK_TD_DETAIL* detail, bool enter) override {
            if (enter) {
                // GL_INFO("BLOCK_TH");
                ImGui::TableSetColumnIndex(m_current_column++);
                // Apply header styling
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
            } else {
                // GL_INFO("BLOCK_TH [X]");
                ImGui::PopFont();
                ImGui::PopStyleColor();
            }
        }

        void BLOCK_TD(const MD_BLOCK_TD_DETAIL* detail, bool enter) override {
            if (enter) {
                // GL_INFO("BLOCK_TD");
                ImGui::TableSetColumnIndex(m_current_column++);
            }else{
                // GL_INFO("BLOCK_TD [X]");
            }
        }
};

int my_markdown::table_counter=0;


void ChatWindow::Render() {
    static bool loadTestMarkdown=false;
    ChatWindow::ResetCodeBlockNumber();
    int childNo=0;
    if(loadTestMarkdown){
        loadTestMarkdown=false;
        std::ifstream file("test.md",std::ios::binary);
        std::ostringstream data;
        data << file.rdbuf();
        file.close();
        chatHistory.emplace_back(data.str(),ChatMessageType::Assistant,mChatID++);
    }
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
    ImGui::SetNextWindowPos({0,0});
    ImGui::Begin("Chat with LLM",0,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);

    ImGui::BeginChild("ChatHistory", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), true);
    
    static my_markdown markdownRenderer;


{
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
    std::scoped_lock<std::mutex> lock(chatMutex);
    for (const auto& msg : chatHistory) {
        if (msg.type == ChatMessageType::User) {
            ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "User:");
        } else {
            ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Assistant:");
        }

        ImGui::Indent(10.0f);
        markdownRenderer.print(msg.text.c_str(), msg.text.c_str() + msg.text.size());
        ImGui::Unindent(10.0f);

        ImGui::Separator();
    }
    ImGui::PopFont();
}

    
    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }

    if(mChatManager.mIsGettingResponse)
        HandleStream();
    
    ImGui::EndChild();

    ImGui::PushItemWidth(-80);
    if (ImGui::InputText("##Input", userInput,1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
        this->SendMessage();
    }
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    if (ImGui::Button("Send",{-1,0})) {
        this->SendMessage();
    }

    ImGui::End();

}

void ChatWindow::HandleStream(){
    std::scoped_lock<std::mutex> lock(chatMutex);
    ChatMessage& msg=chatHistory.back();
    // GL_WARN(mChatManager.GetResponse());
    msg.text=mChatManager.GetResponse();
    ImGui::SetScrollY(ImGui::GetScrollMaxY());
}

void ChatWindow::MakeRequest(std::string message){
    GL_INFO("Message[WORKER-THREAD]:{}",message);
    // mChatManager.StreamResponse("ollama3.2", message);
    mChatManager.StreamResponse("codellama:7b", message);
}

void ChatWindow::SendMessage() {
    if (strlen(userInput)==0) return;

    {
        std::scoped_lock<std::mutex> lock(chatMutex);
        chatHistory.emplace_back(userInput,ChatMessageType::User,mChatID++);
    }

    {
        std::scoped_lock<std::mutex> lock(chatMutex);
        chatHistory.emplace_back("",ChatMessageType::Assistant,mChatID++);
    }

    GL_INFO("Message[MAIN-THREAD]:{}",userInput);
    mFuture=std::async(std::launch::async,&ChatWindow::MakeRequest,this,std::string(userInput));

    userInput[0]='\0';
    scrollToBottom = true;
}


//call this function to render your markdown
void markdown(const char* str, const char* str_end)
{
    static my_markdown s_printer;
    s_printer.print(str, str_end);
}

void TestMarkdown(){
    static const char* markdownText = 
    "# Heading 1\n"
    "## Heading 2\n"
    "### Heading 3\n"
    "\n"
    "**Bold Text** and *Italic Text* and ***Bold Italic Text***\n"
    "\n"
    "Inline `code` example.\n"
    "\n"
    "```\n"
    "// Code block\n"
    "int main() {\n"
    "    printf(\"Hello, Markdown in ImGui!\");\n"
    "    return 0;\n"
    "}\n"
    "```\n"
    "\n"
    "[Click here for example link](https://example.com)\n"
    "\n"
    "---\n"
    "\n"
    "- Unordered list item 1\n"
    "- Unordered list item 2\n"
    "  - Nested item 1\n"
    "  - Nested item 2\n"
    "\n"
    "1. Ordered list item 1\n"
    "2. Ordered list item 2\n"
    "\n"
    "> This is a blockquote.\n"
    "> It spans multiple lines.\n"
    "\n"
    "### Table Example\n"
    "| Name  | Age | Country  |\n"
    "|-------|-----|---------|\n"
    "| Alice |  24 | USA     |\n"
    "| Bob   |  30 | Canada  |\n"
    "\n"
    "![Example Image](https://via.placeholder.com/150)\n"
    "\n"
    // "<div class=\"red\">This text should be red if custom div classes are handled.</div>\n"
    "\n"
    "That's all!";


    ImGui::SetNextWindowSize({600,0},ImGuiCond_FirstUseEver);
    ImGui::Begin("Markdown Viewer",0,ImGuiWindowFlags_NoMove);
    markdown(markdownText, markdownText + strlen(markdownText));
    ImGui::End();
}
