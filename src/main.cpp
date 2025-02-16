#include "pch.h"
#include "Log.h"
#include "Timer.h"
#include "resources/AppIcon.embed"
#include <minwindef.h>
#include <processenv.h>
#include <wingdi.h>
#include "Application.h"
#include "GLFW/glfw3.h"

#include "md4c.h"

// Callback function for handling parsed Markdown text
void process_md(const MD_CHAR* text, MD_SIZE size, void* userdata) {
    std::string* output = static_cast<std::string*>(userdata);
    output->append(text, size);
}

// Function to read a Markdown file into a string
std::string loadMarkdownFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Define a callback function matching the correct signature
int my_callback(MD_BLOCKTYPE type, void *text, void *userdata) {
    std::cout << "Parsed text: " << std::string((const char*)text) << std::endl;
    return 0; // Return an int (0 for success)
}

// Function to parse Markdown using md4c
std::string parseMarkdown(const std::string& markdown) {
    std::string parsedOutput;

    // Define an MD_PARSER struct
    MD_PARSER parser;

    // Parse the Markdown text
    if (md_parse(markdown.c_str(), markdown.size(), &parser, &parsedOutput) != 0) {
        std::cerr << "Error: Failed to parse Markdown." << std::endl;
        return "";
    }
    
    return parsedOutput;
}

void TestMD4C(){
    std::string filename = "README.md"; // Change this to your Markdown file path
    std::string markdownContent = loadMarkdownFile(filename);
    
    if (!markdownContent.empty()) {
        std::string parsedText = parseMarkdown(markdownContent);
        std::cout << "Parsed Markdown Output:\n" << parsedText << std::endl;
    }
}




//Make sure Visual Leak Detector (VLD) is installed
#ifdef DETECT_MEMORY_LEAKS_VLD
    #include <vld.h>
#endif




int main(int argc, char* argv[])
{
    OpenGL::Timer timer;
    if (!Application::Init())
        return -1;
    Application::SetupSystemSignalHandling();
    Application::SetApplicationIcon(AppIcon, IM_ARRAYSIZE(AppIcon));


    // Initialize ImGUI
    if (!Application::InitImGui())
        return -1;
    
    Application::InitFonts();

    if (argc > 1)
        Application::HandleArguments(GetCommandLineW());

    GL_CRITICAL("BootUp Time: {}ms", timer.ElapsedMillis());
    Application::SetWindowIsFocused(true);
    Application::SetFrameRate(120.0f);

    // TestMD4C();

    
    while (!glfwWindowShouldClose(Application::GetGLFWwindow())) {
        #ifdef GL_DEBUG
        Application::RenderFPSInTitleBar();
        #endif

        Application::HandleFPSCooldown();

        int targetFPS = Application::GetTargetFPS();
        if(targetFPS>1)
            Application::Draw();

        if(!Application::RunAtMaxRefreshRate())
            Application::SleepForFPS(targetFPS);
    }

    Application::Destroy();
    return 0;
}







#ifdef _WIN32
    #ifdef GL_DIST
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    std::stringstream test(lpCmdLine);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(test, segment, ' ')) seglist.push_back(segment);

    return main((int)seglist.size(), &lpCmdLine);
}
    #endif
#endif
