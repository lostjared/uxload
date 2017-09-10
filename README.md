# uxload
This program will setup a server where you can load a program
on one system from another. I got the idea from ps3load/wiiload but I made this
so I could test between my ASUS Tinker board and my main system without having to log in to ssh and copy files each time 
with scp.

To compile:

use 

$ ./autogen.sh && ./configure && make

then if you want to install

$ sudo make install


How to use:

on the system to load the program:

open terminal and enter the directory where program should run
put a text file called ip.cfg in that directory and put a list of accepted ip addresses.
run the program with port ot listen on:

$ ./uxload port

on the system to send the program:

open terminal and enter directory where program you want to send is and use

$ ./uxload target port program





