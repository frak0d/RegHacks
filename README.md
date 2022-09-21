# RegHacks
Some Registry Hacks for Windows 10 (and 11?)

---

![screenshot](screenshot.png)

### Compile Command:
```
xxxx-w64-mingw32-g++ -std=c++20 -s -static -flto RegHacks.cpp -o RegHacks.exe -mwindows -municode -lwinmm -Wno-conversion-null
```
