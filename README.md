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

### To test exercise 1	- program fcm

```
./testFCM <inputFile> <outputFile> <order>

<inputFile> 	=>	text file to determine the model
<outputFile>	=>	text file where the model is stored
<order>			=>	model order

Exemplification: ./testFCM ../languages/Portuguese/fernadoPessoa.txt model.txt 4
```

### To test exercise 2	- program lang

```
./lang <modelFile> <testFile> <order> <smoothing>

<modelFile> 	=> text file to create the example model
<testFile>		=> text file to estimate the number of bits for compression
<order>			=> model order
<smoothing>		=> smoothing parameter

Exemplification: ./lang ../languages/Portuguese/PT1.utf8 ../languages/German/german1.txt 4 0.1
```

### To test exercise 3	- program findlang

```
./findLang <modelFile1> <...> <modelFile10> <languageToDiscover>

<modelFile> 			=> text file to create the example model
<languageToDiscover>	=> language text to discover

Exemplification:  ./findLang ../languages/Bulgarian/BULG1.txt ../languages/English/english1.txt ../languages/Estonian/estonian1.txt 
				../languages/French/FR1.txt ../languages/German/german1.txt ../languages/Greek/greek1.txt ../languages/Italian/italian1.txt 
				../languages/Polish/polish1.txt ../languages/Portuguese/lusiadasCanto1.txt ../languages/Spanish/spanish1.txt ../languages/French/FR2.txt

```

### To test exercise 4

./locatelang -v ../languages/English/english.txt ../languages/French/FR.txt ../languages/Portuguese/portuguese.txt ../ioFiles/testInput2.txt > ../ioFiles/testOutput2.txt

./locatelang -s 0.1 -w 3 -o 4 -i -v ../MoreLanguages ../ioFiles/testInput1.txt > ../ioFiles/testOutput1.txt
