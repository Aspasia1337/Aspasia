# Aspasia - Counter-Strike 2 Reverse Engineering Framework

<p align="center">
  <img src="https://img.shields.io/badge/platform-Windows%20x64-blue"/>
  <img src="https://img.shields.io/badge/language-C%2B%2B17-brightgreen"/>
  <img src="https://img.shields.io/badge/context-Master's%20Thesis-blueviolet"/>
  <img src="https://img.shields.io/badge/license-GNU%20GPLv3-blue"/>
</p>

**Aspasia** is a C++ research framework for studying and prototyping game manipulation techniques in *Counter-Strike 2 (CS2)*. It was developed as part of a Master's Thesis at Universidad Carlos III de Madrid.

This project is closely tied to the accompanying thesis document, which explores implementation details, design rationale, and the broader context of game cheating and reverse engineering. While Aspasia is not intended for unfair use in public matches, it serves as a tool to understand how game and anti-cheat systems work under the hood.

---

## Disclaimer

This repository is intended solely for research and educational purposes. It was developed as part of a personal learning journey and academic exploration of game security and reverse engineering.

Using this software to gain unfair advantages in online games is neither supported nor encouraged. That said, individuals with sufficient technical knowledge and custom modifications might adapt it toward such use. Doing so would require bypassing various validation mechanisms implemented by Valve Anti-Cheat (VAC), including integrity checks found in modules like `steamservice.dll`. While somewhat outdated, [danielkrupinski's vac-hooks repository](https://github.com/danielkrupinski/vac-hooks) provides useful insights into these techniques.

Ultimately, you are fully responsible for any use or consequences that may result from interacting with this codebase.

---

## 1. In-Game Preview: Feature Demonstrations

<p align="center">
  <img src="thesis/clips/menu.gif" alt="Menu UI Demo" width="1000"/>
</p>
<p align="center"><em>UI demonstration of the in-game menu system, featuring tab navigation and real-time configuration.</em></p>

<p align="center">
  <img src="thesis/clips/aimbot_smoothing.gif" alt="Aimbot Smoothing Demo" width="1000"/>
</p>
<p align="center"><em>Aimbot with smoothing (interpolation) and chams enabled for both visible and hidden targets.</em></p>

<p align="center">
  <img src="thesis/clips/anti_aim_clip.gif" alt="Anti-Aim Demo" width="1000"/>
</p>
<p align="center"><em>Animated preview of anti-aim behavior and overlay visuals in action.</em></p>

<p align="center">
  <img src="thesis/clips/flashbang.gif" alt="Flashbang Removal Demo" width="1000"/>
</p>
<p align="center"><em>Flashbang effect disabled during gameplay, enabling uninterrupted visual clarity.</em></p>

<p align="center">
  <img src="thesis/clips/smokerender.gif" alt="Smoke Rendering Removal Demo" width="1000"/>
</p>
<p align="center"><em>Smoke rendering bypassed to maintain full visibility in obstructed areas.</em></p>

<p align="center">
  <img src="thesis/clips/gameplay.gif" alt="Gameplay Preview" width="1000"/>
</p>
<p align="center"><em>Live gameplay preview showing multiple features working together in a real match context.</em></p>

<p align="center">
  <img src="thesis/images/Bones/20250608222549_1.jpg" alt="Target Bone Visualization" width="1000"/>
</p>
<p align="center"><em>Chams and skeletal overlays for target visualization and debugging.</em></p>

<p align="center">
  <img src="thesis/images/Bones/20250608215858_1.jpg" alt="Entity Bone Overlay" width="1000"/>
</p>
<p align="center"><em>Real-time bone overlays showing all entities in view for runtime inspection.</em></p>

<p align="center">
  <img src="thesis/images/chams/chams_demo/20250608210828_1.jpg" alt="Chams Demonstration" width="1000"/>
</p>
<p align="center"><em>UI menu with chams tab open, used for configuring material and visibility settings.</em></p>

<p align="center">
  <img src="thesis/images/20250615230119_1.jpg" alt="Visual Overlay Snapshot" width="1000"/>
</p>
<p align="center"><em>Close-up of USP-S and player arms using custom chams.</em></p>

<p align="center">
  <img src="thesis/images/20250615230003_1.jpg" alt="Debug Menu Showcase" width="1000"/>
</p>
<p align="center"><em>Close-up of M4A1 and player arms using custom chams.</em></p>

---

## 2. Key Features Overview

- **Aimbot System**
  - Hitbox selection and FOV constraints
  - Smooth aiming using interpolation factor

- **Chams Rendering**
  - Custom materials and entity visibility overrides
  - Support for both hidden and visible states

- **Anti-Aim / Desync**
  - Lower Body Yaw (LBY) manipulation

- **Entity Analysis**
  - Runtime tracking via OnAddEntity / OnRemoveEntity
  - Memory structure monitoring

- **Movement Research**
  - Automatic bunny hopping (Bhop)

- **Visual Clarity Tweaks**
  - Flashbang and smoke rendering disabled for unobstructed visibility

- **Light and Sky Modulation**
  - Adjust in-game ambient lighting and skybox color values for enhanced visibility or custom aesthetics

- **Player Overlay Enhancements**
  - Health bars, bone skeletons, and equipped weapon indicators for visible players

---

## 3. Project Structure

| Directory | Description |
|---|---|
| `Features/` | Individual feature modules (aimbot, visuals, anti-aim, movement, misc) |
| `HooksManager/` | Hook setup and lifecycle management via MinHook |
| `EntityManager/` | Entity tracking via `OnAddEntity` / `OnRemoveEntity` callbacks |
| `Interfaces/` | CS2 interface resolution at runtime |
| `Classes/` | Game class definitions and memory layout structs |
| `helper/` | PE utilities and miscellaneous helpers |
| `math/` | Vector math |
| `includes/` | Third-party libraries (ImGui, Kiero, MinHook, offsets) |
| `thesis/` | Academic write-up, slides, screenshots, and GIFs |

---

## 4. Compilation & Setup

### 4.1 System Requirements

- Windows 10/11 (x64)
- Visual Studio 2019 or later
- DirectX SDK (June 2010)
- ImGui and MinHook (already bundled)
- C++17 support (`/std:c++17`)
- Only the `x64` configuration is actively maintained and tested

### 4.2 Build Instructions

1. Download the repository as a ZIP or use Git for Windows:

   ```cmd
   git clone https://github.com/Aspasia1337/Aspasia.git
   ```

2. Open `Aspasia Solution.sln` in Visual Studio.

3. Set the build configuration to `Release x64`.

4. Build the solution — `Aspasia.dll` will be generated in the output directory.

---

## 5. Injection Guidelines & Runtime Behavior

This project does not include an injector. Due to the nature of Valve Anti-Cheat (VAC), standard injection methods such as `LoadLibrary` are detectable and unsafe.

For safe usage in a controlled environment, the loader should support:

- Manual mapping
- Kernel-assisted injection
- Memory concealment or obfuscation techniques

> VAC employs memory validation and behavioral heuristics. In public matches, using this without a trusted bypass is likely to result in a VAC ban.

If injection is successful, an in-game console will appear displaying logs. This confirms proper loading and hook initialization.

<p align="center">
  <img src="thesis/images/inject.png" alt="Injection Console" width="800"/>
</p>

Once in-game, the main menu can be opened using the `INSERT` key.

---

## 6. Research Intent & Scope

Aspasia is not a conventional cheat—it is a research tool.

It was designed to:

- Demonstrate reverse engineering and game manipulation techniques
- Model typical anti-cheat detection surfaces
- Serve as a platform for in-memory inspection and live experimentation

It is particularly suited for educational demonstrations in sandboxed or offline environments.

### Technical Notes (as of April 2026)

- Anti-cheat module streaming is currently **disabled** — VAC modules are not being pushed to clients at this time.
- The most significant detection logic resides in **`client.dll`**, which is where the bulk of integrity and behavioral checks are implemented.

---

## 7. Academic Background

This project was developed as part of a Master's Thesis at Universidad Carlos III de Madrid. A full written thesis accompanies the repository, detailing implementation, context, and reflections on game security.

| Resource | Description | Link |
|---|---|---|
| Thesis (PDF) | Full academic write-up of the Aspasia project | [Read the thesis](https://github.com/Aspasia1337/Aspasia/blob/main-ui/thesis/memoria.pdf) |
| Slides (PDF) | Beamer presentation with feature visuals | [View the slides](https://github.com/Aspasia1337/Aspasia/blob/main-ui/slides/slides.pdf) |

> To properly render the animated GIFs in the presentation, use a PDF viewer that supports embedded animations, such as Adobe Reader or a modern browser.

Special thanks to **Alexey Zabashta** for providing the [ITMO Beamer Theme](https://www.overleaf.com/latex/templates/itmo-beamer-theme/fpttrgnmqwsb), which served as the foundation for the presentation slides.

### Acknowledgements

This project was developed from a learning perspective as part of my academic journey. Much of the code was written under time constraints in preparation for the thesis presentation. I hope that sharing this work can be useful to others, just as the communities of UnknownCheats, GuidedHacking, and many others have been to me.

---

## 8. Potential Enhancements

This project is complete as a thesis submission and is no longer actively developed. The following were identified as areas for future work:

- **Code Refactor**: Modularization, naming, and clarity improvements across the C++ codebase
- **Encrypted Strings**: Use compile-time obfuscation macros (e.g., `XORSTR`)
- **Dynamic Schema Manager**: Automatically resolve netvars and offsets
- **Smarter Targeting**: Improve aimbot selection logic using prediction and prioritization
- **Skin Changer**: Inventory simulation with custom skins

---

## 9. License & Usage Terms

This project is licensed under the **GNU General Public License v3.0**.

You may:

- Use the code for academic, personal, or educational purposes
- Modify and redistribute it under the same license
- Study and adapt it, provided derivatives remain open-source under GPL-3.0

See the [`LICENSE`](./LICENSE) file for complete terms.

---

**Aspasia** — Exploring the inner workings of Counter-Strike 2 through offensive and defensive analysis.
