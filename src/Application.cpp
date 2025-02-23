#include "pch.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "Timer.h"
#include "Log.h"
#include "Application.h"
#include "MultiThreading.h"
#include <csignal>
#include <shellapi.h>
#include <stdio.h>
#include <winuser.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include <dwmapi.h>
#include <windef.h>
#include <wingdi.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_img.h"
#include "Utils.h"


#include "resources/FontAwesomeRegular.embed"
#include "resources/FontAwesomeSolid.embed"
#include "resources/JetBrainsMonoNLRegular.embed"
#include "resources/JetBrainsMonoNLItalic.embed"


#include "imgui_markdown.h"
#include "Shellapi.h"

#undef min
#undef max

void LinkCallback( ImGui::MarkdownLinkCallbackData data_ );
inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ );

static ImFont* H1 = NULL;
static ImFont* H2 = NULL;
static ImFont* H3 = NULL;

static ImGui::MarkdownConfig mdConfig; 


void LinkCallback( ImGui::MarkdownLinkCallbackData data_ )
{
    std::string url( data_.link, data_.linkLength );
    if( !data_.isImage )
    {
        ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
    }
}

inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ )
{
    // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
    ImTextureID image = ImGui::GetIO().Fonts->TexID;
    // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
    ImGui::MarkdownImageData imageData;
    imageData.isValid =         true;
    imageData.useLinkCallback = false;
    imageData.user_texture_id = image;
    imageData.size =            ImVec2( 40.0f, 20.0f );

    // For image resize when available size.x > image width, add
    ImVec2 const contentSize = ImGui::GetContentRegionAvail();
    if( imageData.size.x > contentSize.x )
    {
        float const ratio = imageData.size.y/imageData.size.x;
        imageData.size.x = contentSize.x;
        imageData.size.y = contentSize.x*ratio;
    }

    return imageData;
}

void LoadFonts( float fontSize_ = 12.0f )
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    // Base font
    // io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf",font_size+3,&font_config);
    io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeui.ttf", fontSize_ );
    // Bold headings H2 and H3
    H2 = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuib.ttf", fontSize_ );
    H3 = mdConfig.headingFormats[ 1 ].font;
    // bold heading H1
    float fontSizeH1 = fontSize_ * 1.1f;
    H1 = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuib.ttf", fontSizeH1 );
}

void ExampleMarkdownFormatCallback( const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_ )
{
    // Call the default first so any settings can be overwritten by our implementation.
    // Alternatively could be called or not called in a switch statement on a case by case basis.
    // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
    ImGui::defaultMarkdownFormatCallback( markdownFormatInfo_, start_ );        
       
    switch( markdownFormatInfo_.type )
    {
    // example: change the colour of heading level 2
    case ImGui::MarkdownFormatType::HEADING:
    {
        if( markdownFormatInfo_.level == 2 )
        {
            if( start_ )
            {
                ImGui::PushStyleColor( ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] );
            }
            else
            {
                ImGui::PopStyleColor();
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

void Markdown( const std::string& markdown_ )
{
    // You can make your own Markdown function with your prefered string container and markdown config.
    // > C++14 can use ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { H1, true }, { H2, true }, { H3, false } }, NULL };
    mdConfig.linkCallback =         LinkCallback;
    mdConfig.tooltipCallback =      NULL;
    mdConfig.imageCallback =        ImageCallback;
    mdConfig.linkIcon =             ICON_FA_LINK;
    mdConfig.headingFormats[0] =    { H1, true };
    mdConfig.headingFormats[1] =    { H2, true };
    mdConfig.headingFormats[2] =    { H3, false };
    mdConfig.userData =             NULL;
    mdConfig.formatCallback =       ExampleMarkdownFormatCallback;
    ImGui::Markdown( markdown_.c_str(), markdown_.length(), mdConfig );
}

void MarkdownExample()
{
    const std::string markdownText = u8R"(
# 🚀 AskLLM - Context-Aware AI Q&A

AskLLM is a powerful tool that allows users to ask questions to an LLM (Large Language Model) **without manually providing context**. The system automatically retrieves relevant project data using **Retrieval-Augmented Generation (RAG) and other techniques**, ensuring that the LLM understands your queries in the right context.

## ✨ Features

- **Context-Aware Questioning** - No need to explain project details; the system handles context retrieval.
- **Seamless Integration** - Works with any project structure.
- **Fast & Accurate Responses** - Uses RAG to fetch relevant data before querying the LLM.
- **C++ GUI with OpenGL** - A sleek and efficient interface for seamless interaction.

## 🔧 Installation

### Prerequisites

- **C++17 or later**
- **OpenGL 3.3+**
- **CMake** (for building the project)
- **Python 3.x** (for LLM backend, if applicable)

## 🛠️ Usage

1. Open the GUI.
2. Type your question in the input field.
3. The system fetches the required context and queries the LLM.
4. Get accurate answers instantly!

## 🚀 How It Works

1. **Context Extraction:** The system analyzes your project files and metadata.
2. **Retrieval-Augmented Generation (RAG):** It fetches relevant snippets using smart retrieval.
3. **LLM Querying:** The refined query is sent to the LLM.
4. **Answer Generation:** The response is displayed in the GUI.





# 📌 Setup Guide

## 🛠️ Prerequisites

To build and run the project, ensure you have the following installed on your system:

### **1. Install Premake5**

Premake5 is used as the build system. Follow these steps to install it:

1. Download Premake5 from the official site: https://premake.github.io/download/

2. Extract the downloaded archive.

3. Add Premake5 to your system PATH:

    - Open **System Properties** → **Advanced** → **Environment Variables**.
    - Under `System Variables`, find `Path` and click **Edit**.
    - Click **New** and add the path where `premake5.exe` is located.
    - Click **OK** to save changes.

4. Verify installation by running the following command in 

    Command Prompt:

    ```sh
    premake5 --version
    ```

### **2. Install Visual Studio (MSVC Compiler)**

1. Download and install **Visual Studio** from: https://visualstudio.microsoft.com/
2. During installation, select the following workloads:
    - **Desktop development with C++**
    - Ensure **MSVC v142 (or later)** is checked under **Individual Components**.
    - Include **CMake tools for Windows** if not already selected.
3. Restart your system after installation.

------

## 🚀 Project Setup

Once all dependencies are installed, follow these steps:

1. Clone the repository:

    ```sh
    git clone https://github.com/yourusername/ask-llm.git
    cd ask-llm
    ```

2. Run the setup script (Only required once):

    ```sh
    setup.bat
    ```

    - This script will:
        - Download and install all required packages and dependencies.
        - Build the project automatically.

------

## 🔄 Building the Project

For further development, you don’t need to run `setup.bat` again. Instead, use:

```sh
build.bat
```

This will compile the project using the existing setup.

------

## 📚 Libraries Used

The project uses the following external libraries:

- **GLFW** - Window management and input handling.
- **ImGui** - Immediate-mode GUI library.
- **LunaSVG** - Scalable Vector Graphics (SVG) rendering.
- **SpdLog** - Fast and powerful logging library.
- **nlohmann JSON** - JSON parsing and serialization.
- **OpenGL** - Graphics rendering API.
- **dwmapi** - Windows Desktop Window Manager API.
- **Shlwapi** - Windows Shell Light-weight Utility API.
- **winmm** - Windows Multimedia API.

------

## ✅ Verification

To confirm everything is set up correctly:

1. Open **Command Prompt** and navigate to the project folder.

2. Run:

    ```sh
    build.bat
    ```

3. If the build completes successfully, the setup is complete!

------

For any issues, open an issue on the GitHub repository or contact the project maintainer.

Happy coding! 🚀)";
    Markdown( markdownText );
}


// For files being dragged and dropped
void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	for (int i = 0; i < count; i++) {
		if (std::filesystem::is_directory(paths[i])) {
			GL_INFO("Folder: {}", paths[i]);
		} else {
			GL_INFO("File: {}", paths[i]);
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);
	Application::Get().Draw();
}

bool Application::Init()
{
	OpenGL::ScopedTimer timer("Application::Init");

#ifdef GL_DEBUG
	OpenGL::Log::Init();
#endif

	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1);

	Get().mWindow = glfwCreateWindow(Get().width, Get().height, APP_NAME, NULL, NULL);
	glfwSetWindowSizeLimits(Get().mWindow, 380, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);
	if (!Get().mWindow) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(Get().mWindow);
	GL_INFO("OPENGL - {}", (const char*)glGetString(GL_VERSION));
	HWND WinHwnd = glfwGetWin32Window(Application::GetGLFWwindow());
	BOOL USE_DARK_MODE = true;
	BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS =
	    SUCCEEDED(DwmSetWindowAttribute(WinHwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &USE_DARK_MODE, sizeof(USE_DARK_MODE)));


	glfwSetDropCallback(Get().mWindow, drop_callback);
	glfwSetFramebufferSizeCallback(Get().mWindow, framebuffer_size_callback);
	return true;
}


void Application::SetApplicationIcon(unsigned char* logo_img, int length)
{
	OpenGL::ScopedTimer timer("AppIconLoadTime");
	GLFWimage images[1];
	images[0].pixels = stbi_load_from_memory(logo_img, length, &images[0].width, &images[0].height, 0, 4); // rgba channels
	glfwSetWindowIcon(Get().mWindow, 1, images);
	stbi_image_free(images[0].pixels);
}


bool Application::InitImGui()
{
	GL_INFO("ImGui Init");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename=nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef IMGUI_HAS_DOCK
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Enable when you what the imgui window to be dragged outside the window
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
#endif



#ifdef IMGUI_HAS_DOCK
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
#endif

	if (!ImGui_ImplGlfw_InitForOpenGL(Get().mWindow, true)) {
		GL_CRITICAL("FAILED INIT IMGUI");
		return false;
	}
#ifdef GL_BUILD_OPENGL2
	if (!ImGui_ImplOpenGL2_Init()) {
		GL_INFO("INIT OPENGL2");
#else
	if (!ImGui_ImplOpenGL3_Init()) {
		GL_INFO("INIT OPENGL3");
#endif
		GL_ERROR("Failed to initit OpenGL");
		return false;
	}
	GL_INFO("OPENGL - {}", (const char*)glGetString(GL_VERSION));

	glfwSwapInterval(0); // Gives maximum FPS
	// SetStyleColorDarkness();

	return true;
}

void Application::CenterWindowOnScreen()
{
	int horizontal = 0;
	int vertical = 0;
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void Application::BackupDataBeforeCrash()
{

}

void Application::SleepForFPS(int aTargetFPS) {
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto frame_time = std::chrono::milliseconds(1000 / aTargetFPS);

    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);

    if (elapsed < frame_time) {
        std::this_thread::sleep_for(frame_time - elapsed);
    }

    last_time = std::chrono::high_resolution_clock::now();
}

int Application::GetTargetFPS() {
	// Focused - 30
	// Unfocused/Minimized - 10
	GLFWwindow* window=Application::GetGLFWwindow();

    if (!glfwGetWindowAttrib(window, GLFW_FOCUSED) || glfwGetWindowAttrib(window, GLFW_ICONIFIED))
    	return 10;

    return 120;  // Unfocused but visible window: Moderate FPS
}

void Application::HandleFPSCooldown(){
    static auto last_scroll_time = std::chrono::high_resolution_clock::now();

    if (Get().mEnableRunAtMaxRefreshRate) {
        last_scroll_time = std::chrono::high_resolution_clock::now();
        Get().mRunAtMaxRefreshRate=true;
        Get().mEnableRunAtMaxRefreshRate=false;
        glfwSwapInterval(1);
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - last_scroll_time);

    if (elapsed.count() > 500) { // 200ms after last scroll
    	Get().mRunAtMaxRefreshRate=false;
        glfwSwapInterval(0);
    }
}


void Application::RenderFPSInTitleBar(){
    static double prevTime = 0.0;
    static double crntTime = 0.0;
    static double timeDiff;
    static unsigned int counter = 0;

    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    counter++;
    if (timeDiff >= 1.0 / 30.0)
    {
        std::string FPS = std::to_string(int((1.0 / timeDiff) * counter));
        std::string ms = std::to_string((timeDiff / counter) * 1000);
        std::string newTitle = "Ask LLM(Debug) - " + FPS + "FPS / " + ms + "ms";
        glfwSetWindowTitle(Application::GetGLFWwindow(), newTitle.c_str());

        prevTime = crntTime;
        counter = 0;
    }
}




void Application::Draw()
{
    // Process window events
    glfwPollEvents();

    // Skip rendering if window is minimized
    if (glfwGetWindowAttrib(Get().mWindow, GLFW_ICONIFIED)) return;

    // Skip rendering if the window is not focused (but still process events)
    // if (!Application::IsWindowFocused()) return; -- freezes the ui no updates seen

#ifdef GL_BUILD_OPENGL2
    ImGui_ImplOpenGL2_NewFrame();
#else
    ImGui_ImplOpenGL3_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render application UI
    Get().mChatWindow.Render();
    ImGui::ShowDemoWindow();
    // MarkdownExample();

	if(ImGui::GetIO().MouseWheel != 0.0f || ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		EnableHighFPS();


    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(Get().mWindow, &display_w, &display_h);
    if (display_w > 0 && display_h > 0) {
        glViewport(0, 0, display_w, display_h);
    }

    // Clear screen (minimize state check prevents unnecessary clearing)
    glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render ImGui draw data
#ifdef GL_BUILD_OPENGL2
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#else
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

#ifdef IMGUI_HAS_DOCK
    // Ensure platform windows are updated and rendered
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(Get().mWindow); // Ensure we are back to the main context
    }
#endif



    glfwSwapBuffers(Get().mWindow);
}


void Application::Destroy()
{
#ifdef GL_BUILD_OPENGL2
	ImGui_ImplOpenGL2_Shutdown();
#else
	ImGui_ImplOpenGL3_Shutdown();
#endif
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(GetGLFWwindow());
	glfwTerminate();
}

void Application::HandleArguments(std::wstring commands)
{
	// TODO: Display Notification to user if something goes wrong
	GL_INFO("Parsing Args");
	namespace fs = std::filesystem;
	int nArgs;

	LPWSTR* wArgList = CommandLineToArgvW(commands.c_str(), &nArgs);
	if (!wArgList) {
		GL_ERROR("Failed to parse args");
		return;
	}


	for (int i = 1; i < nArgs; ++i) {
		fs::path path(wArgList[i]);
		if (fs::exists(path)) {
			if (fs::is_regular_file(path)) {
				GL_INFO("FILE:{}", path.generic_string());
				// core->GetTextEditor()->LoadFile(path.generic_string().c_str());
			} else if (fs::is_directory(path)) {
				GL_INFO("FOLDER:{}", path.generic_string());
			} else {
				ShowErrorMessage("Invalid File/Folder Selected");
			}
		} else {
			ShowErrorMessage("Path Doesn't Exists");
		}
	}

	LocalFree(wArgList);
}


void Application::HandleCrash(int signal)
{
	const char* errorMessage = nullptr;
	switch (signal) {
		case SIGSEGV:
			errorMessage = "Segmentation fault";
			break;
		case SIGABRT:
			errorMessage = "Abort signal received";
			break;
		case SIGFPE:
			errorMessage = "Floating point exception";
			break;
		case SIGILL:
			errorMessage = "Illegal instruction";
			break;
		case SIGTERM:
			errorMessage = "Termination request";
			break;
		case SIGINT:
			errorMessage = "User Interrupt(Ctrl+C)";
		default:
			errorMessage = "Unknown error";
			break;
	}

	GL_CRITICAL("Error:{} (Signal: {})", errorMessage, signal);
}


void Application::SetupSystemSignalHandling()
{
	std::signal(SIGSEGV, Application::HandleCrash); // Segmentation fault
	std::signal(SIGABRT, Application::HandleCrash); // Abort
	std::signal(SIGFPE, Application::HandleCrash);  // Floating point exception
	std::signal(SIGILL, Application::HandleCrash);  // Illegal instruction
	std::signal(SIGTERM, Application::HandleCrash); // Termination request
	std::signal(SIGINT, Application::HandleCrash);  // Interrupt (Ctrl+C)
}


float GetFontSize()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	float baseSize = 12.0f;
	float scaleFactor = 0.007f;
	float fontSize = baseSize + std::min(screenWidth, screenHeight) * scaleFactor;
	return fontSize;
}


void Application::InitFonts()
{
	GL_INFO("Initializing Fonts");
	const ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	ImFontConfig icon_config;
	icon_config.MergeMode = true;
	icon_config.PixelSnapH = true;
	icon_config.FontDataOwnedByAtlas = false;

	ImFontConfig font_config;
	font_config.FontDataOwnedByAtlas = false;
	const float font_size = GetFontSize();
	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf",font_size+3,&font_config);
	io.Fonts->AddFontFromMemoryTTF((void*)JetBrainsMonoNLRegular, IM_ARRAYSIZE(JetBrainsMonoNLRegular), font_size+2.0f, &font_config);
	io.Fonts->AddFontFromMemoryTTF((void*)FontAwesomeSolid, IM_ARRAYSIZE(FontAwesomeSolid), (font_size + 4.0f) * 2.0f / 3.0f, &icon_config,
	                               icons_ranges);

	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuii.ttf",font_size+3,&font_config);
	io.Fonts->AddFontFromMemoryTTF((void*)JetBrainsMonoNLItalic, IM_ARRAYSIZE(JetBrainsMonoNLItalic), font_size + 2.0f, &font_config);

	io.Fonts->AddFontFromMemoryTTF((void*)JetBrainsMonoNLRegular, IM_ARRAYSIZE(JetBrainsMonoNLRegular), font_size+2.0f, &font_config);
	io.Fonts->AddFontFromMemoryTTF((void*)FontAwesomeRegular, IM_ARRAYSIZE(FontAwesomeRegular), (font_size + 4.0f) * 2.0f / 3.0f,
	                               &icon_config, icons_ranges);

    // io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeui.ttf", 24 );
    // io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuib.ttf", 24 );
    // io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuib.ttf", 32 );
    // io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuii.ttf", 24 );

    io.Fonts->AddFontFromFileTTF( "./assets/fonts/AROneSans-Medium.ttf", 24 );
    io.Fonts->AddFontFromFileTTF( "./assets/fonts/AROneSans-Bold.ttf", 24 );
    io.Fonts->AddFontFromFileTTF( "./assets/fonts/AROneSans-Bold.ttf", 32 );
    io.Fonts->AddFontFromFileTTF( "./assets/fonts/AROneSans-Medium.ttf", 24 );
}
