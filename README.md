fselect
=======

A full-screen file selector. 
Imagine a menu to `ls -l` : fselect does just this.

Usage
-----

  $ fselect [-hV] [-f viewflags] [-y] [FILES] ...

  -h prompts usage
  
  -V version
  
  -y instead of the menu interface, asks in stdout if you wish to select 
     a file at a time

  -c use curses/ncurses selection mode

  -o select only one file 

File parameters are those you'd pass to `ls`, mostly expanded by the shell.

And it's paired to the posix shell functions where fselect gives the best:

  $ play $(fselect ~/music/*)

Supposing you have audio files in the music folder, you can select the files
you prefer there in order to play them with the sox tool.

Keybindings
-----------

  * Arrow keys (Up/Down)/vi keys (K/J) : Moving up and down in the list of 
    files
    
  * Space/Ins : Marking files to have them printed in standard output 
    exiting `fselect`
    
  * Enter/Esc : exits fselect
  
  * . (Dot) : Marks the file where the cursor is and exits fselect


Installation
------------

Try rename to Makefile one of Makefile.gnu and Makefile.bsd, then launch 
`make` in your terminal.


History
-------

As far as i know fselect is a work of Peter Pentchev. His version in [his site](http://devel.ringlet.net/misc) 
is the 1.0, hasn't got the `.` key and shouldn't compile in
[GNU](https://www.gnu.org/) systems without some minor tweakings.


Contributing
------------

You can make a lot of hacks with fselect, I've to learn C anew so if someone
would like to point me out where do I have to tweak to fix the warnings
I'll thank him/her.

This program has potential, I seek to integrate it with dircolors and to
make some kind of hacked up file manager with it and a shell scripts.
Submit your patches.

(c) 2001-2014 Peter Pentchev, Teodoro Santoni
