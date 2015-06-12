

Doppelganger
------------

A simple C engine specifically tailored for my own hobby project.

![Alt text](/resources/familiars.jpg?raw=true "Witch and her familiars")


At the basis it follows some principles from 'Handmade Hero';
- live coding loop.
- all memory is pre-allocated and at fixed adresses.

On top of that it's a SDL backed engine based on the 'Lazy Foo tutorials'.


Dependancies:
-------------
SDL2, SDL2_image
linux:
    inotify-tools
osx:
    fswatch



Notes to self
-------------

Compiling in Emacs (while working in the source folder):
`make all --directory=../`
