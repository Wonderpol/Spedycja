### Spedycja - about project

## Table of contents
* [About project](#spedycja--about-project)
* [Technologies](#technologies)
* [Setup](#setup)

### Spedycja - about project
Spedycja - compute the shortest route between cities and show the path.

## Get started
- Clone project from github repository.

**Linux/mac users:**
1. Compile it using for example g++ `g++ -o spedycja main.cpp`
2. Run with parameters -i input txt file -o output file -s start city: `./spedycja -i inputfile.txt -o outputfile.txt -s Poznań`.

**Windows users:**
1. TODO

## Format of input and output files

**Input file:**
1. If you want to use polish special characters set file coding to: `UTF-8`
2. Format of input file: `City City Distance`

**The way how the input file should be formatted:**
```
Szczecin Poznań 220
Szczecin Koszalin 110
Poznań Bytom 300
Poznań Łódź 130
Łódź Katowice 170
Bytom Katowice 15
Bytom Wrocław 180
```

**Example output generated using input file defined above and start point as Poznań:**
```
Poznań -> Bytom: 300
Poznań -> Łódź -> Katowice: 300
Poznań -> Szczecin -> Koszalin: 330
Poznań -> Szczecin: 220
Poznań -> Bytom -> Wrocław: 480
Poznań -> Łódź: 130

```
