# uxload
This program will setup a server where you can load a program
on one system from another. I got the idea from ps3load/wiiload but I made this
so I could test between my ASUS Tinker board and my main system without having to log in to ssh and copy files each time 
with scp.

To compile requires autoconf, automake, g++

use 

$ ./autogen.sh && ./configure && make

then if you want to install

$ sudo make install


How to use:

on the system to load the program:

open terminal and enter the directory where program should run
put a text file called ip.cfg in that directory and put a list of accepted ip addresses.
run the program with port ot listen on:

$ uxload -l port

You can manually specify the ip config file as shell variable UXLOAD_CFG

on the system to send the program:
First export UXLOAD_NET as a shell variable in bash with IP address of uxload server
with the format ip:port so 

export UXLOAD_NET=192.168.8.145:10241

would be valid.

open terminal and enter directory where program you want to send is and use

$ uxload -s program

![ScreenShot](https://github.com/lostjared/uxload/blob/master/screens/uxload.png?raw=true "screenshot")




