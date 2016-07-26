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
in_file		unitcell.vasp
center			.5	.5	.5
radius			2
out_file	pore
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

Notes:
* setting names are not case-sensitive.
* The file can be delimited by either spaces or tabs
* [POSCAR format](http://cms.mpi.univie.ac.at/vasp/guide/node59.html) (format of .vasp files used througout)


###Comments
`@` act as comments, and pore will ignore anything after that character.

Settings - ([Docopt syntax](http://docopt.org/))
-------------------------------------------------------
```
in_file <filename> [<X Y Z>]
Options:
	<filename>	file in POSCAR format (.vasp)
	<X Y Z>		Scale file amount in direction [default: 1	1	1]
note:
	multiple "in_file" lines may be included!

center <x y z>
	Options:
		<x y z>		coordinates of the pore [Default:1]
random <number>
distribute <number>
random_No_Overlap <number>
	Options:
		<number>	Number of pores to attempt to create [Default: 1]
		
Radius <angstroms>
Radius from <begin> to <end> by <step>

Passivation <elementSymbol>

out_path <dir>
	Note: can use "path" to append a prefix to filenames
out_file <filename>
out_convention [options]
	Options:
		N	number of the model
	Default: "N"
	Note: case-insensitive
out_delimeter (space | tab | <string>)
out_extension <extension>
	Options:
		<extension>	Extension appended to the file with a leading '.'
	Note:	can use "extension" to place a suffix on filenames

data_tag [options] %<elementSymbol>% #<elementSymbol>#
	Options:
		c,C Center of pores
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
data_file <file.tsv>
```

Contribute
----------

- Issue Tracker: <https://github.com/brt10/Nano-Pore-Maker/issues>
- Source Code: <https://github.com/brt10/Nano-Pore-Maker/>

Support
-------

If you are having issues, please let us know.
We have a mailing list located at: poreproject@googlegroups.com

License
-------

xxx
