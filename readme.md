
Doppelgangers
-------------

A simple C engine specifically tailored for my own hobby project.



![Alt text](/resources/familiars.jpg?raw=true "Witch and her familiars")



At the basis it follows some principles from 'Handmade Hero';
- live coding loop.
- all memory is pre-allocated and at fixed adresses.

On top of that it's a SDL backed engine based on the 'Lazy Foo tutorials'.


- after fswatch is installed I run a `fswatch-run ./source "make game" ` in the root to auto update the lib on changes made.

Notes
-----

Running compile in emacs, when I am in the source folder and want to build all. The compile command should be:
`make all --directory=../`
