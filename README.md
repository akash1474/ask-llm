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

3. Add Premake5 to your system 

    ```
    PATH
    ```

    :

    - Open **System Properties** → **Advanced** → **Environment Variables**.
    - Under `System Variables`, find `Path` and click **Edit**.
    - Click **New** and add the path where `premake5.exe` is located.
    - Click **OK** to save changes.

4. Verify installation by running the following command in 

    Command Prompt

    :

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

1. Clone the repository

    :

    ```sh
    git clone https://github.com/yourusername/ask-llm.git
    cd ask-llm
    ```

2. Run the setup script

     (Only required once):

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

Happy coding! 🚀