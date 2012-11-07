VAnishing DOmino pRoblem
========================

This is a solution of the problem defined at:
http://alpha.mini.pw.edu.pl/~lucknerm/downloads/AC/domino.pdf

The above file is mirrored in this repository.

VAnishing DOmino pRoblem -> VADOR


Current main features
---------------------

The program is able to solve the above-defined problem in exponential time: O(2^n). Currently, 
it is able to analyze around 16500000 states (possible domino pieces configurations) before 
running out of memory. In such case, the program provides partial solution.

There is also possibility to solve the problem using approximation. In such case, 
time complexity is polynomial: O(n^2). User can choose one of several approximation methods.

The program accepts two input formats: XML and txt. XML format is described in detail in initial 
documentation, and txt in a separate document. You can find all documentation in the repository. 
Where? See *Where is everything* chapter of this readme file.


Command line options
--------------------

To start Qt GUI, execute:

<code>vador.exe</code>

To run in text mode, execute:

<code>vador.exe -cmd file.xml/txt</code>

In text mode, you have many more command line options. For example:

<code>vador.exe -cmd -delay=200 problem3.xml -depthsort -purge</code>

will solve problem3.xml using greedy depth-first search, with purging of useless results in runtime.

For details of all available options, and other examples, please run:

<code>vador.exe -help</code>


Where is everything
-------------------

* */bin/windows* - windows binaries, compiled with Qt4.8.3 and MSVC2010
* */doc/problem* - description of the problem
* */doc/txt* - description of the accepted txt input file format
* */doc/initial* - initial documentation, description of the problem and the theoretical outline of the solution
* */doc/doxygen* - documentation of classes **(not yet ready)**
* */doc/userguide* - user guide **(not yet ready)**
* */res* - resources used for development
* */src* - source code and Qt Creator project files


License
-------

Copyright 2012. All rights reserved: M. Bysiek, R. Łojek, S. Peryt.
Do not distribute without permission of all of the authors, unless 
a different license is applied for specific source code files.
