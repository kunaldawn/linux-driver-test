linux-device-driver devl base
======================
Linux device driver test code repo.

### 1. null character driver

A character driver which acts as /dev/null { Black Hole of linux O.o }

##### Build
kunaldawn@kunaldawn-VirtualBox:~/lindrivertest$ make
##### Load
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest$ cd null_character_driver/
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ ls
- null_character_driver.c  null_character_driver.ko  null_character_driver.mod.c  null_character_driver.mod.o  null_character_driver.o
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ sudo insmod ./null_character_driver.ko 
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ lsmod | grep null
- null_character_driver    12621  0 
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ dmesg | grep "NULL DRIVER"
- [ 3222.550547] NULL DRIVER : INIT REQUEST
- [ 3222.551702] NULL DRIVER : INIT SUCCESS
##### Test
- kunaldawn@kunaldawn-VirtualBox:~$ sudo su
- [sudo] password for kunaldawn: 
- root@kunaldawn-VirtualBox:/home/kunaldawn# cd /dev
- root@kunaldawn-VirtualBox:/dev# ls | grep "nullc"
- nullcdev0
- root@kunaldawn-VirtualBox:/dev# echo "go to black hole" > nullcdev0 
- [ 3222.550547] NULL DRIVER : INIT REQUEST
- [ 3222.551702] NULL DRIVER : INIT SUCCESS
- [ 3363.595007] NULL DRIVER : OPEN CALL
- [ 3363.595050] NULL DRIVER : WRITE CALL
- [ 3363.595059] NULL DRIVER : CLOSE CALL
- root@kunaldawn-VirtualBox:/dev# cat nullcdev0 
- root@kunaldawn-VirtualBox:/dev# dmesg | grep "NULL DRIVER"
- [ 3222.550547] NULL DRIVER : INIT REQUEST
- [ 3222.551702] NULL DRIVER : INIT SUCCESS
- [ 3363.595007] NULL DRIVER : OPEN CALL
- [ 3363.595050] NULL DRIVER : WRITE CALL
- [ 3363.595059] NULL DRIVER : CLOSE CALL
- [ 3458.028640] NULL DRIVER : OPEN CALL
- [ 3458.028668] NULL DRIVER : READ CALL
- [ 3458.028676] NULL DRIVER : CLOSE CALL
##### Unload
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ sudo rmmod null_character_driver 
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/null_character_driver$ dmesg | grep "NULL DRIVER"
- [ 3222.550547] NULL DRIVER : INIT REQUEST
- [ 3222.551702] NULL DRIVER : INIT SUCCESS
- [ 3363.595007] NULL DRIVER : OPEN CALL
- [ 3363.595050] NULL DRIVER : WRITE CALL
- [ 3363.595059] NULL DRIVER : CLOSE CALL
- [ 3458.028640] NULL DRIVER : OPEN CALL
- [ 3458.028668] NULL DRIVER : READ CALL
- [ 3458.028676] NULL DRIVER : CLOSE CALL
- [ 3518.439426] NULL DRIVER : UNLOADED

### SETUP ECLIPSE FOR DRIVER DEVELOPMENT IN UBUNTU
Here are the steps to remove annoying 
- Download linux source <sudo apt-get install linux-source>
- Download Eclipse CDT from <https://www.eclipse.org/cdt/>
- Create a folder for project say "~/linuxdrivers"
- Create a c file and a Makefile (See null driver example)
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

### LICENSE : GPLv3, 29 June 2007 
linux-device-driver
Copyright (C) 2014  Kunal Dawn <kunal.dawn@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
