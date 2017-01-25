# CP-8

This code is a work in progress and yes, it is about a ``CHIP-8`` emulator that uses ``ANSI/TERM`` stuff to emulate
the screen output.

## How to clone this repo?

Well, I use some ``git-submodules`` here so:

```
watson@BakerStreet221B:~/src/clues/rafael-santiago# git clone https://github.com/rafael-santiago/CP-8
watson@BakerStreet221B:~/src/clues/rafael-santiago# cd CP-8
watson@BakerStreet221B:~/src/clues/rafael-santiago/CP-8# git submodule update --init
```

If the above incantations are trickier for you, try:

```
watson@BakerStreet221B:~/src/clues/rafael-santiago# git clone https://github.com/rafael-santiago/CP-8 --recursive
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

Firstly all should be done under ``src`` sub-directory, them...

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
sherlock@BakerStreet221B:~/src/clues/rafael-santiago/src/CP-8/src# ../bin/cp8 emulate --rom=/home/sherlock/rom/PUZZLE
```

This emulation task does not remap any key.

## Additional tasks

((( Coming soon... work in progress... )))

## Issues, limitations, known bugs, raptors, etc, .*

((( Coming soon... work in progress... )))
