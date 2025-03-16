# Hidden DLL Persistence

## Overview
**Hidden DLL Persistence** is a tool designed to demonstrate how malicious DLLs can be stealthily persisted by leveraging writable directories found in Windows registry startup entries. This technique is commonly used in DLL hijacking and persistence attacks.

## Features
- Scans Windows registry startup entries for executable paths.
- Extracts and cleans directory paths from registry values.
- Identifies writable directories within those paths.
- Copies a malicious DLL into a randomly selected writable directory for persistence.

## How It Works
1. The tool enumerates registry keys from:
   - `HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run`
   - `HKEY_LOCAL_MACHINE\Software\Wow6432Node\Microsoft\Windows\CurrentVersion\Run`
   - `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`
2. Extracts executable paths and determines their parent directories.
3. Checks which directories are writable.
4. Copies the malicious DLL to one of the writable directories.
5. The next time the legitimate application starts, it will load the malicious DLL.

## Usage
```sh
hidden_dll_persistence.exe <malicious_dll_path>
```
Example:
```sh
hidden_dll_persistence.exe C:\Users\Attacker\malicious.dll
```

## Prerequisites
- Windows
- A malicious DLL to be injected (with the corrects imports, check the [official Koppeling repo](https://github.com/monoxgas/Koppeling))

## Disclaimer
**This project is for educational and research purposes only.**
Misuse of this tool for unauthorized access, exploitation, or any malicious intent is illegal. The author takes no responsibility for any misuse or damage caused by this software.

## Author
- **nxvertime** ([GitHub](https://github.com/nxvertime))

## License
This project is released under the [MIT License](LICENSE).
