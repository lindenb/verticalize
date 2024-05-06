Verticalize
===========

[![Conda Badge](https://anaconda.org/conda-forge/verticalize/badges/version.svg)](https://anaconda.org/conda-forge/verticalize)

Simple tool to verticalize text delimited files.

Author: Pierre Lindenbaum Phd @yokofakun

![http://i.imgur.com/23kd0Il.jpg](http://i.imgur.com/23kd0Il.jpg)

## Installation

Verticalize can be installed with conda:

```
conda install conda-forge::verticalize
```

## Compilation

```
make
```


## Usage

```
verticalize (options) (file|stdin)
```

## Options

* **-n** first line is NOT the header
* **-d (char)** delimiter (default: tab)
* **-h** get help.




## Example

### Example 1: git log

```
$ git log --pretty=format:'%h|%an|%s' | verticalize -n -d '|'
>>> 1
$1   : 53c8167
$2   : Pierre Lindenbaum
$3   : 1st
<<< 1

>>> 2
$1   : 9e23c0c
$2   : Pierre Lindenbaum
$3   : Initial commit
<<< 2
```

### Example 2 : A VCF file

```bash
$ curl "https://raw.githubusercontent.com/ekg/vcflib/master/samples/sample.vcf"  |\
  grep -vE "^##" |\
  verticalize
```

```
>>> 2
$1    #CHROM : 19
$2       POS : 111
$3        ID : .
$4       REF : A
$5       ALT : C
$6      QUAL : 9.6
$7    FILTER : .
$8      INFO : .
$9    FORMAT : GT:HQ
$10  NA00001 : 0|0:10,10
$11  NA00002 : 0|0:10,10
$12  NA00003 : 0/1:3,3
<<< 2

>>> 3
$1    #CHROM : 19
$2       POS : 112
$3        ID : .
$4       REF : A
$5       ALT : G
$6      QUAL : 10
$7    FILTER : .
$8      INFO : .
$9    FORMAT : GT:HQ
$10  NA00001 : 0|0:10,10
$11  NA00002 : 0|0:10,10
$12  NA00003 : 0/1:3,3
<<< 3

>>> 4
$1    #CHROM : 20
$2       POS : 14370
$3        ID : rs6054257
$4       REF : G
$5       ALT : A
$6      QUAL : 29
$7    FILTER : PASS
$8      INFO : NS=3;DP=14;AF=0.5;DB;H2
$9    FORMAT : GT:GQ:DP:HQ
$10  NA00001 : 0|0:48:1:51,51
$11  NA00002 : 1|0:48:8:51,51
$12  NA00003 : 1/1:43:5:.,.
<<< 4

>>> 5
$1    #CHROM : 20
$2       POS : 17330
$3        ID : .
$4       REF : T
$5       ALT : A
$6      QUAL : 3
$7    FILTER : q10
$8      INFO : NS=3;DP=11;AF=0.017
$9    FORMAT : GT:GQ:DP:HQ
$10  NA00001 : 0|0:49:3:58,50
$11  NA00002 : 0|1:3:5:65,3
$12  NA00003 : 0/0:41:3:.,.
<<< 5

>>> 6
$1    #CHROM : 20
$2       POS : 1110696
$3        ID : rs6040355
$4       REF : A
$5       ALT : G,T
$6      QUAL : 67
$7    FILTER : PASS
$8      INFO : NS=2;DP=10;AF=0.333,0.667;AA=T;DB
$9    FORMAT : GT:GQ:DP:HQ
$10  NA00001 : 1|2:21:6:23,27
$11  NA00002 : 2|1:2:0:18,2
$12  NA00003 : 2/2:35:4:.,.
<<< 6

>>> 7
$1    #CHROM : 20
$2       POS : 1230237
$3        ID : .
$4       REF : T
$5       ALT : .
$6      QUAL : 47
$7    FILTER : PASS
$8      INFO : NS=3;DP=13;AA=T
$9    FORMAT : GT:GQ:DP:HQ
$10  NA00001 : 0|0:54:.:56,60
$11  NA00002 : 0|0:48:4:51,51
$12  NA00003 : 0/0:61:2:.,.
<<< 7

>>> 8
$1    #CHROM : 20
$2       POS : 1234567
$3        ID : microsat1
$4       REF : G
$5       ALT : GA,GAC
$6      QUAL : 50
$7    FILTER : PASS
$8      INFO : NS=3;DP=9;AA=G;AN=6;AC=3,1
$9    FORMAT : GT:GQ:DP
$10  NA00001 : 0/1:.:4
$11  NA00002 : 0/2:17:2
$12  NA00003 : 1/1:40:3
<<< 8

>>> 9
$1    #CHROM : 20
$2       POS : 1235237
$3        ID : .
$4       REF : T
$5       ALT : .
$6      QUAL : .
$7    FILTER : .
$8      INFO : .
$9    FORMAT : GT
$10  NA00001 : 0/0
$11  NA00002 : 0|0
$12  NA00003 : ./.
<<< 9

>>> 10
$1    #CHROM : X
$2       POS : 10
$3        ID : rsTest
$4       REF : AC
$5       ALT : A,ATG
$6      QUAL : 10
$7    FILTER : PASS
$8      INFO : .
$9    FORMAT : GT
$10  NA00001 : 0
$11  NA00002 : 0/1
$12  NA00003 : 0|2
<<< 10
```
