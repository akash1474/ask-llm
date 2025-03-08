#include "FontAwesome6.h"
#include "imgui_internal.h"
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
        if(m_is_strong)
            return fonts[7];

        switch (m_hlevel)
        {
        case 0:
            return m_is_strong ? fonts[4] : fonts[3];
        case 1:
            return fonts[5];
            break;
        case 2:
            return fonts[6];
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
            // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            ImGui::SameLine(0.0f,0.0f);  // Prevent new line
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 165, 0, 255));  // Orange text
        }
        else 
        {
            ImGui::PopStyleColor();
            // ImGui::PopFont();
        }
    }

    void BLOCK_CODE(const MD_BLOCK_CODE_DETAIL* details, bool e) override
    {
        static bool m_shouldcopy=false;
        if (e)
        {
            m_last_code_block_text.clear();
            ImGui::Text("");

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            std::string idtitle=std::string("##CodeBlockDetails"+std::to_string(ChatWindow::GetCodeBlockNumber()));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f,0.0f));
            ImGui::BeginChild(idtitle.c_str(),ImVec2(-FLT_MIN, 33.0f),ImGuiChildFlags_FrameStyle);
                ImGui::Text("%s", std::string(details->lang.text,details->lang.text+details->lang.substr_offsets[1]).c_str());
                ImGui::SameLine();
                const float availWidth=ImGui::GetContentRegionAvail().x;
                ImGui::Dummy({availWidth-75.0f,10.0f});
                ImGui::SameLine();
                if(ImGui::Button(ICON_FA_COPY" Copy",{75.0f,-1}))
                    m_shouldcopy=true;

            ImGui::EndChild();

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 170, 0, 255)); // Green for block code
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 255)); // Dark gray background
            std::string id=std::string("##CodeBlock"+std::to_string(ChatWindow::GetCodeBlockNumber()));
            ImGui::BeginChild(id.c_str(),ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
            ImGui::PopStyleVar();
        }
        else
        {
            ImGui::PopStyleColor(2);
            ImGui::SetCursorPos({ImGui::GetWindowWidth()-60.0f,10.0f});
            if(m_shouldcopy){
                ImGui::SetClipboardText(m_last_code_block_text.c_str());
                m_shouldcopy=false;
            }
            ImGui::EndChild();
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
                
                m_column_count = detail->col_count;
                
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
                // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
            } else {
                // GL_INFO("BLOCK_TH [X]");
                // ImGui::PopFont();
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
void ChatWindow::RenderChatMessage(const char* text) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiStyle& style=ImGui::GetStyle();
    float parentWidth = ImGui::GetContentRegionAvail().x;
    float chatWidth = parentWidth * 0.8f;  // 80% of parent width
    float textWidth=ImGui::CalcTextSize(text).x;
    const std::string uid=std::to_string((size_t)text);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);  // Rounded corners
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 10));

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));  // Background color
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));    // Border color

    if(textWidth < chatWidth)
        chatWidth=textWidth+style.WindowPadding.x*2.0f;

    ImGui::InvisibleButton(std::string(std::string("##bnt")+uid.c_str()).c_str(),{parentWidth-chatWidth-10.0f-50.0f,10.0f});
    ImGui::SameLine();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::PushStyleColor(ImGuiCol_Button,ImGuiCol_WindowBg);
    ImGui::PushStyleColor(ImGuiCol_Text,ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    if(ImGui::Button(std::string(ICON_FA_PEN"##"+uid).c_str())){
        std::strcpy(userInput,text);
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
    ImGui::SameLine();

    ImGui::BeginChild((std::string("ChatMessage")+std::to_string((size_t)text)).c_str(), ImVec2(chatWidth, 0),ImGuiChildFlags_AutoResizeY |ImGuiChildFlags_AlwaysUseWindowPadding|ImGuiChildFlags_FrameStyle);

    ImGui::TextWrapped("%s", text);  // Render chat text

    ImGui::EndChild();

    ImGui::PopStyleColor(2);  // Restore colors
    ImGui::PopStyleVar(2);    // Restore styles
}

void ChatWindow::RenderError(const char* aErrorMessage){
    const float textWidth=ImGui::CalcTextSize(aErrorMessage).x+65.0f;
    ImGui::InvisibleButton("##errorinvisible",{ImGui::GetWindowWidth()-textWidth,10.0f});
    ImGui::SameLine();
    ImGui::BeginChild("##Error");
    ImGui::TextColored(ImVec4(0.897f,0.140f,0.367f,1.000f),ICON_FA_CIRCLE_EXCLAMATION);
    ImGui::SameLine();
    ImGui::Text("%s", aErrorMessage);
    ImGui::EndChild();
}

void ChatWindow::Render() {
    static my_markdown markdownRenderer;
    ChatWindow::ResetCodeBlockNumber();
    int childNo=0;
#ifdef GL_DEBUG
    static bool loadTestMarkdown=true;
    if(loadTestMarkdown){
        loadTestMarkdown=false;
        std::ifstream file("response.txt",std::ios::binary);
        std::ostringstream data;
        data << file.rdbuf();
        file.close();
        chatHistory.emplace_back("Give a markdown sample",ChatMessageType::User,mChatID++);
        chatHistory.emplace_back(data.str(),ChatMessageType::Assistant,mChatID++);
    }
    // ImGui::Begin("Parsed Text");
    // ImGui::TextWrapped("%s", markdownRenderer.get_parsed_text().c_str());
    // ImGui::End();
#endif


    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
    ImGui::SetNextWindowPos({0,0});
    ImGui::Begin("Chat with LLM",0,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);


    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGuiIO().MouseWheel != 0.0f) {
        GL_INFO("Scrolling");
        scrollToBottom=false;
    }


    ImGui::BeginChild("ChatHistory", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 60), true);
    


    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
        RenderChatMessage("Hello, this is a chat message!");
        RenderChatMessage("Here’s a basic example of how to create an ImGui child window styled like a chat message inside a main ImGui window. This child window will have a rounded border, contain text, and have a width of 80% of the parent window.");
        std::scoped_lock<std::mutex> lock(chatMutex);
        for (const auto& msg : chatHistory) {
            if (msg.type == ChatMessageType::User) {
                ImGui::Dummy({-1,10.0f});
                RenderChatMessage(msg.text.c_str());
            }else{
                markdownRenderer.print(msg.text.c_str(), msg.text.c_str() + msg.text.size());
            }
        }
        ImGui::PopFont();
    }
    if(this->hasError){
        RenderError("Unable to reach the server!");
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
    }


    if(mChatManager.mIsGettingResponse){
        HandleStream();
        if(scrollToBottom)
            ImGui::SetScrollY(ImGui::GetScrollMaxY());
    }
    
    ImGui::EndChild();

    ImGui::PushItemWidth(-80);
    if (ImGui::InputTextMultiline("##Input", userInput,1024,{0,-1},ImGuiInputTextFlags_EnterReturnsTrue)) {
        this->SendMessage();
    }
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    if (ImGui::Button("Send",{-1,-1})) {
        this->SendMessage();
    }

    // if(this->scrollToBottom){
    //     ImGui::SetScrollY(ImGui::GetScrollMaxY());
    //     this->scrollToBottom=false;
    // }

    ImGui::End();

}

void ChatWindow::HandleStream(){
    std::scoped_lock<std::mutex> lock(chatMutex);
    ChatMessage& msg=chatHistory.back();
    msg.text=mChatManager.GetResponse();
}

void ChatWindow::MakeRequest(std::string message){
    GL_INFO("Message[WORKER-THREAD]:{}",message);
    if(!mChatManager.StreamResponse("llama3.2", message))
        this->hasError=true;

    // mChatManager.StreamResponse("codellama:7b", message);
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
    scrollToBottom=true;
    this->hasError=false;
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
