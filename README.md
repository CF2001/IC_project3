# Project 3 - Information and Coding 

*******
*******

##  Preparing the compilation environment

In a terminal, enter the base directory of your project, create the **build** directory,
and use _cmake_ to prepare _make_

```
cd «directory-of-your-choice»
cd «your-project»
mkdir build
cd build
cmake ../tests/
```

****** 

## Compiling the code

In a terminal, enter the **build** directory of your project and run _make_ 

```
cd «directory-of-your-choice»
cd «your-project»«your-project»«your-project»/build
make
```

****** 

## Testing the code 

After building the code, a program will be put in the <tt>«your-project»/tests-bin</tt> directory.

## locatelang Execution examples:

./locatelang -v ../languages/English/english.txt ../languages/French/FR.txt ../languages/Portuguese/portuguese.txt ../ioFiles/testInput2.txt > ../ioFiles/testOutput2.txt

./locatelang -s 0.1 -w 3 -o 4 -i -v ../MoreLanguages ../ioFiles/testInput1.txt > ../ioFiles/testOutput1.txt