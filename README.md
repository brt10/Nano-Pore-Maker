pore - Passivating Objective Radial Eliminator
=============================================
This code creates a nanometer hole or pore in a periodic atomic model.

Quickstart
-----------

compile:
`$ make a`   
Run:
`$ ./bin/pore <filename>`   
`<filename>` is a .tsv laid out simularly to the following example:
```
INPUT
	filename	unitcell.vasp
PORE
	coordinate	.5	.5	.5
	radius		2
OUTPUT
	filename	pore
```
This makes a single pore of radius 2 Angstroms in the center of the file named`unitcell.vasp`, and outputs the results to a file named `pore.vasp`.  

Both input and output fles use the format of VASP POSCAR files (see http://cms.mpi.univie.ac.at/vasp/vasp/POSCAR_file.html  for more info ).

Makefile
--------
```
make [options]
	Options:
		a,all		Compile the project
		r,remake	Recompile from scratch
		c,clean		Remove generated files (binary and object files)
```

File Syntax
-----------

**sections** are referred to as the names that are not indented.   
Their **settings** are referanced through them and must be indented, either by spaces or tabs.

Notes:
* Neither the sections or settings are case-sensitive.
* The file can be delimited by either spaces or tabs
* [POSCAR format](http://cms.mpi.univie.ac.at/vasp/guide/node59.html) (format of .vasp files used througout)


###Comments
`@` act as comments, and pore will ignore anything after that character.

Settings by Section - ([Docopt syntax](http://docopt.org/))
-------------------------------------------------------
###Input
```
filename <filename> [<X Y Z>]   
filename <file.tsv>
Options:
	<filename>	file in POSCAR format (.vasp)
	<file.tsv>	File containing filenames [Format for each line: (<file.tsv> | <X Y Z>)]
	<X Y Z>		Scale file amount in direction [default: 1	1	1]
note:
	multiple "filename" lines may be included!

```
###Scaling
```
Scale <X Y Z>
Scale <file.tsv>
Options:
	<file.tsv>	File containing scales [Format for each line: (<file.tsv> | <X Y Z>)]
	<X Y Z>		Scale file amount in direction [default: 1	1	1]
note:
	multiple "scale" lines may be included!
```
###Pore
```
coordinate <x> <y> <z>
Coordinate <file.tsv>
coordinate random <number>
coordinate distribute <number>
coordinate randomNoOverlap <number>
	Options:
		<number>	Number of pores to attempt to create [Default: 1]
Radius <angstroms>
Radius from <begin> to <end> by <step>

Passivation <elementSymbol>
```
###Output
```
path <dir>
	Note: can use "path" to append a prefix to filenames
filename <filename>
convention [options]
	Options:
		N	number of the model
		F	Filename of input
		S	Scale of model
		P	Polytype/ pore
	Note: case-insensitive
delimeter (space | tab | <string>)
extension <extension>
	Options:
		<extension>	Extension appended to the file with a leading '.'
	Note:	can use "extension" to place a suffix on filenames
```
###Data
```
tag [options] %<elementSymbol>% #<elementSymbol>#
	Options:
		D	Density of the model in g/cm^3
		E	Extension of the file
		I	Input filename
		O	Output filename
		P	Passivation number (Delta#H)
		p	path to output file
		R	Radius caluclated from average distance to passivation (Angstroms)
		r	radius used for making pores (Angstroms)
		S	The Full scale of the model (filescale*scale)
		%<elementSymbol>%	The percent of the element in model by number
		#<elementSymbol>#	The number of the element in model
	Note: these weill be written as headers to the datafile in the order selected
	Example: tag I%Si%RO
filename <file.tsv>
```

Contribute
----------

- Issue Tracker: <https://github.com/Pyrogue/Pore/issues>
- Source Code: <https://github.com/Pyrogue/Pore/>

Support
-------

If you are having issues, please let us know.
We have a mailing list located at: poreproject@googlegroups.com

License
-------

xxx
