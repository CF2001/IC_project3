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

Exemplification:  ./findLang ../languages/Bulgarian/BULG2.txt ../languages/Danish/Danish1.utf8 ../languages/English/english1.txt ../languages/Estonian/estonian1.txt ../languages/Finnish/finnish1.txt ../languages/French/FR1.txt ../languages/German/german1.txt ../languages/Greek/greek1.txt ../languages/Hungarian/HG1.txt ../languages/Italian/italian2.txt ../languages/Latvian/latvian1.utf8 ../languages/Polish/polish2.txt ../languages/Portuguese/lusiadasCanto1.txt ../languages/Romanian/romanian1.utf8 ../languages/Slovak/slovak1.utf8 ../languages/Spanish/spanish1.txt ../languages/French/FR2.txt

```

### To test exercise 4 - application locateLang

```
./locateLang [options] <modelFile1> ... <modelFileN> <FileToTest>
Or
./locateLang [options] <folder> <FileToTest>

[options] => Described in the report


Exemplification 1 - Print the result on the terminal: 

./locateLang -v ../languages/English/english1.txt ../languages/French/FR1.txt ../languages/Portuguese/lusiadasCanto1.txt ../ioFiles/testInput2.txt

Exemplification 2 - Print the result in a file in the ioFiles directory:

./locateLang -v ../languages/English/english1.txt ../languages/French/FR1.txt ../languages/Portuguese/lusiadasCanto1.txt ../ioFiles/testInput2.txt > ../ioFiles/testOutput2.txt

Exemplification 3 : 

./locateLang -s 0.1 -w 3 -o 4 -i -v ../MoreLanguages ../ioFiles/testInput1.txt > ../ioFiles/testOutput1.txt
```








