dummy-driver devl base
======================
dummy Linux device driver test code repo.
experimental / does nothing.

- dummy.ko : character driver allows single character read write
- chrdummydriver.ko : character driver allows stream read write

##SETUP ECLIPSE FOR DRIVER DEVELOPMENT IN UBUNTU
Here are the steps to remove annoying 
- Download linux source <sudo apt-get install linux-source>
- Download Eclipse CDT from <https://www.eclipse.org/cdt/>
- Create a folder for project say "~/linuxdrivers"
- create a c file and a Makefile to compile driver as provided in the dummy driver code
- Open eclipse and Create a C project from existing Makefile project and select the folder ~/linuxdrivers
- Open project Properties
- Open the C/C++ General selection on the left
- Click on "Indexer" and enable "Enable project specific settings", in "Indexer options" disable "Index source files not included in the build"
- Click on "Paths and Symbols", select "GNU C" in "Includes" tab, click "Add" and select "File system"
- Add two include directories, for me it was, "/usr/src/linux-headers-3.11.0-19-generic/include" and "/usr/src/linux-headers-3.11.0-19-generic/arch/x86/include"
- Select "Symbols" tab and add new with Name = __KERNEL__ and Value = 1
- Now select "Preprocessor Include Paths, Macros etc.", select "Providers" tab, select "CDT GCC Built-in Compiler Settings", disable "Use global provider share between projects" and add at the end og "Command to get compiler specs:" with "-nostdinc", finally which will look like "${COMMAND} -E -P -v -dD "${INPUTS}" -nostdinc"
- Click apply, ok
- Select Project menu > C/C++ Index > Rebuild
- Ready to go.