prog: compile execute

compile:
	gcc -ansi -Wall vigenere.c -o vigenere

execute:
	vigenere > out.txt

rm:
	del vigenere.exe