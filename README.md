# Aspasia — Counter-Strike 2 Reverse Engineering Framework
<p align="center">
  <img src="Master Thesis Template/images/AspasiaBanner.png" alt="Aspasia Banner" width="1000"/>
</p>

**Aspasia** is a C++ research framework designed for studying and prototyping game manipulation techniques in *Counter-Strike 2 (CS2)*. It was developed as part of a Master's Thesis at Universidad Carlos III de Madrid and serves both academic and technical purposes.

The project explores game internals, cheat construction, and the broader landscape of reverse engineering in multiplayer environments. It is not intended for unfair gameplay in public matches, but rather as a tool to deepen understanding of how game and anti-cheat mechanisms work.

---

## 🖼️ Screenshots

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608222549_1.jpg" alt="Target Bone Visualization" width="700"/>
</p>
<p align="center"><em>Visualizing precise bone targeting used for aimbot calibration.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608215858_1.jpg" alt="Entity Bone Overlay" width="700"/>
</p>
<p align="center"><em>Overlay of all entity bones during runtime for debugging and development.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/chams/chams_demo/20250608210828_1.jpg" alt="Chams Demonstration" width="700"/>
</p>
<p align="center"><em>Chams applied to various entity states with material highlighting.</em></p>

---

## ⚠️ Disclaimer

This repository is intended strictly for research and educational purposes. The use of this software to gain unfair advantages in online games is neither supported nor encouraged. You are solely responsible for any use.

---

## ✨ Features

- **Aimbot System**
  - Hitbox selection and FOV constraints
  - Smooth aiming and recoil control
- **Chams Rendering**
  - Custom materials and entity visibility overrides
  - Supports both hidden and visible states
- **Anti-Aim / Desync**
  - Lower Body Yaw breaking
  - Desynchronized local animations
  - Anti-resolver patterns
- **Hooking Infrastructure**
  - MinHook-based detours and inline patching
  - Function trampolines with memory safety
- **Entity Analysis Tools**
  - Hooking OnAddEntity / OnRemoveEntity
  - Live tracking of class instances and properties
- **Movement Research**
  - Acceleration curves, Bhop modeling, strafe logic

---

## 🔧 Compilation

### Requirements
- Windows 10/11 (x64)
- Visual Studio 2019 or later
- DirectX SDK (June 2010)
- ImGui, MinHook
- Only the `x64` configuration is actively maintained and tested
- C++20 support must be enabled (set via `/std:c++20` in the project)
- MinHook and ImGui are already bundled in the repository (no external install required)

### Instructions

1. Download the repository as a ZIP from GitHub or use Git for Windows:
   - Open PowerShell or Command Prompt
   - Run:  
     ```cmd
     git clone https://github.com/Aspasia1337/Aspasia.git
     ```

2. Open `SourceHook.sln` in Visual Studio (2019 or later)

3. Set the build configuration to `Release x64`

4. Build the solution, this will generate `Aspasia.dll` inside the appropriate output directory

---

## 💉 Injection Guidelines

This project does not include an injector. Due to VAC (Valve Anti-Cheat) protections, standard methods such as `LoadLibrary` are highly detectable and unsafe.

To use this in a controlled environment, a proper loader must support:
- Manual mapping
- Kernel-assisted injection
- Code obfuscation / memory concealment

> VAC performs internal consistency and memory validation checks that can trigger post-injection, especially on `.text` segments. Using this in public matches **without a trusted bypass** will likely result in a VAC ban.

---

## 🧪 Research Purpose

Aspasia is not a cheat in the conventional sense, it's a tool for understanding them.

This framework was designed to:
- Demonstrate common game manipulation strategies
- Analyze attack surfaces in game engines
- Understand detection vectors from the perspective of anti-cheat systems

It also supports educational demonstration of memory analysis and runtime manipulation techniques in live environments.

---

## 🎓 Academic Context

This project is part of a Master's Thesis submitted at Universidad Carlos III de Madrid. It is accompanied by a full thesis document that provides theoretical background and analysis on game cheating, reverse engineering practices, and anti-cheat architectures.

You can read the full thesis here: [memoria.pdf](https://github.com/Aspasia1337/Aspasia/blob/main-ui/Master%20Thesis%20Template/memoria.pdf)

Sharing this repository is a way of contributing back to the community that inspired it. Countless resources from platforms like UnknownCheats and GuidedHacking played a role in this research. This work aims to pass that knowledge forward.

This project was created from a researcher's perspective and is far from a professional-grade cheat implementation. I'm not an expert, and part of the motivation behind releasing this is to make visible the process of learning, prototyping, and iterating under the pressure of a Master's Thesis. There is plenty of room for improvement—and that is by design.

---

## 🤝 Contributions

Feel free to fork the project, report issues, or propose improvements. Contributions focused on security research, cheat detection, loader design, or schema abstraction are particularly welcome.

---

## 🔄 Potential Improvements

The current implementation is functional for demonstration purposes, but there are several enhancements planned or under consideration to increase robustness, stealth, and extensibility:

- **Code Quality Refactor**: The C++ codebase was developed under tight academic deadlines. Significant improvements could be made in structure, style, naming conventions, modularity, and modern C++ idioms to enhance maintainability and clarity.
- **Encrypted String Storage**: Replace plain string literals with runtime-decrypted constants using techniques like `XORSTR` or compile-time encrypted string macros.
- **Schema Manager**: Implement a dynamic netvar and offset resolution system to remove hardcoded addresses and support post-update compatibility.
- **Target Selection Enhancements**: Refine the aimbot's entity selection using prediction, visibility heuristics, and hitbox prioritization.
- **Skin Changer Module**: Introduce a customizable inventory/skin changer system for offline or sandbox testing scenarios.

---

## 🧾 License

This project is licensed under the **GNU GPL v3.0**.

You are free to:
- Use the code for personal, academic, or non-commercial purposes
- Modify and redistribute it under the same license
- Study its design and adapt it for your own learning

Any derivative work must remain open-source and distributed under GPL-3.0.

See the [`LICENSE`](./LICENSE) file for full terms.

---

**Aspasia** — Investigating the internals of Counter-Strike 2 from both sides of the game.
