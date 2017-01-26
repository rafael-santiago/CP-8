# CP-8

This code is a work in progress and yes, it is about a ``CHIP-8`` emulator that uses ``ANSI/TERM`` stuff to emulate
the screen output.

Well, here follows some ``CHIP-8 ROMs`` emulated with this software:

|                |                  |                  |
|:--------------:|:----------------:|:----------------:|
|  **INVADERS**  |    **TIC-TAC**   |   **WIPE-OFF**   |
|![INVADERS](https://github.com/rafael-santiago/CP-8/blob/master/etc/invaders.jpeg)|![TIC-TAC](https://github.com/rafael-santiago/CP-8/blob/master/etc/tictac.jpeg)|![WIPE-OFF](https://github.com/rafael-santiago/CP-8/blob/master/etc/wipeoff.jpeg)|
|   **MERLIN**   |      **UFO**     |     **TANK**     |
|![MERLIN](https://github.com/rafael-santiago/CP-8/blob/master/etc/merlin.jpeg)|![UFO](https://github.com/rafael-santiago/CP-8/blob/master/etc/ufo.jpeg)|![TANK](https://github.com/rafael-santiago/CP-8/blob/master/etc/tank.jpeg)|
|   **TETRIS**   |     **HIDDEN**   |    **MISSILE**   |
|![TETRIS](https://github.com/rafael-santiago/CP-8/blob/master/etc/tetris.jpeg)|![HIDDEN](https://github.com/rafael-santiago/CP-8/blob/master/etc/hidden.jpeg)|![MISSILE](https://github.com/rafael-santiago/CP-8/blob/master/etc/missile.jpeg)|

## How to clone this repo?

Well, I use some ``git-submodules`` here so:

```
watson@BakerStreet221B:~/src/clues/rafael-santiago# git clone https://github.com/rafael-santiago/CP-8
watson@BakerStreet221B:~/src/clues/rafael-santiago# cd CP-8
watson@BakerStreet221B:~/src/clues/rafael-santiago/CP-8# git submodule update --init
```

If the incantations above are trickier for you, try:

```
watson@BakerStreet221B:~/src/clues/rafael-santiago# git clone \
> https://github.com/rafael-santiago/CP-8 --recursive
```

Done.

## How to build it?

My code my rules. I use my own [build system](https://github.com/rafael-santiago/hefesto). Once ``Hefesto`` well-installed and working
on your system you should move to CP-8's ``src`` sub-directory and invoke ``hefesto`` from there... Something like:

```
watson@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8# cd src
watson@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# hefesto
(...)
watson@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# _
```

After invoking "hefesto" you will get the binary ``cp8`` under ``../bin`` sub-directory.

## How to install/uninstall it?

Firstly all should be done under ``src`` sub-directory and then...

### Install

```
watson@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# hefesto --install
```

### Uninstall

```
watson@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# hefesto --uninstall
```

Answer the questions confirming your choice and you will get this software installed on your environment.

## How to use it?

This application works based on tasks. So you need to inform the ``task`` and its ``--arguments=(...)``. Each task
requires its own specific arguments.

Until now I only have implemented the ``emulate`` task. Pretty obvious, you will use it to play the ``CHIP-8`` games on
the following way:

```
sherlock@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# ../bin/cp8 emulate \
> --rom=/home/sherlock/rom/PUZZLE
```

This emulation task does not re-map any key. However, you have some special keys, look the **Table 1**.

|     **Key**         |              **Action**              |
|:-------------------:|:------------------------------------:|
|      ``R``          |  Reset/Restart the ``ROM`` execution |
|      ``P``          |  Pause the ``ROM`` execution         |
|      ``ESC``        |  Exit the program                    |

## Additional tasks

In spite of being mainly a ``CHIP-8`` emulator I have written some minor tasks for this software.
Here in this section you can find information about them.

### The umount task

This task should be useful to anyone who wants to probe the code of a ``CHIP-8 ROM``. Using it you are able
to disassemble the machine code.

The usage is pretty straightforward:

```
sherlock@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# cp8 umount \
> --rom=/home/sherlock/rom/PUZZLE
```

The default output is the ``stdout`` if you want to spit the assembly mnemonics to another file:

```
sherlock@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# cp8 umount \
> --rom=/home/sherlock/rom/PUZZLE --out=Elementary.c8
```

A "umounted" ``ROM`` looks like it:

```assembly
LD VA, 0x12
LD VB, 0x01
LD V1, 0x10
LD V2, 0x00
LD V0, 0x00
LD I, 0x2b0
DRW V1, V2, 0x7
LD F, V0
SE V0, 0x00
DRW VA, VB, 0x5
ADD V1, 0x08
ADD VA, 0x08
SE V1, 0x30
JP 0x224
LD V1, 0x10
ADD V2, 0x08
LD VA, 0x12
ADD VB, 0x08
LD I, 0x300
ADD I, V0
LD [I], V0
ADD V0, 0x01
SE V0, 0x10
JP 0x20a
LD VA, 0x12
LD VB, 0x01
LD VC, 0x00
LD V2, 0xff
RND V0, 0x06
ADD V0, 0x02
CALL 0x252
(...)
```

As you can see the ``CHIP-8 Assembly`` is pretty simple with a short processor's instruction set.

If you started to write code when people usually had to compile, worry about memory, program size, etc... You may know that is
not a good idea let a data chunk in the middle of a instruction chunk... Due to it this disassembler skips the first data
chunk when the first instruction is a jump. I said "data chunk" because in the old days was common you pick up a ``ROM`` with
some copyright disclaimer before the real program "text". Then a ``ROM`` that mixes data and instructions at random in any
place will produce a "screwed up" output. However, it is considered a bad practice and uncommon because it could be dangerous
in that days...

## Issues, limitations, known bugs, raptors, etc, .*

((( Coming soon... work in progress... )))
