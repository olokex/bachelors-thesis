# Comparison of circuit encodings used in evolutionary combinational circuit design
Comparison between two encodings, which are CGP _Cartesian Genetic Programming_ and ANF _Algebraic Normal Form_.

## Build dependencies
Code requires:
* GCC or Clang supporting C++17
* [Boost library](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html) version **1.66.0 or higher**
* Cmake version **3.18.2 or higher**

## Building
In order to compile the application you only have to run `make` that will make `./build` with binaries.

## Usage
Path argument requires file in truth table notation. For futher comprehension, investigate the `./data` directory.
#### CGP:
```
./build/cgp path ./data/parity5.txt
```

#### ANF:
```
./build/anf path ./data/parity5.txt
```

### Parameters
In case of parameter duplicity, the last value is used. The order of the parameters doesn't matter.

**Common**
option          | expected value
----------------|---------------
lambda          | >= 1
path            | truth table path
mutate          | >= 1
generations     | >= 1
seed            | >= 0
print-count     | >= 1 (have to be used with print-fitness)
print-fitness   | true/false
print-used-gates| true/false
print-used-area | true/false
second-criterion| true/false

**CGP only**
option                | expected value
----------------------|---------------
column                | >= 1
row                   | >= 1
lback                 | >= 1
functions (2 and more)| in,not,and,or,xor,nand,nor,xnor (no spaces within comma)

**ANF only**
option      | expected value
------------|---------------
terms       | >= inputs count
arity       | >= 1
print-ascii | true/false

### Get in touch
Don't hesitate to contact me, if anything unclear, or not working. <br>
[VUT e-mail](mailto:xsedla1e@fit.vutbr.cz) <br>
[olokex@gmail.com](mailto:olokex@gmail.com) (personal and preffered)
