# Aspasia Injector

<p align="center">
  <img src="https://img.shields.io/badge/platform-Windows%20x64-blue"/>
  <img src="https://img.shields.io/badge/language-C%2B%2B17-brightgreen"/>
  <img src="https://img.shields.io/badge/method-Manual%20Map-orange"/>
  <img src="https://img.shields.io/badge/target-CS2-red"/>
</p>

Simple manual mapper injector for loading `aspasia.dll` into Counter-Strike 2. Companion tool to the [Aspasia](https://github.com/Aspasia1337/Aspasia) research framework.

---

## How It Works

Manual mapping loads a DLL into a target process without using `LoadLibrary`, leaving no trace in the module list. The injector:

1. Finds `cs2.exe` via `CreateToolhelp32Snapshot`
2. Reads and validates the DLL from disk (PE header check, x64 check)
3. Allocates memory in the target process and maps each PE section
4. Writes a shellcode stub that handles relocations, imports, TLS callbacks, and calls `DllMain`
5. Executes the shellcode via `CreateRemoteThread` and waits for confirmation

---

## Usage

1. Build the solution in `Release x64`
2. Place `aspasia.dll` in the same directory as the injector executable
3. Launch CS2
4. Run the injector as Administrator

If successful, the in-game console will appear in CS2.

---

## Requirements

- Windows 10/11 (x64)
- Visual Studio 2019 or later
- C++17 (`/std:c++17`)
- Administrator privileges
- CS2 running before injection

---

## Notes

- Standard `LoadLibrary` injection is detectable by VAC — manual mapping is required
- `CreateRemoteThread` itself is detectable; this injector is intended for research in controlled environments only
- Part of the [Aspasia](https://github.com/Aspasia1337/Aspasia) Master's Thesis project — see main branch for context
