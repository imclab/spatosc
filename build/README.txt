This directory contains tools to build spatosc on Microsoft Windows.

 * Launch a command-line console, cd to this directory and type something like this:
   - C:\Users\USER\spatosc> premake4.exe vs2008
   - Note that if you use a version of Visual Studio that is earlier than 2010, you will need to install the standard TR1 C++ libraries.

 * Next, launch the create project with Microsoft Visual Studio 2002, 2003, 2005, or 2008, including the Express editions.

 * You need to add a few libraries to this project.
   - liblo_d.lib (liblo)
   - ws2_32.lib (Win Sock)

 * Make sure you link statically to all libraries.
   - Go to "Configuration Properties > General > Project Defaults > Configuration Type" and choose "Static Library (.lib)"

See http://industriousone.com/premake for more info on how to use Premake.

