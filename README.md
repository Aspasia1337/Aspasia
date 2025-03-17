# Aspasia - CS2 Research Project

Aspasia is an academic research project analyzing the mechanics of Counter-Strike 2 (CS2). Developed in C++, it investigates in-game interactions such as movement physics, aiming behavior, and rendering techniques. The project uses DirectX and ImGui for graphical rendering and MinHook for function interception.

## ⚠️ Disclaimer
Aspasia is strictly for academic research and educational purposes. It must not be used for unauthorized modifications, online cheating, or any activity that violates game integrity. The developers are not responsible for misuse of this code.

---

## 📌 Key Features
- **Targeting Mechanics**: Research-focused aim assistance to analyze player precision and reaction times.
- **Movement Analysis**: Studies on strafing, acceleration, bunny hopping, and movement optimizations.
- **Visual Enhancements**: Advanced rendering modifications to better observe game environments.
- **Behavioral Studies**: Evaluation of in-game events, such as player evasion techniques and reaction-based responses.
- **Advanced Hooking Framework**: Utilizes MinHook to safely intercept and analyze game functions.

## 🛠️ Dependencies
Aspasia requires the following libraries to function:
- **DirectX SDK**: Renders graphical overlays and in-game visual modifications.
- **ImGui**: Provides a lightweight, customizable user interface for in-game research tools.
- **MinHook**: A function hooking library that allows runtime interception and analysis of game behavior.

## 🚀 Installation & Compilation
### System Requirements
- **Operating System**: Windows 10/11 (64-bit only)
- **Compiler**: Microsoft Visual Studio 2019 or later

### How to Compile
1. Clone the repository:
   ```sh
   git clone https://github.com/Aspasia1337/Aspasia
   cd Aspasia
   ```
2. Open `SourceHook.sln` in Visual Studio.
3. Set the build configuration to `Release x64`.
4. Compile the project to generate `Aspasia.dll`.
5. Load the DLL in a controlled research environment.

## 🔧 DLL Injection Requirement
To inject `Aspasia.dll` into the game process, a **custom injector** is required. Standard injection methods such as `LoadLibrary` are blocked by CS2's security mechanisms. Ensure that the injector is designed for research purposes and follows safe injection techniques.

## ❓ Frequently Asked Questions
### Is Aspasia undetectable?
Aspasia is not intended for competitive use or cheating. It is a research project designed to study in-game mechanics under a controlled environment.

### Can I contribute to Aspasia?
Yes! If you are interested in game mechanics research, feel free to fork the repository and submit pull requests with improvements, optimizations, or new insights.

## 📜 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 📬 Contact
For academic inquiries or contributions, please open an issue on GitHub or contact the project maintainers.

---
**Aspasia** - A research project analyzing Counter-Strike 2 game mechanics.
