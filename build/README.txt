This directory contains tools to build spatosc on with Visual Studio Microsoft Windows.

Here is how to do this.

 * Install the pthreads-win32 library using the exe installer.

 * In Visual Studio, path to this header file:
   - pthread.h (from pthreads-Win32)

 * Build liblo for Visual Studio. (first run premake4.exe, as explained in liblo's documentation)

 * Launch a command-line console, cd to this spatosc/build directory and type something like this:
   - C:\Users\USER\spatosc> premake4.exe vs2008
   - Note that if you use a version of Visual Studio that is earlier than 2010, you will need to install the standard TR1 C++ libraries.

 * Next, launch the created ".vcproj" project file with Microsoft Visual Studio 2008, or later. (including the Express editions)

 * You probably want to choose the ReleaseLib configuration. (in the top toolbar) It will build a static library.

 * Build the library

See http://industriousone.com/premake for more info on how to use Premake.

