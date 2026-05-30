#!/bin/bash

# this script bootstraps the installation of the ghost project. it is meant
# to be run after the terminal is downloaded and made ready. it is eventually
# going to be cross-platform compatible, but is being tested on Windows 11 to
# start. 
#
# note that this script redirects about all of its output to a log file.
# this is by design; non-techincal users don't need to see it.

logfile="./.ghost_installer.log"
echo "----" >>${logfile}

#---- welcome messages --------------------------------------------------------

function print_welcome() {
  if [ -e .bootstrap.skipwelcome ]; then
    return
  fi
  echo "This script will perform a complete"
  echo "installation of the Ghost program, from"
  echo "start to finish."
  echo ""
  echo "Please pin this terminal program to your"
  echo "taskbar - it will close a few times, and"
  echo "you will need to reopen it manually."
  echo ""
  echo "The installation will take about 10-15"
  echo "minutes as the code and its supporting"
  echo "libraries are downloaded and compiled."
  echo "(I will eventually make a standalone"
  echo "ghost.exe, but for now, this is all I"
  echo "can do... Windows programming is"
  echo "tricky.)"
  echo "Thank you for your support!"
  echo ""
  touch .bootstrap.skipwelcome
}


#---- updates and downloads ---------------------------------------------------

function update_pacman() {
  echo ""
  echo -n "updating pacman (this step might close the terminal)... "
  pacman -Suy -q --noconfirm >>${logfile} 2>&1
  echo "done"
}

function install_tools() {
  echo ""
  echo "installing tools... "

  echo -n "- gcc..... "
  pacman -S -q --needed --noconfirm mingw-w64-ucrt-x86_64-gcc >>${logfile} 2>&1
  echo "done"

  echo -n "- cmake... "
  pacman -S -q --needed --noconfirm mingw-w64-x86_64-cmake >>${logfile} 2>&1
  echo "done"

  echo -n "- ninja... "
  pacman -S -q --needed --noconfirm mingw-w64-x86_64-ninja >>${logfile} 2>&1
  echo "done"

  echo -n "- git..... "
  pacman -S -q --needed --noconfirm git >>${logfile} 2>&1
  echo "done"

  export PATH="$PATH:/c/msys64/mingw64/bin/"
}

function clone_ghost() {
  echo ""
  echo -n "downloading ghost source code... "
  rm -rf ./ghost
  git clone https://github.com/OnLinedPaper/ghost.git
  echo "done"
}

function download_libraries() {
  echo "" 
  echo "downloading supporting libraries..."
  
  echo -n "SDL......... "
     wget -nv https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.4.8.tar.gz >>${logfile} 2>&1
  echo "done"

  echo -n "SDL_Image... "
    wget -nv https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-3.4.4.tar.gz >>${logfile} 2>&1
  echo "done"

  mv release-3.4.8.tar.gz ghost/external
  mv release-3.4.4.tar.gz ghost/external
}

function unpack_libraries() {
  echo ""
  echo -n "unpacking supporting libraries... "
  cd ghost/external
  tar -xzvf release-3.4.8.tar.gz >>${logfile} 2>&1
  tar -xzvf release-3.4.4.tar.gz >>${logfile} 2>&1
  echo "done"
  cd ../..
}


#---- compilation -------------------------------------------------------------

function build_libraries() {
  echo ""
  echo "compiling supporting libraries... (this will take a while)"
  cd ghost/external  

  echo "SDL... "
    cd SDL-release-3.4.8
    mkdir build

    echo -n "- config.... "
    `cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-x86_64.cmake` >>${logfile} 2>&1
    echo "done"

    echo -n "- compile... "
    `cmake --build build --parallel` >>${logfile} 2>&1

    echo "done"

    echo -n "- install... "
    `cmake --install build --prefix ..` >>${logfile} 2>&1

    echo "done"
    cd ..

  echo "SDL_Image... "
    cd SDL_image-release-3.4.4
    mkdir build

    echo -n "- config.... "
    `cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-x86_64.cmake -DCMAKE_PREFIX_PATH=../lib/cmake/SDL3` >>${logfile} 2>&1

    echo "done"

    echo -n "- compile... "
    `cmake --build build --parallel` >>${logfile} 2>&1

    echo "done"

    echo -n "- install... "
    `cmake --install build --prefix ..` >>${logfile} 2>&1

    echo "done"
    cd ..
  cd ../..
}

function build_ghost() {
  echo ""
  echo "compiling ghost..."
  cd ghost

  echo -n "- config.... "
  `cmake -B build` >>${logfile} 2>&1
  echo "done"

  echo -n "- compile... "
  `cmake --build build` >>${logfile} 2>&1
  echo "done"

  echo -n "- install... "
  cp external/bin/SDL3.dll .
  cp external/bin/SDL3_image.dll .
  echo "done"
  cd ..
}


#---- actual script -----------------------------------------------------------

echo ""
echo "---- Welcome to the Ghost installer! ----"
echo ""
 
print_welcome

echo "Press \"Enter\" to proceed with the"
echo -n "installation. > "
read tmp

update_pacman
install_tools
clone_ghost
download_libraries
unpack_libraries
build_libraries
build_ghost

cd ghost
echo ""
echo ""
echo "Done! You may now run ghost by typing:"
echo "  start build/ghost.exe"
