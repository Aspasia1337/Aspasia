<p align="center">
  <img src="Master Thesis Template/images/AspasiaBanner.png" alt="Aspasia Banner" width="700"/>
</p>
# Aspasia — CS2 Reverse Engineering Suite

Aspasia is a C++-based research framework for reverse engineering and prototyping cheat features in **Counter-Strike 2 (CS2)**. Designed for educational and academic purposes, it enables both offensive analysis (cheat development) and defensive exploration (anti-cheat evasion and detection logic).

---

## 🖼️ Screenshots

Visual previews of Aspasia in action, demonstrating various features such as aimbot locking, chams rendering, and bone overlays.

> **Note**: All captures were taken in controlled, offline environments for research and demonstration purposes only.

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608222549_1.jpg" alt="Target Bone Visualization" width="700"/>
</p>
<p align="center"><em>Bone targeting visualization used for precise aimbot locking.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/Bones/20250608215858_1.jpg" alt="Entity Bone Overlay" width="700"/>
</p>
<p align="center"><em>Bone overlays rendered for all entities during runtime analysis.</em></p>

<p align="center">
  <img src="Master Thesis Template/images/chams/chams_demo/20250608210828_1.jpg" alt="Chams Demonstration" width="700"/>
</p>
<p align="center"><em>Custom chams materials demonstrating visibility states and entity highlighting.</em></p>

---

## ⚠️ Disclaimer

This project is intended **solely for research and educational purposes**. The authors do not endorse or support cheating in multiplayer games. You are fully responsible for how you use this software.
---

## ✨ Features

- 🎯 **Aimbot System**
  - Target bone selection
  - Smooth aim & FOV limit
  - Configurable hit zones

- 👀 **Visual Enhancements (Chams)**
  - Custom material generation and injection
  - Full engine-based wall visibility
  - Configurable entity filters

- 🧠 **Anti-Aim & Desync Logic**
  - Lower Body Yaw (LBY) breaker
  - Anti-resolve techniques
  - Local animation desync

- 🧵 **Hooking Framework**
  - MinHook-based trampoline hooks
  - Mid-function patching and detouring
  - Inline patch protections

- 🧩 **Entity Analysis**
  - Memory traversal for entity lists
  - OnAddEntity / OnRemoveEntity hook analysis
  - Real-time entity state tracing

- 📈 **Movement & Physics Research**
  - Strafe optimization
  - Bhop and acceleration curves
  - Inertia and friction modeling

---

## 🔨 Compilation

**Requirements:**
- Windows 10/11 x64
- Visual Studio 2019+ with C++ support
- DirectX SDK (June 2010)
- ImGui
- MinHook

**Steps:**
```sh
git clone https://github.com/Aspasia1337/Aspasia
cd Aspasia
```

- Open `SourceHook.sln` in Visual Studio
- Set configuration to `Release x64`
- Build the solution — this will generate `Aspasia.dll`

---

## 💉 Injection


Aspasia is injected via a **custom DLL injector** (not provided). Standard `LoadLibrary` injection is insufficient due to VAC protections. Use:
- Manual mapping or kernel-assisted injection
- Custom shellcode loaders or manual PE loaders

> 💡 Bypassing Steam alone is not sufficient. Since CS2, there are numerous in-game checks and internal detection vectors that must also be bypassed. Many of the more aggressive Steam VAC scans only activate after suspicious behavior is detected internally. For instance, if you notice VAC validating read-only sections of game modules, it is likely too late — your process has already been flagged or marked as suspicious.

---

## 🎯 Research Goals

Aspasia is more than just a cheat — it serves as a dual-purpose tool for:
- Developing proof-of-concept exploits
- Analyzing anti-cheat surface areas and countermeasures
- Teaching internal game structure reverse engineering

It highlights how modern cheats are built and how anti-cheat systems attempt to detect and mitigate them.

---

## 📌 Academic Contributions

This project is part of a Master's Thesis and illustrates:
- Offensive security applied to game environments
- Defensive response modeling
- Secure memory traversal and hook safety

---

## 📚 Thesis Document

This repository is part of a broader academic submission. In addition to the code and technical content included here, the full thesis document—hosted in the `Master Thesis Template` directory—provides a comprehensive overview of the context, objectives, and implications of this work.

The document offers:
- A detailed explanation of the project's goals and methodology
- A historical and technical overview of cheating in online games
- A critical analysis of modern anti-cheat systems and evasion techniques


You can access the full thesis here: [memoria.pdf](https://github.com/Aspasia1337/Aspasia/blob/main-ui/Master%20Thesis%20Template/memoria.pdf)

Sharing this repository publicly reflects the intention that it may serve as a learning resource for others, beyond being just a personal academic project or anecdote. The goal is to give back to the same community that supported the development of this work—drawing from knowledge shared in forums like UnknownCheats, GuidedHacking, and many others that have contributed to advancing reverse engineering and applied security research.

---

## 📬 Contact & Contributions

We welcome academic contributions and discussions on game reverse engineering. Open an issue or fork the project to propose improvements.

---

## 🧾 License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

This means you are free to:
- **Use** the code for any purpose, including academic, personal, or educational use.
- **Modify** and **distribute** the source code and binaries under the same license.
- **Study** and **learn** from the implementation, as long as any derivative work remains open-source and licensed under GPL-3.0 as well.

The GPL-3.0 license ensures that all contributions remain free and accessible, encouraging collaboration and transparency in research and development.

See the full license text in the [`LICENSE`](./LICENSE) file.

---

**Aspasia** — Reverse engineering Counter-Strike 2 from both sides of the battlefield.
