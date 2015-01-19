# HonkeyPong
Homebrew NES Pong clone, designed as a lesson on programming NES games.
___

##Why?
  I love NES homebrew. The fact that new NES games are still being made 30 years after the console's
release fills me with glee. When I see a new game that interests me, I buy it. The problem is, I 
don't see as many interesting new games as I would like. So, as a service to aspiring NES programmers, 
and as a service to myself to sharpen my own programming skills, I spent a week slapping together 
this silly little Pong clone.

  It seems to me that the biggest barrier to entry for most developers is the difficulty in learning
assembly language. After this, the next barrier seems to be the seemingly conflicting views in all
the tutorials about what assembler to use. Generally, NES dev tutorials seem to center around three 
different assemblers: CA65, NESASM, or ASM6. Sure, these tools are perfectly usable for making NES
games, but various things about each of them bother me, as well as the batch file-based build system
that most NES programmers I've seen seem to adhere to. Why are we still stuck in the 1990s here?

  I wanted to illustrate a more modern, and what I believe to be user-friendly way to develop retro
games, for people who are used to modern programming techniques. The software is built with GNU Make,
and assembled with the Ophis assembler. If you're used to building Linux software, or Windows software
using MinGW/MSYS, you should feel right at home. I have also included a Python tool for preparing 
EPROM images that you can use to write the game to a flash cartridge, and play it on a real NES.

  One of these days, I hope to get around to writing an actual tutorial explaining further how all
of this stuff works, but in the meantime, I've tried to heavily comment the source code and explain
what each block does, and why it works. To better understand the workings of the NES hardware, and
specifics of how to program the NES, I highly recommend checking out the "Nerdy Nights" tutorials
on NintendoAge. These can be read here: http://nintendoage.com/pub/faq/NA/nerdy_nights_out.html
___

##Tools Used
###Ophis 6502 Assembler
http://michaelcmartin.github.io/Ophis/

  Unlike some assemblers used in NES development, Ophis is not geared toward any machine specifically,
but for any machine with a 6502/65C02 processor or similar variant. It can be used to write programs
for the NES, Famicom, Atari 2600, Commodore 64, Apple II and other computers and consoles. It has
plenty of useful features, like building macros, anonymous labeling, exporting maps of your source
code, and much more. It makes retro software development much less of a headache.

###Python
https://www.python.org/

  If you don't know what Python is, SHAME ON YOU! (Just kidding... almost.) Python is a scripting
language commonly used for cross-platform and rapid application development. It's an interpreted
language, and as such will run on any platform that has an interpreter. This coupled with its great
string manipulation abilities make it a great tool for quickly developing small programs that can
run on many platforms with little to no modification that may be more time-consuming in a language
like C. The Ophis assembler is written in Python, as well as the tools I have written for working
with NES ROM files and source code.

###YY-CHR
http://www.romhacking.net/utilities/119/

  YY-CHR is a program for creating and editing graphics for many different consoles, NES included.
It might not be very user-friendly, and it doesn't have the best graphic editor, but it can be handy
due to the fact that you can copy and paste graphics from other software in it. If you mouse around
on the unlabeled checkboxes below the CHR viewer, you can find one with an "Optimize Palette" tooltip.
Check that, then you can paste an indexed-color image from a program like GIMP.
Protip: Make one tile that just has your 4-color palette as the first entry to make the palette 
optimization better when you paste. Check "res/gfx/pongtiles.xcf" for an example.

###GIMP
http://www.gimp.org/

  Ahh, GIMP. The GNU Image Manipulation Program. I'm not going to BS you and say this is the
"Free Photoshop" like lots of people do, but I will admit that it's a damn nice program for working
with graphics in nearly any format. This and Inkscape are the only graphics programs I use on my
laptop. Pretty versatile, loaded with features, and it's free. What more can be said?

###PlayPower Nametable Tool
http://fsp.fm/wrk/playpower/

  Unlike the other tools used here, this one is web-based. It's basically an all-in-one editor
for NES graphics, palettes and maps written in Flash. There's also a desktop version, or if you're
lazy like me, you can use Google Chrome to create desktop app shortcuts for it. Good for building
palettes (colors seem more accurate than YY-CHR, plus it puts them in the right format), and
complex map screens like title screens. This was used to create the title screen for HonkeyPong.

###MSYS
http://www.mingw.org/wiki/MSYS

  If you're on Windows, you're probably going to want this. It provides a minimal system for
building Windows applications in a Unix-like environment. At the very least, you'll probably
want BASH and GNU Make for building this game, so you can use Makefiles instead of batch files.

###Nestopia
http://nestopia.sourceforge.net/

  A cycle-accurate NES emulator. Requires at least an 800MHz CPU to run, but it's much more
accurate than a lot of other NES emulators. If something works in this, there's a good chance 
it'll work on a real NES as well, though an emulator should never be a substitute for testing
on real hardware.

###Nintendulator
http://sourceforge.net/projects/nintendulator/

  Another highly-accurate NES emulator. Unlike Nestopia, this one includes a debugger. I don't
really like the renderer, so I test with Nestopia, and only use this when I need to debug something.
It's Win32-only though, so if you're on Linux or a Mac, you're better off debugging wth FCE Ultra.
I recommend Nestopia over FCE Ultra for testing though, because even though FCEU is highly
compatible, it's not always accurate. Fire up Top Gun to see what I'm talking about.
___

##FAQ
###Will this run on a real NES?
  While I haven't tested it on real hardware yet, I'm pretty sure it'll work fine. This is a 
Mapper 0 (NROM) game, meaning it uses no mapper hardware at all. I don't have any NROM-compatible 
flash hardware, so I haven't been able to test. It works fine in Nestopia, Nintendulator, and 
FCEUX though. After I finish the sound engine, I will also be porting this game to use the MMC3 
chip (Mapper 4), and I do have flash hardware that uses MMC3, so I'll test it on real hardware then. 
Of course, I will be publishing source code for that version as well, so more people can learn 
how to use mappers to make bigger, more complex games. Once I test the MMC3 version on my NES, 
I will make note of anything that doesn't work on the real thing, and fix it in this version.

###How do I build this?
  On a Windows PC, some kind of GNU Make system should be in place, and a BASH environment would
help as well. Personally, I use MSYS on my Windows machines. Python and Ophis are also required 
for assembling the code and using the included tools (NESPrep is the only tool included with this
distribution, other tools will be added in subsequent releases). For the testing and debugging
targets, you should also have Nestopia and Nintendulator in your path. These are optional though,
and you can always just load the assembled ROM in your emulator of choice. The important things
to remember are that Ophis, Make, MSYS and Python are in your system path.

  On Linux and Mac OS X, GNU Make and BASH should already be in place, so you just need to make
sure you have Ophis and Python installed and in your system path. Again, Nestopia is also useful
for the testing targets. For the debug target, you should replace Nintendulator with FCEUX, since
Nintendulator is Win32-only. If you're testing on a low-power system like the Raspberry Pi, you
can also substitute Nestopia for an emulator like FCEUX or RetroArch with one of the FCEU cores.
I can't vouch for the compatibility or accuracy of using other emulators, but I don't do anything
too crazy with the NES hardware here, so I can't see them causing any problems.

**ANYWAY...** once you've made sure all the prerequisites are in place, building the game is as simple
as opening the HonkeyPong directory in a terminal and typing the following:

>make

That's it. That'll build a ROM image in iNES format, that can be used with almost any NES emulator.
If you want to build a ROM and load it in Nestopia for testing, just type:

>make test

If you're hacking away at the code and run into bugs, you can build the game and load it in a debugging
emulator with the following command:

>make debug

By default, this will load the game in Nintendulator. If you're on Mac OS X or Linux, you will want
to update the makefile to load FCEUX instead. The debug target will also generate a HonkeyPong.map
file in plaintext format containing addresses of every variable, data block and subroutine address
so you know where to set breakpoints and watch memory while debugging.

If you want to prepare the game for programming to an actual cartridge, there are two targets for
doing so. The first will create a 16KB PRG file and the 8KB CHR file. This is for standard NROM-128
boards.

>make cartridge

Once this is done, you should have two additional files in your 'bin' folder. HonkeyPong.nes.PRG is
the file you should flash to the PRG-ROM, and HonkeyPong.nes.CHR is the graphics data, which should
be written to the CHR-ROM. If you have one of the INL-ROM NROM flashcarts, or another NROM-256 cartridge,
use the following command instead:

>make cartridge-256

This will create the same 8KB CHR-ROM image as HonkeyPong.nes.CHR, but will create a larger version
of the PRG-ROM, called HonkeyPong.nes.PRG.bin, 32KB (256 kilobits) in size by doubling up the PRG-ROM
data to fill the larger 32KB ROM chip. Now you're ready to program your cartridge and play the game
on a real NES.

Finally, to clean up all the compiled binaries, debug data, and other stuff generated at assemble-time:

>make clean

In general, it's a good idea to do this any time you're about to rebuild, to ensure that any stale
data doesn't get left over to interfere with testing, debugging, etc.

###Can I borrow some of your code to use in my own game?
  Yes. All the code and tools I've included here are made available under the MIT License.
If you're unfamiliar with the terms, the gist of it is that you can use this code however
you want, even in a commercial title that you want to sell. If you want to use the headers
I made to address the NES hardware easily, or modify them, go ahead. The only thing you 
can't do is hold me responsible if it blows up your NES, Famicom, PC, TV, cell phone, tablet, 
cat, or toaster. That (probably) won't happen though. There's no warranty, use at your own risk.

###How can I put this game on a cartridge and play it on a real NES?
  There are several ways to do this. Memory card-based devices like the Everdrive and Powerpak
are fairly popular, but also extremely expensive. The plus sides are that you can use an SD
or CompactFlash card to store many games, and they support a ton of different mappers.
This type of solution seems to be geared more toward gamers who just want a ton of games on
a single cartridge, as opposed to developers with a rapid testing cycle.

Everdrive N8: http://www.stoneagegamer.com/flash-carts/everdrive-n8-fc-nes/north-america-europe-nes/

Powerpak: http://www.retrousb.com/product_info.php?cPath=24&products_id=34


  If you're like me, you'd probably rather have a cheaper flash device that can be reprogrammed
quickly and easily from a PC, and don't care about just loading up a bunch of games and playing
them. This is where dedicated flash cartridges come in. I highly recommend the INL-ROM flash
boards from Infinite NES Lives. I own one of his MMC3 flash boards, as well as the SNES Hi/LoROM
flash board, and have never had any problems with either unit. If you just want a basic NROM
board for making small games, you can buy one for about $13. He also sells new NES plastic
cartridge shells starting at $5, so you can make your dev cart look cool and not cannibalize
an original NES game for parts. If you want something like a larger 6 megabit MMC3 battery
backed flash board, one can be had for $26. That's a damn sight cheaper than a Powerpak.

  Paul's boards are programmable via the INL-Retro USB programmer, available for $20 in a 
one-slot version, with up to two additional slots available for $5 each. If you just want NES, 
it's $20. If you want NES and SNES (or Famicom) it's $25. If you want all three (NES + SNES + 
Famicom), it'll only cost you $30. Again, it's much cheaper (and modern) than the old-school 
expensive parallel port and floppy-disk based flash devices and copiers.

INL-Retro USB Programmer/Dumper: http://www.infiniteneslives.com/aux3.php

NES Cartridge Production Supplies: http://www.infiniteneslives.com/aux4.php

___

##Credits

###Graphics
"Boxy Bold" pixel font by Clint Bellanger and usr_share
Licensed under Public Domain: http://opengameart.org/content/boxy-bold-font-0

Game tiles and screen arrangements by Ryan Souders (HonkeyKong)
http://www.honkeykong.org/

###Programming
Inspired by "Nerdy Nights" by Brian Parker (BunnyBoy)
http://nintendoage.com/pub/faq/NA/nerdy_nights_out.html

Ophis port, game programming and tool development by Ryan Souders (HonkeyKong)
http://www.honkeykong.org/