# Aspasia — Counter-Strike 2 Reverse Engineering Framework

<div align="justify">

**Aspasia** is a C++ research framework for studying and prototyping game manipulation techniques in *Counter-Strike 2 (CS2)*. It was developed as part of a Master's Thesis at Universidad Carlos III de Madrid.

This project is closely tied to the accompanying thesis document, which explores implementation details, design rationale, and the broader context of game cheating and reverse engineering. While Aspasia is not intended for unfair use in public matches, it serves as a tool to understand how game and anti-cheat systems work under the hood.

---

## 🖼️ Screenshots

<p align="center">
  <img src="Master Thesis Template/clips/anti_aim_clip.gif" alt="Anti-Aim Demo" width="1000"/>
</p>
<p align="center"><em>Animated preview of anti-aim behavior and overlay visuals in action.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608222549_1.jpg" alt="Target Bone Visualization" width="700"/>
</p>
<p align="center"><em>Chams and skeletal overlays for target visualization and debugging.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608215858_1.jpg" alt="Entity Bone Overlay" width="700"/>
</p>
<p align="center"><em>Real-time bone overlays showing all entities in view for runtime inspection.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/chams/chams_demo/20250608210828_1.jpg" alt="Chams Demonstration" width="700"/>
</p>
<p align="center"><em>UI menu with chams tab open, used for configuring material and visibility settings.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/20250615230119_1.jpg" alt="Visual Overlay Snapshot" width="1000"/>
</p>
<p align="center"><em>Close-up of USP-S and player arms using custom chams for visual debugging.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/20250615230003_1.jpg" alt="Debug Menu Showcase" width="1000"/>
</p>
<p align="center"><em>Debug overlay and UI elements for weapon (M4A1) and player view configuration.</em></p>

---

## ⚠️ Disclaimer

This repository is intended solely for research and educational purposes. It was developed as part of a personal learning journey and academic exploration of game security and reverse engineering.

Using this software to gain unfair advantages in online games is neither supported nor encouraged. That said, individuals with sufficient technical knowledge and custom modifications might adapt it toward such use. Doing so would require bypassing various validation mechanisms implemented by Valve Anti-Cheat (VAC), including integrity checks found in modules like `steamservice.dll`. While somewhat outdated, [danielkrupinski's vac-hooks repository](https://github.com/danielkrupinski/vac-hooks) provides useful insights into these techniques.

Ultimately, you are fully responsible for any use or consequences that may result from interacting with this codebase.

---

## ✨ Features

- **Aimbot System**
  - Hitbox selection and FOV constraints
  - Smooth aiming using interpolation factor
- **Chams Rendering**
  - Custom materials and entity visibility overrides
  - Support for both hidden and visible states
- **Anti-Aim / Desync**
  - Lower Body Yaw (LBY) manipulation
- **Hooking Infrastructure**
  - MinHook-based detours and inline patching
- **Entity Analysis**
  - Runtime tracking via OnAddEntity / OnRemoveEntity
  - Memory structure monitoring
- **Movement Research**
  - Automatic bunny hopping (Bhop)
- **Visual Clarity Tweaks**
  - Flashbang and smoke rendering disabled for unobstructed visibility
- **Player Overlay Enhancements**
  - Health bars, bone skeletons, and equipped weapon indicators for visible players

---

## 🔧 Compilation

### Requirements

- Windows 10/11 (x64)
- Visual Studio 2019 or later
- DirectX SDK (June 2010)
- ImGui and MinHook (already bundled)
- C++20 support (`/std:c++20`)
- Only the `x64` configuration is actively maintained and tested

### Instructions

1. Download the repository as a ZIP or use Git for Windows:
   ```cmd
   git clone https://github.com/Aspasia1337/Aspasia.git
   ```

2. Open `SourceHook.sln` in Visual Studio.

3. Set the build configuration to `Release x64`.

4. Build the solution — `Aspasia.dll` will be generated in the output directory.

---

## 💉 Injection Guidelines

This project does not include an injector. Due to the nature of Valve Anti-Cheat (VAC), standard injection methods such as `LoadLibrary` are detectable and unsafe.

For safe usage in a controlled environment, the loader should support:
- Manual mapping
- Kernel-assisted injection
- Memory concealment or obfuscation techniques

> VAC employs memory validation and behavioral heuristics. In public matches, using this without a trusted bypass is likely to result in a VAC ban.


If injection is successful, an in-game console will appear displaying logs. This confirms proper loading and hook initialization.

<p align="center">
  <img src="Master Thesis Template/images/inject.png" alt="Injection Console" width="800"/>
</p>

Once in-game, the main menu can be opened using the `INSERT` key.

---

## 🧪 Research Purpose

Aspasia is not a conventional cheat—it is a research tool.

It was designed to:
- Demonstrate reverse engineering and game manipulation techniques
- Model typical anti-cheat detection surfaces
- Serve as a platform for in-memory inspection and live experimentation

It is particularly suited for educational demonstrations in sandboxed or offline environments.

---

## 🎓 Academic Context

This project was developed as part of a Master's Thesis at Universidad Carlos III de Madrid. A full written thesis accompanies the repository, detailing implementation, context, and reflections on game security.

📄 [Read the thesis here](https://github.com/Aspasia1337/Aspasia/blob/main-ui/Master%20Thesis%20Template/memoria.pdf)

It’s worth mentioning that this project was developed from a learning perspective as part of my academic journey. I don’t consider myself an expert, and much of the code was written under time constraints in preparation for the thesis presentation. There’s plenty of room for improvement, and I hope that sharing this work can help others just as the communities of UnknownCheats, GuidedHacking, and many others have helped me.

---

## 🤝 Contributions

Contributions are welcome — particularly those focusing on:
- Code quality
- Loader development
- Schema automation
- Anti-cheat research

Open an issue or submit a pull request if you'd like to contribute.

---

## 🔄 Potential Improvements

The following improvements are planned or suggested:

- **Code Refactor**: Modularization, naming, and clarity improvements across the C++ codebase
- **Encrypted Strings**: Use compile-time obfuscation macros (e.g., `XORSTR`)
- **Dynamic Schema Manager**: Automatically resolve netvars and offsets
- **Smarter Targeting**: Improve aimbot selection logic using prediction and prioritization
- **Skin Changer**: Offline inventory simulation with custom skins

---

## 🧾 License

This project is licensed under the **GNU General Public License v3.0**.

You may:
- Use the code for academic, personal, or educational purposes
- Modify and redistribute it under the same license
- Study and adapt it, provided derivatives remain open-source under GPL-3.0

See the [`LICENSE`](./LICENSE) file for complete terms.

---

**Aspasia** — Exploring the inner workings of Counter-Strike 2 through offensive and defensive analysis.

</div>
