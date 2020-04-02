#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <string>
#include <set>
#include <list>
#include "DataTypes.h"
#include "SymbolTable.h"

using namespace std;

#define MAX_DIRECTIVE_SIZE 10
#define MAX_SECTION_NAME_SIZE 10  // with "" and .

#define MAX_TOKEN_SIZE 50 // 20 + 1 for '\0' end of string character

class Assembler {

public:

	Assembler();
	~Assembler();


	void assemble(string inputFileName, string outputFileName);
	void firstPass(string inputFile);
	void secondPass();

	void printSymTable();

private:
	
	static set<string> poolOfDirectives;
	
	static set<string> poolOfInstructions;  // just name of instruction - without operands

	char* firstToken, *secondToken, *thirdToken, *fourthToken;


	SymbolTable symTable;
	LineType lineType;

	unsigned lineNum = 0;

	unsigned tokenNum = 0;

	int tableIndex = 0;

	//location counters

	//int textLC, rodataLC, dataLC, bssLC;
	int globLC;

	unsigned textSectionSize, rodataSectionSize, dataSectionSize, bssSectionSize;

	unsigned textSA, dataSA, rodataSA, bssSA;

	char* textSection;
	char* rodataSection;
	char* dataSection;
	char* bssSection;



	//SIDE EFFECTS OF isLineValidMethod()

	Section section = dummy;

	Section prevSection = dummy;

	Directive directive;
	unsigned numOfInitValues; //for .char , .word and .long
	unsigned skipValue;
	bool labelExist;
	bool endOfFile = false;

	unsigned numOfGlobal; //for .global

	unsigned instructionSize;

	char* label;

	char* equsym;

	AddressingType adrType;
	char* sym_ = new char[MAX_TOKEN_SIZE];
	int immedVal;
	
	int immedValHex;

	int positionHex;
	int cntHex;


	//---------------------------------------------------
	//---------------------	METHODS ------------------------------

	//---------------------------------------------------

	void extractTokens(char * line);
	void flushTokens();

	bool isLineValid(char* line);

	bool isSection();
	bool isLabel();
	bool isInstruction();
	bool isDirective();

	bool isValidGlobalSeq(char * token);

	
	bool isValidInitSeq(char * token);

	bool isInstruction(char* line);
	bool isInstruction(char * firstToken, char * secondToken);
	bool isInstruction(char * firstToken, char * secondToken, char * thirdToken);

	bool isTokenDirective(char* token);
	bool isTokenInstructionName(char* token);

	//-----------------

	bool isDirectMemoryAccess(char* token);
	bool isRegisterIndirectAcess(char* token);
	

	//--------------------

	bool isRegister(char* token);
	bool isMemoryLocation(char* token);
	bool isValidImmediateNumber(char * token);
	bool isImmediate(char* token);
	bool isPCRelative(char* token);

	bool isValidSymbolName(char* token);
	bool isValidLabel(char* token);

	void removeSpaces(char* line);

	void handleValidLineFirstPass();


	//SECOND PASS

	//first pass loads this table
	list<TokenTableEntry*> tokenTable;

	list<RelocationTableEntry*> relocationTable;



	char * codeInstruction(char * ft);
	char * codeInstruction(char * ft, char * st);
	char * codeInstruction(char * ft, char * st, char * tt);

	list<char*> extractSymbols(char * initSeq);

	list<int> extractNumbers(char * initSeq);
	

	//defining string methods

	char* transformByteToHex(char byte);
	char* transformWordToHex(unsigned word);

	void copyString(char* dst, char* src);


	//elf file variables

	void makeELFRelocationTable();

	void makeELFSymbolTable();

	void makeELF(string filename);

	unsigned elfSymTableSize = 0;
	
	//char** elfSymTable = new char*[30];

	list<char*> elfSymTable;

	list<char*> elfRelocTable;

	/*list<char*> elfTextSection;
	list<char*> elfDataSection;
	list<char*> elfRodataSection;
	list<char*> elfBssSection;
	*/
	list<char*> globSection;

	list<char*>* currElfSection = &globSection;

	void freeSpace();
};



#endif