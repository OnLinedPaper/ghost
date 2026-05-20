this txt document will walk you through installing the required tools and libraries to 
build the "ghost" program. eventually i'll make a standalone .exe (or at least an 
installer), but for now you'll have to deal with this mess. sorry lmao

install MSYS2, the terminal we will use to download and compile the program. when 
we're done, we'll uninstall this, and it will uninstall everything (except the ghost) 
with it.
- go to https://www.msys2.org/
- get the x86_64 installer (msys2-x86_64-YYYYMMDD.txt)
- run the installer, leave all settings as default
- launch "MSYS2 UCRT64" terminal, if it doesn't open automatically. you may want to pin this to the taskbar, it will close and reopen a few times!

install the compiler into the terminal, and run these commands, one at a time. some 
of these commands will ask you to close and reopen the terminal once they're done 
running.
- pacman -Suy  # updates the rest of the things we will install. this will restart the terminal, so be sure you know how to reopen it.
- pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja git  # installs the required tools
- export PATH="$PATH:/c/msys64/mingw64/bin/"  # tell the terminal where to find the software we installed

download the code for ghost, and the libraries it needs to run
- git clone https://github.com/OnLinedPaper/ghost.git  # gets the ghost program's source code
- cd ghost/external && wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.4.8.tar.gz  # gets the source code of a library we need and put it into the folder where we need it
- tar -xzvf release-3.4.8.tar.gz && cd SDL-release-3.4.8  # unpacks the library's source code and prepares to compile it

compile the libraries ghost needs to run. this segment is taken 
from https://wiki.libsdl.org/SDL3/README-windows and can be followed there, if you prefer!
- mkdir build  # prepares a place to build the library
- cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-x86_64.cmake  # prepares to build the library
- cmake --build build --parallel  # builds the library
- cmake --install build --prefix .. && cd ../..  # installs the library to ghost

compile the ghost program and run it!
- rm -rf build/ && cmake -DCMAKE_PREFIX_PATH=./external/lib/cmake/SDL3/ -B build  # prepares to build ghost
- cmake --build build  # builds ghost
- cp external/bin/SDL3.dll .  # adds the library ghosts needs
- start build/ghost.exe  # runs ghost!

OPTIONAL: move ghost to your desktop - WORK IN PROGRESS! ghost can be moved without 
issue, but if MSYS2 is uninstalled or SDL3.dll is not moved with it, it breaks. 
currently working on this dependency issue.
- cp build/ghost.exe /c/Users/YOURUSERNAME/Desktop  # copies ghost to your desktop 
- cp external/bin/SDL3.dll /c/Users/YOURUSERNAME/Desktop  # copies the library to your desktop
- you can now move ghost.exe wherever you want! just make sure to put SDL3.dll in the same folder. i'm working on a way to get around this dependency, but for now, keep them together so ghost doesn't get lonely

OPTIONAL: uninstall everything else.
- go to your uninstaller ("Installed Apps" on windows 11)
- uninstall MSYS2, which will also uninstall all the other stuff you just installed... including ghost, if you haven't moved it to your desktop yet! use defaults for the uninstaller.
