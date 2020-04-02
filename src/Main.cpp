#include "Assembler.h"
#include "SymbolTable.h"
#include <string>
#include <string.h>

using namespace std;



int main(int argc, char* argv[]) {

	Assembler* assembler = new Assembler();

	if (argc != 4) {
		cout << "Sintaksa: ./program -o 'outputfile' 'inputfile'";
		exit(1);
	}

	if (strcmp("-o", argv[1]) != 0) {
		cout << "Sintaksa: ./program -o 'outputfile' 'inputfile'";
		exit(1);
	}

	
	string input(argv[3]);
	string output(argv[2]);

	

	assembler->assemble(input, output);

	

	delete assembler;

	return 0;
}