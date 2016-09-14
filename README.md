VAnishing DOmino pRoblem {#mainpage}
========================

Copyright 2012, 2014, 2016 by Mateusz Bysiek, Radosław Łojek, Stanisław Peryt -- in 2012,
students of Faculty of Mathematics and Information Sciences at Warsaw University of Technology.

    This file is part of VaDoR.

    VaDoR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VaDoR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VaDoR.  If not, see <http://www.gnu.org/licenses/>.

About this project
------------------

This is a 3-person team project that we did for Algorithms & Complexity course during our Bachelor
studies. The project introduced the problem starting with a rectangular board filled with domino
pieces, and defined certain rules for removing them. The task was to design and implement an
algorithm that removes (in accordance with rules) as many domino pieces as possible from the board
as quickly as possible. Hence the name VaDoR, from VAnishing DOmino pRoblem.

Originally, a detailed problem description was available on a co-lecturer's page at:
`http://alpha.mini.pw.edu.pl/~lucknerm/downloads/AC/domino.pdf`
but it seems to be no longer there. We provide a backup in this repository.


Technicalities
--------------

We implemented the solver to the above-defined problem that solves it in exponential time, O( 2^n ).
There are also several approximate solvers available, with polynomial time complexity,
O( n^2 ), but the solutution obtained by those is not guaranteed to be optimal of course.

Program accepts two input formats: XML and txt. Format detailes are described in the documentation.

This software depends on Qt. It should work with Qt 4.8, 5.5, 5.7 and most probably with many other
Qt versions. It should work on Ubuntu 14.04, Ubuntu 16.04, Windows 7 and maybe other systems too.

Program uses as much memory as available, so it might need several Gigabytes of RAM for large
problems. For example it analyzed around 15000000 states (possible domino pieces configurations)
before running out of memory on my lap-top which had 2GiB of RAM at the time. In such cases program
clears some of the allocated memory and proceeds. It is still able to produce a correct answer,
however the complexity might increase beyond O( 2^n ) because of some repeated computation.


Where is everything, i.e. structure of the repository
-----------------------------------------------------

You should be able to find all resources needed to fully understand the project in this repository.

* */bin/windows* - windows binaries, compiled with Qt4.8.3 and MSVC2010
* */doc/problem* - description of the problem
* */doc/txt* - description of the accepted txt input file format
* */doc/initial* - initial documentation, description of the problem and the theoretical outline of the solution
* */doc/final* - final documentation, outline of the solution described after implementation
* */doc/doxygen* - documentation of classes
* */res* - resources used for development
* */src* - source code and Qt Creator project files
* */src/approximate_r.cpp* - approximate algorithm by Radosław Łojek


Command-line options
--------------------

To start Qt GUI, execute:

    vador.exe

To run in text mode, execute:

    vador.exe -cmd file.xml/txt

In text mode, you have many more command line options. For example:

    vador.exe -cmd -delay=200 problem3.xml -depthsort -purge

will solve problem3.xml using greedy depth-first search, with purging of useless results in runtime.

For details of all available options, and other examples, please run:

    vador.exe -help


Notes
-----

This file is in Markdown, paste it here: `http://daringfireball.net/projects/markdown/dingus`
if you cannot see formatting and this file is not very readible for you.

