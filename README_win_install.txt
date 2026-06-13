this txt document will walk you through installing the required tools and libraries to 
build the "ghost" program. eventually i'll make a standalone .exe (or at least an 
installer), but for now you'll have to deal with this mess. sorry lmao

install MSYS2, the terminal we will use to download and compile the program. when 
we're done, we'll uninstall this, and it will uninstall everything (except the ghost) 
with it.
- go to https://www.msys2.org/
- get the x86_64 installer (msys2-x86_64-YYYYMMDD.txt)
- run the installer, leave all settings as default.
- launch "MSYS2 UCRT64" terminal, if it doesn't open automatically. you may want to pin this to the taskbar, it will close and reopen a few times!

in the terminal, run the following command to download the installer:
wget https://raw.githubusercontent.com/OnLinedPaper/ghost/refs/heads/main/bootstrap.sh

run the following command to start the installer (you may have to run it twice)
. bootstrap.sh
