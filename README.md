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