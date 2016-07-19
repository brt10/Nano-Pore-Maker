pore - Passivated Objective Radial Eliminator
=============================================

Quickstart:
-----------

compile:
`$ make a`   
Run:
`$ ./bin/pore filename`   
`filename` is a .tsv laid out simularly to the following example:
```
INPUT
	filename	unitcell.vasp
PORE
	coordinate	.5	.5	.5
	radius		2
OUTPUT
	filename	pore
```
This makes a single pore of radius 2 Angstroms in the center of the `unitcell.vasp`, and outputs the resulting file to `pore.vasp`

Compilation
-----------

compile project by running the makefile in the main directory   
`$ make a`

Usage
-----

**sections** are referred to as the capitalized names that are not indented.   
Their **settings** are referanced through them and must be indented, either by spaces or tabs.

Note that neither the sections or settings are case-sensitive.

####Comments
`@` act as comments, and pore will ignore anything after that character.

####Settings by Section - using Docopt - http://docopt.org/
#####Input
```
filename <filename> [(<scaleX scaleY scaleZ>)]   
filename <file.tsv> @file.tsv of format for each line: [file.vasp (int scale[3]) | files.tsv]	
```
#####Scaling
```
Scale <scaleX> <scaleY> <scaleZ>
Scale <file.tsv>
Options:
	<file.tsv>	File containing scales [Format for each line: (<file.tsv> | <scaleX> <scaleY> <scaleZ>)]
	<Scale_>	scale file in direction [default: 1]
note:
	all scales default to 1
	multiple scale lines may be included!
```
#####~~Bonding~~
#####Pore
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
#####Output
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
#####Data
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
filename <file.tsv>
```
#####~~Conditions~~



```
@This is a comment
@USING STANDARDS LAID OUT IN: http://pubs.opengroup.org/onlinepubs/009696899/basedefs/xbd_chap12.html
@(IEEE NOTATION)
@all whitespace should be tabs to be safe...
@optional in parentheses
@-------------------------------------
INPUT		
	filename		[filename (int scale[3]) | (file).tsv]	@file.tsv of format for each line: [file.vasp (int scale[3]) | 
	files.tsv]
		default:	<NONE>
@--------scaling of outputs------------
SCALING
	scale			[(u_int scale[3]) | scaling.tsv]	@scaling.tsv of format for each line: [scaling.tsv | u_int scale[3]]
		default:	1	1	1
@-------------------------------------
BONDING		
	tolerance		[(double tolerance)]
		default:	.1
	lengths			[(string element) (double Angstroms) (string element) | bonding.tsv]
		default:	(see src/K.h)
@-------------------------------------
PORE		
	@	seed			[(unsigned int)]	
	coordinate		[pores.tsv | (coordinate[3]) | random (number=1) | distribute (number=1) | RandomNoOverlap (number=1)]
	center			[(string element)]
	radius			[(double Angstroms) | from (double Angstroms) to (double Angstroms) by (double Angstroms)]
	passivation 	[(string element) | none]
@-------------------------------------
OUTPUT
	path			[(directory name)]	@use path to append a prefix to filenames
	filename		[(string)]
	convention		[N|F|S|P]
		N	number of the model
	delimeter		[none | space | tab | char(character)]
	extension		[(extension name)]
@-------------------------------------
DATA
	tag				[O|P|E|%<elementSymbol>%|#<elementSymbol>#|D|R|S|I]
		A
		B
		C
		D	density
		E	extension
		F
		G
		H
		I	inputfile
		J
		K
		L
		M
		N
		O	outfilename
		p	path
		P	Passivation (Delta#H)
		Q
		R	Radius	(calculated)
		r	radius	(used)
		S	fullscale
		T
		U
		V
		W
		X
		Y
		Z
		%	percentage of element
		#	number of element

		@EXAMPLE:	tag	I%RO
	filename		[(string)]
@------------------------------------
CONDITIONS
	density			[(double low)<tab>high]
	Percent			[element]	[low,high]
	number			[element]	[low,high]
```

Contribute
----------

- Issue Tracker: github.com/Pyrogue/Pore/issues
- Source Code: github.com/Pyrogue/Pore/

Support
-------

If you are having issues, please let us know.
We have a mailing list located at: poreproject@googlegroups.com

License
-------

xxx