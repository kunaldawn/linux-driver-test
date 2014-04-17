linux-device-driver devl base
======================
Linux device driver test code repo.

### 1. NULL CHARACTER DRIVER

A character driver which acts as /dev/null { Black Hole of linux O.o }

##### Build
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest$ make

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

### 2. LIFO CHARACTER DRIVER 

A character driver which acts like LIFO(Last In First Out)

##### Build
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest$ make

##### Load
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest$ cd lifo_character_driver/
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/lifo_character_driver$ sudo insmod ./lifocdev.ko
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/lifo_character_driver$ lsmod | grep "lifo"
- lifocdev               12780  0
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/lifo_character_driver$ dmesg | grep "LIFO"
- [11212.113016] LIFO DRIVER : LOADING
- [11212.120456] LIFO DRIVER : INIT SUCCESS

##### Test 1
- kunaldawn@kunaldawn-VirtualBox:~$ sudo su
- root@kunaldawn-VirtualBox:/home/kunaldawn# cd /dev
- root@kunaldawn-VirtualBox:/dev# ls | grep "lifo"
- lifocdev0
- root@kunaldawn-VirtualBox:/dev# echo -n 'hello_world' > lifocdev0
- root@kunaldawn-VirtualBox:/dev# dmesg | grep "LIFO"
- [11212.113016] LIFO DRIVER : LOADING
- [11212.120456] LIFO DRIVER : INIT SUCCESS
- [11448.804119] LIFO DRIVER : OPEN CALL
- [11448.804157] LIFO DRIVER : WRITE CALL
- [11448.804163] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:1][SIZE:11][WRITE:11/11]
- [11448.804172] LIFO DRIVER : CLOSE CALL
- root@kunaldawn-VirtualBox:/dev# cat lifocdev0 
- dlrow_olleh
- root@kunaldawn-VirtualBox:/dev# dmesg | grep "LIFO"
- [11212.113016] LIFO DRIVER : LOADING
- [11212.120456] LIFO DRIVER : INIT SUCCESS
- [11448.804119] LIFO DRIVER : OPEN CALL
- [11448.804157] LIFO DRIVER : WRITE CALL
- [11448.804163] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:1][SIZE:11][WRITE:11/11]
- [11448.804172] LIFO DRIVER : CLOSE CALL
- [11525.712954] LIFO DRIVER : OPEN CALL
- [11525.712982] LIFO DRIVER : READ CALL
- [11525.712989] LIFO DRIVER : READ SUCCESS [SIZE:10]
- [11525.715453] LIFO DRIVER : READ CALL
- [11525.715456] LIFO DRIVER : READ SUCCESS [SIZE:9]
- [11525.715472] LIFO DRIVER : READ CALL
- [11525.715473] LIFO DRIVER : READ SUCCESS [SIZE:8]
- [11525.715481] LIFO DRIVER : READ CALL
- [11525.715482] LIFO DRIVER : READ SUCCESS [SIZE:7]
- [11525.715486] LIFO DRIVER : READ CALL
- [11525.715487] LIFO DRIVER : READ SUCCESS [SIZE:6]
- [11525.715491] LIFO DRIVER : READ CALL
- [11525.715492] LIFO DRIVER : READ SUCCESS [SIZE:5]
- [11525.715496] LIFO DRIVER : READ CALL
- [11525.715498] LIFO DRIVER : READ SUCCESS [SIZE:4]
- [11525.715504] LIFO DRIVER : READ CALL
- [11525.715505] LIFO DRIVER : READ SUCCESS [SIZE:3]
- [11525.715512] LIFO DRIVER : READ CALL
- [11525.715514] LIFO DRIVER : READ SUCCESS [SIZE:2]
- [11525.715521] LIFO DRIVER : READ CALL
- [11525.715523] LIFO DRIVER : READ SUCCESS [SIZE:1]
- [11525.715530] LIFO DRIVER : READ CALL
- [11525.715531] LIFO DRIVER : READ SUCCESS [SIZE:0]
- [11525.715539] LIFO DRIVER : READ CALL
- [11525.715540] LIFO DRIVER : NO DATA [SIZE:0]
- [11525.715549] LIFO DRIVER : CLOSE CALL

##### Test 2
- root@kunaldawn-VirtualBox:/dev# dmesg > lifocdev0 
- dmesg: write failed: Bad address
- [11689.685036] LIFO DRIVER : OPEN CALL
- [11689.695264] LIFO DRIVER : WRITE CALL
- [11689.695268] LIFO DRIVER : WRITE SUCCESS [BLOCKS:1][SIZE:1024][WRITE:4096/1024]
- [11689.695269] LIFO DRIVER : WRITE CALL
- [11689.695271] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:2][SIZE:2048][WRITE:3072/1024]
- [11689.695271] LIFO DRIVER : WRITE CALL
- [11689.695273] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:3][SIZE:3072][WRITE:2048/1024]
- [11689.695273] LIFO DRIVER : WRITE CALL
- [11689.695274] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:4][SIZE:4096][WRITE:1024/1024]
- [11689.695487] LIFO DRIVER : WRITE CALL
- [11689.695488] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:5][SIZE:5120][WRITE:4096/1024]
- [11689.695489] LIFO DRIVER : WRITE CALL
- [11689.695490] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:6][SIZE:6144][WRITE:3072/1024]
- [11689.695491] LIFO DRIVER : WRITE CALL
- [11689.695492] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:7][SIZE:7168][WRITE:2048/1024]
- [11689.695493] LIFO DRIVER : WRITE CALL
- [11689.695495] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:8][SIZE:8192][WRITE:1024/1024]
- [11689.695707] LIFO DRIVER : WRITE CALL
- [11689.695708] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:9][SIZE:9216][WRITE:4096/1024]
- [11689.695709] LIFO DRIVER : WRITE CALL
- [11689.695711] LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:10][SIZE:10240][WRITE:3072/1024]
- [11689.695712] LIFO DRIVER : WRITE CALL
- [11689.695712] LIFO DRIVER : WRITE FAIL : MAX_BLOCKS
- [11689.696155] LIFO DRIVER : CLOSE CALL

##### Unload
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/lifo_character_driver$ sudo rmmod lifocdev
- kunaldawn@kunaldawn-VirtualBox:~/lindrivertest/lifo_character_driver$ dmesg | grep "LIFO" | tail -2
- [11689.696155] LIFO DRIVER : CLOSE CALL
- [11846.490903] LIFO DRIVER : UNLOADED

### 3. SETUP ECLIPSE FOR DRIVER DEVELOPMENT IN UBUNTU
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
