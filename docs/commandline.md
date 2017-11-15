# depthmapX Command Line Interface

## Overview
The depthmapX command line interface allows to run steps that are traditionally
executed by loading a file into the GUI and clicking buttons/menu entries
by running a command from a command line/shell.

The command line interface is especially useful in cases where a similar kind of
analysis needs to be repeated regularly, as
the steps can be written into a script and run automatically or 
semi-automatically, do not require user input via mouse/GUI and can be 
parallelised into multiple processes or on multiple machines.

The general idea is that each call to the command line interface reads in a
depthmapX graph file, then applies some transformation/analysis to it and saves
the result into a different file. Thus, a pipeline of operations can be
created where each step reads in the input of the previous step, and in the end
we have the end result and all intermediate steps.  This is particularly helpful
if something has gone wrong, as we can go back, figure out which step went wrong
and rerun this and all subsequent steps.

The command line interface also supports importing of data from DXF and csv 
formats and export to csv so that a whole pipeline from plan to statistical
output can be run programmatically.

## The Command Line

The functionality in the depthmapX cli is split up into execution modes - each
mode roughly represents a class of functionalities in the UI. The overall 
commandline looks like:

Mac/Linux: `./depthmapXcli -f <infile.graph> -o <outfile.graph> -m <mode> [mode 
options]`  
Windows: `depthmapXcli.exe -f <infile.graph> -o <outfile.graph> -m <mode>
[mode options]`

