
#include "Assembler.h"



#include <fstream>
#include <iostream>

#include <string.h>

#include <string>

#include <stdlib.h>


#include <cstdio>
#include <cwchar>
#include <stdio.h>
#include <wchar.h>

#include <regex>

using namespace std;


set<string> Assembler::poolOfDirectives;
set<string> Assembler::poolOfInstructions;


Assembler::Assembler() {
	poolOfDirectives.insert(".char");
	poolOfDirectives.insert(".word");
	poolOfDirectives.insert(".long");
	poolOfDirectives.insert(".align");
	poolOfDirectives.insert(".skip");
	poolOfDirectives.insert(".end");
	poolOfDirectives.insert(".global");
	poolOfDirectives.insert(".extern");
	poolOfDirectives.insert(".equ");


	poolOfInstructions.insert("add");
	poolOfInstructions.insert("sub");
	poolOfInstructions.insert("mul");
	poolOfInstructions.insert("div");
	poolOfInstructions.insert("cmp");
	poolOfInstructions.insert("and");
	poolOfInstructions.insert("or");
	poolOfInstructions.insert("not");
	poolOfInstructions.insert("test");
	poolOfInstructions.insert("mov");
	poolOfInstructions.insert("shl");
	poolOfInstructions.insert("shr");
	poolOfInstructions.insert("push");
	poolOfInstructions.insert("pop");
	poolOfInstructions.insert("iret");
	poolOfInstructions.insert("call");

	poolOfInstructions.insert("addeq");
	poolOfInstructions.insert("subeq");
	poolOfInstructions.insert("muleq");
	poolOfInstructions.insert("diveq");
	poolOfInstructions.insert("cmpeq");
	poolOfInstructions.insert("andeq");
	poolOfInstructions.insert("oreq");
	poolOfInstructions.insert("noteq");
	poolOfInstructions.insert("testeq");
	poolOfInstructions.insert("moveq");
	poolOfInstructions.insert("shleq");
	poolOfInstructions.insert("shreq");
	poolOfInstructions.insert("pusheq");
	poolOfInstructions.insert("popeq");
	poolOfInstructions.insert("ireteq");
	poolOfInstructions.insert("calleq");


	poolOfInstructions.insert("addne");
	poolOfInstructions.insert("subne");
	poolOfInstructions.insert("mulne");
	poolOfInstructions.insert("divne");
	poolOfInstructions.insert("cmpne");
	poolOfInstructions.insert("andne");
	poolOfInstructions.insert("orne");
	poolOfInstructions.insert("notne");
	poolOfInstructions.insert("testne");
	poolOfInstructions.insert("movne");
	poolOfInstructions.insert("shlne");
	poolOfInstructions.insert("shrne");
	poolOfInstructions.insert("pushne");
	poolOfInstructions.insert("popne");
	poolOfInstructions.insert("iretne");
	poolOfInstructions.insert("callne");

	poolOfInstructions.insert("addgt");
	poolOfInstructions.insert("subgt");
	poolOfInstructions.insert("mulgt");
	poolOfInstructions.insert("divgt");
	poolOfInstructions.insert("cmpgt");
	poolOfInstructions.insert("andgt");
	poolOfInstructions.insert("orgt");
	poolOfInstructions.insert("notgt");
	poolOfInstructions.insert("testgt");
	poolOfInstructions.insert("movgt");
	poolOfInstructions.insert("shlgt");
	poolOfInstructions.insert("shrgt");
	poolOfInstructions.insert("pushgt");
	poolOfInstructions.insert("popgt");
	poolOfInstructions.insert("iretgt");
	poolOfInstructions.insert("callgt");


	poolOfInstructions.insert("jmp");
	poolOfInstructions.insert("jne");
	poolOfInstructions.insert("jeq");
	poolOfInstructions.insert("jgt");



	firstToken = new char[MAX_TOKEN_SIZE];
	secondToken = new char[MAX_TOKEN_SIZE];
	thirdToken = new char[MAX_TOKEN_SIZE];
	fourthToken = new char[MAX_TOKEN_SIZE];

	for (int i = 0; i < MAX_TOKEN_SIZE; i++) {
		firstToken[i] = secondToken[i] = thirdToken[i] = fourthToken[i] = 0;
	}

	//textLC = rodataLC = dataLC = bssLC = 0;

	label = new char[MAX_TOKEN_SIZE];

}


void Assembler::flushTokens() {
	for (int i = 0; i < MAX_TOKEN_SIZE; i++) {
		firstToken[i] = secondToken[i] = thirdToken[i] = fourthToken[i] = 0;
	}
}

void Assembler::extractTokens(char* line) {
	removeSpaces(line);

	int i = 0;
	int j = 0;

	//first token

	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
		if (j >= MAX_TOKEN_SIZE) {
			cerr << "Too big token in line:" << lineNum;
			exit(-1);
		}
		firstToken[j] = line[i];
		i++;
		j++;
	}


	firstToken[j] = '\0';
	//prepare for second token
	j = 0;


	//skiping blanks between first and potentially second token

	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;


	if (line[i] == '\0') {
		//no more tokens
		secondToken[0] = thirdToken[0] = fourthToken[0] = '\0';
		tokenNum = 1;
		return;
	}


	//second token

	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
		if (j >= MAX_TOKEN_SIZE) {
			cerr << "Too big token in line:" << lineNum;
			exit(-1);
		}
		secondToken[j] = line[i];
		i++;
		j++;
	}
	secondToken[j] = '\0';
	j = 0;


	//skiping potential blanks between second and potentially third token

	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;

	if (line[i] == '\0') {
		//there are no third and fourth tokens
		thirdToken[0] = fourthToken[0] = '\0';
		tokenNum = 2;
		return;
	}


	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
		if (j >= MAX_TOKEN_SIZE) {
			cerr << "Too big token in line:" << lineNum;
			exit(-1);
		}
		thirdToken[j] = line[i];
		i++;
		j++;
	}
	thirdToken[j] = '\0';
	j = 0;


	//skiping blanks between second and potentially third token

	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;

	if (line[i] == '\0') {
		//there is no fourth token
		tokenNum = 3;
		fourthToken[0] = '\0';
		return;
	}


	while (line[i] != ' ' && line[i] != '\t' && i != '\0') {
		if (j >= MAX_TOKEN_SIZE) {
			cerr << "Too big token in line:" << lineNum;
			exit(-1);
		}
		fourthToken[j] = line[i];
		i++;
		j++;
	}
	fourthToken[j] = '\0';
	j = 0;

	tokenNum = 4;


	//check if there is more tokens - that would be ERROR

	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;

	if (line[i] != '\0') {
		tokenNum = 5;
	}



}


Assembler::~Assembler() {

	freeSpace();

}


bool Assembler::isTokenDirective(char* token) {

	string tmp(token);

	set<string> ::iterator it = poolOfDirectives.find(tmp);

	if (it == poolOfDirectives.end())
		return false;

	return true;
}


bool Assembler::isTokenInstructionName(char* token) {

	if (poolOfInstructions.find(token) == poolOfInstructions.end()) {
		return false;
	}



}


void Assembler::removeSpaces(char* str) {
	int i = 0, j = 0;

	while (str[i] == ' ' || str[i] == '\t') {
		i++;
	}

	while (str[i]) {
		str[j] = str[i];
		i++;
		j++;
	}

	str[j] = '\0';

}



bool Assembler::isSection() {

	//first condition
	if (tokenNum > 2)
		return false;

	//second condition
	if (strcmp(firstToken, ".section") != 0)
		return false;


	if (section != dummy) {
		prevSection = section;

		switch (section) {
		case TEXT:
			textSectionSize = globLC - textSA;
			break;
		case DATA:
			dataSectionSize = globLC - dataSA;
			break;
		case RODATA:
			rodataSectionSize = globLC - rodataSA;
			break;
		case BSS:
			bssSectionSize = globLC - bssSA;
			break;
		default:
			exit(-1);
		}
	}



	//third condition
	if (strcmp(secondToken, "\".rodata\"") == 0) {
		section = RODATA;
		rodataSA = globLC;
		return true;
	}

	if (strcmp(secondToken, "\".data\"") == 0) {
		section = DATA;
		dataSA = globLC;
		return true;
	}

	if (strcmp(secondToken, "\".text\"") == 0) {
		section = TEXT;
		textSA = globLC;
		return true;
	}

	if (strcmp(secondToken, "\".bss\"") == 0) {
		section = BSS;
		bssSA = globLC;
		return  true;
	}

	return false;

}



bool Assembler::isLabel() {
	int i = 0;

	if (tokenNum > 4)
		return false;

	if (!isValidLabel(firstToken))
		return false;

	//...else label is alright

	while (firstToken[i] != '\0') {
		label[i] = firstToken[i];
		i++;
	}
	label[i] = '\0';



	//now check if there is more than one token

	if (secondToken[0] == '\0') {
		lineType = LABEL;
		return true;
	}


	if (isTokenDirective(secondToken)) {

		lineType = LABEL_AND_DIRECTIVE;

		if (tokenNum > 3)
			return false;

		if (strcmp(secondToken, ".align") == 0) {
			directive = ALIGN;
			if (tokenNum > 2)
				return false;
			return true;
		}

		if (strcmp(secondToken, ".end") == 0) {
			directive = END;
			if (tokenNum > 2)
				return false;
			return true;
		}



		if (tokenNum != 3)
			return false;

		if (strcmp(secondToken, ".skip") == 0) {
			directive = SKIP;
			while (thirdToken[i] != '\0') {
				if (!isdigit(secondToken[i]))
					return false;
				i++;
			}
			int num = atoi(thirdToken);
			skipValue = num;
			return 0 <= num <= 1024;
		}


		if (strcmp(secondToken, ".char") == 0) {
			directive = CHAR;
			return isValidInitSeq(thirdToken);
		}

		if (strcmp(secondToken, ".word") == 0) {
			directive = WORD;
			return isValidInitSeq(thirdToken);
		}

		if (strcmp(secondToken, ".long") == 0) {
			directive = LONG;
			return isValidInitSeq(thirdToken);
		}
	}


	if (isTokenInstructionName(secondToken)) {
		lineType = LABEL_AND_INSTRUCTION;

		if (tokenNum == 2)
			return isInstruction(secondToken);

		if (tokenNum == 3)
			return isInstruction(secondToken, thirdToken);

		if (tokenNum == 4)
			return isInstruction(secondToken, thirdToken, fourthToken);

		return false;
	}

	return false;
}



bool Assembler::isInstruction() {
	if (tokenNum == 2)
		return isInstruction(secondToken);

	if (tokenNum == 3)
		return isInstruction(secondToken, thirdToken);

	if (tokenNum == 4)
		return isInstruction(secondToken, thirdToken, fourthToken);

}


//instructions without operands
bool Assembler::isInstruction(char* firstToken) {

	//iret is only instruction without operands
	instructionSize = 1;
	if (strcmp(firstToken, "iret") == 0 || strcmp(firstToken, "ireteq") == 0 || strcmp(firstToken, "iretne") == 0
		|| strcmp(firstToken, "iretgt") == 0)
		return true;

	return false;

}

//instructions with one operand
bool Assembler::isInstruction(char* firstToken, char* secondToken) {

	if (strcmp(firstToken, "push") == 0 || strcmp(firstToken, "pushb") == 0
		|| strcmp(firstToken, "pushw") == 0) {
		
		
		
		int i = 0;
		int opersize = -1;
		while (firstToken[i] != '\0')
			i++;

		char osize = firstToken[i - 1];
		if (osize = 'w') {
			opersize = 1;
		}

		if (osize = 'b') {
			opersize = 0;
		}
		
		
		if (isRegister(secondToken)) {
			instructionSize = 2;
			return true;
		}

		if (isMemoryLocation(secondToken)) {
			instructionSize = 4;
			return true;
		}

		if (isImmediate(secondToken)) {
			if (opersize == 0) {
				instructionSize = 3;
			}
			else {
				instructionSize = 4;
			}
			return true;
		}

		return false;
	}


	if (strcmp(firstToken, "pop") == 0 || strcmp(firstToken, "popb") == 0
		|| strcmp(firstToken, "popw") == 0) {
		if (isRegister(secondToken)) {
			instructionSize = 2;
			return true;
		}

		if (isMemoryLocation(secondToken)) {
			instructionSize = 4;
			return true;
		}
	}

	if (strcmp(firstToken, "call") == 0) {

		if (isRegister(secondToken)) {
			instructionSize = 2;
			return true;
		}

		if (isValidSymbolName(secondToken)) {
			instructionSize = 4;
			return true;
		}

		return false;
	}


	if (strcmp(firstToken, "jne") == 0 || strcmp(firstToken, "jeq") == 0
		|| strcmp(firstToken, "jgt") == 0 || strcmp(firstToken, "jmp") == 0) {

		if (isValidSymbolName(secondToken)) {
			instructionSize = 4;
			return true;
		}

		if (isPCRelative(secondToken)) {
			instructionSize = 4;
			return true;
		}

		return false;
	}

	return false;
}


//instructions with two operands
bool Assembler::isInstruction(char* firstToken, char* secondToken, char* thirdToken) {

	//look for comma 
	int i = 0;
	while (firstToken[i] != '\0') {
		i++;
	}
	char w = firstToken[i - 1];

	int opersize = -1;

	if (w == 'w') {
		opersize = 1;
	}
	else if (w == 'b' && !(strcmp(firstToken, "sub") == 0 || strcmp(firstToken, "subb") == 0)) {   //opcode for sub
		opersize = 0;
	}

	if (w == 'b' && firstToken[i - 2] == 'b') {
		opersize = 0;
	}


	
	i = 0;
	while (secondToken[i] != '\0') {
		i++;
	}

	if (secondToken[i - 1] != ',') {
		return false;
	}

	//we don't need comma anymore
	secondToken[i - 1] = '\0';


	if (strcmp(firstToken, "add") == 0 || strcmp(firstToken, "sub") == 0 || strcmp(firstToken, "mul") == 0
		|| strcmp(firstToken, "div") == 0 || strcmp(firstToken, "cmp") == 0 || strcmp(firstToken, "and") == 0
		|| strcmp(firstToken, "or") == 0 || strcmp(firstToken, "test") == 0 || strcmp(firstToken, "not") == 0
		|| strcmp(firstToken, "shl") == 0 || strcmp(firstToken, "shr") == 0 || strcmp(firstToken, "mov") == 0		//+
		|| strcmp(firstToken, "addb") == 0 || strcmp(firstToken, "subb") == 0 || strcmp(firstToken, "mulb") == 0
		|| strcmp(firstToken, "divb") == 0 || strcmp(firstToken, "cmpb") == 0 || strcmp(firstToken, "andb") == 0
		|| strcmp(firstToken, "orb") == 0 || strcmp(firstToken, "testb") == 0 || strcmp(firstToken, "notb") == 0
		|| strcmp(firstToken, "shlb") == 0 || strcmp(firstToken, "shrb") == 0 || strcmp(firstToken, "movb") == 0	//+
		|| strcmp(firstToken, "addw") == 0 || strcmp(firstToken, "subw") == 0 || strcmp(firstToken, "mulw") == 0
		|| strcmp(firstToken, "divw") == 0 || strcmp(firstToken, "cmpw") == 0 || strcmp(firstToken, "andw") == 0
		|| strcmp(firstToken, "orw") == 0 || strcmp(firstToken, "testw") == 0 || strcmp(firstToken, "notw") == 0 //+
		|| strcmp(firstToken, "shlw") == 0 || strcmp(firstToken, "shrw") == 0 || strcmp(firstToken, "movw") == 0)
		
	{
		
		
		
		
		if (isRegister(secondToken)) {
			if (isRegister(thirdToken)) {
				instructionSize = 3;
				return true;
			}


			if (isImmediate(thirdToken)) {
				if (opersize == 0) {
					instructionSize = 4;
				}
				else {
					instructionSize = 5;
				}
				return true;
			}

			if (isDirectMemoryAccess(thirdToken) ) {
				instructionSize = 5;
				return true;
			}

			if (isRegisterIndirectAcess(thirdToken)) {
				switch (adrType) {
				case REG_INDIRECT_ZERO:
					instructionSize = 3;
					break;
				case REG_INDIRECT_8:
					instructionSize = 4;
					break;
				case REG_INDIRECT_16:
					instructionSize = 5;
					break;
				case REG_INDIRECT_SYMBOL:
					instructionSize = 5;
					break;
				default:
					exit(-1);
				}
				return true;
			}

			return false;
		}

		if (isMemoryLocation(secondToken)) {
			
			AddressingType local = REG_DIRECT;

			if (isRegisterIndirectAcess(secondToken)) {
				local = adrType;
				if (isRegister(thirdToken)) {
					switch (adrType) {
					case REG_INDIRECT_ZERO :
						instructionSize = 3;
						break;
					case REG_INDIRECT_8:
						instructionSize = 4;
						break;
					case REG_INDIRECT_16:
						instructionSize = 5;
						break;
					case REG_INDIRECT_SYMBOL:
						instructionSize = 5;
						break;
					default:
						exit(-1);
					}
					return true;
				}

				if (isImmediate(thirdToken)) {
					switch (local) {
					case REG_INDIRECT_ZERO:
						instructionSize = 5;
						break;
					case REG_INDIRECT_8:
						instructionSize = 6;
						break;
					case REG_INDIRECT_16:
						instructionSize = 7;
						break;
					case REG_INDIRECT_SYMBOL:
						instructionSize = 7;
						break;
					default:
						exit(-1);
					}
					return true;
				}


			}

			if (isDirectMemoryAccess(secondToken)) {
				if (isRegister(thirdToken)) {
					instructionSize = 5;
					return true;
				}

				if (isImmediate(thirdToken)) {
					if (opersize == 0) {
						instructionSize = 6;
					}
					else {
						instructionSize = 7;
					}
					return true;
				}

			}


			

			

			return false;
		}

		return false;
	}




}


bool Assembler::isValidSymbolName(char* token) {

	if (!isalpha(token[0]))
		return false;

	int i = 0;
	while (token[i] != '\0') {
		if (!isalpha(token[i]) && !isdigit(token[i]) && token[i] != '_')
			return false;
		sym_[i] = token[i];
		i++;
	}
	sym_[i] = '\0';


	return true;
}


bool Assembler::isValidLabel(char* token) {
	if (!isalpha(token[0]))
		return false;

	int i = 1;
	while (token[i] != '\0' && token[i] != ':') {
		if (!isalpha(token[i]) && !isdigit(token[i]) && token[i] != '_')
			return false;
		i++;
	}

	if (token[i] != ':')
		return false;

	if (token[i + 1] != '\0')
		return false;

	return true;
}


bool Assembler::isRegister(char* token) {
	if (strcmp(token, "r0") == 0)
		return true;

	if (strcmp(token, "r1") == 0)
		return true;

	if (strcmp(token, "r2") == 0)
		return true;

	if (strcmp(token, "r3") == 0)
		return true;

	if (strcmp(token, "r4") == 0)
		return true;

	if (strcmp(token, "r5") == 0)
		return true;

	if (strcmp(token, "r6") == 0)
		return true;

	if (strcmp(token, "r7") == 0)
		return true;

	if (strcmp(token, "r0l") == 0)
		return true;

	if (strcmp(token, "r1l") == 0)
		return true;

	if (strcmp(token, "r2l") == 0)
		return true;

	if (strcmp(token, "r3l") == 0)
		return true;

	if (strcmp(token, "r4l") == 0)
		return true;

	if (strcmp(token, "r5l") == 0)
		return true;

	if (strcmp(token, "r6l") == 0)
		return true;

	if (strcmp(token, "r7l") == 0)
		return true;

	if (strcmp(token, "r0h") == 0)
		return true;

	if (strcmp(token, "r1h") == 0)
		return true;

	if (strcmp(token, "r2h") == 0)
		return true;

	if (strcmp(token, "r3h") == 0)
		return true;

	if (strcmp(token, "r4h") == 0)
		return true;

	if (strcmp(token, "r5h") == 0)
		return true;

	if (strcmp(token, "r6h") == 0)
		return true;

	if (strcmp(token, "r7h") == 0)
		return true;

	if (strcmp(token, "pc") == 0) {
		token[0] = 'r';
		token[1] = '7';
		return true;
	}

	if (strcmp(token, "sp") == 0){		
		token[0] = 'r';
		token[1] = '6';
		return true;
	}


	return false;

}


bool Assembler::isDirectMemoryAccess(char* token) {
	int i = 0;
	if (isValidSymbolName(token)) {
		adrType = MEM_DIRECT_SYMBOL;
		while (token[i] != '\0') {
			sym_[i] = token[i];
			i++;
		}
		sym_[i] = '\0';
		return true;
	}

	char firstChar = token[0];
	if (firstChar != '*')
		return false;

	char* temp = new char[MAX_TOKEN_SIZE];
	while (token[i] != '\0') {
		temp[i] = token[i];
		++i;
	}
	temp[i] = '\0';

	i = 1;
	while (temp[i] != '\0') {
		temp[i - 1] = token[i];
		i++;
	}
	temp[i - 1] = '\0';

	if (isValidImmediateNumber(temp)) {
		immedVal = atoi(temp);
		adrType = MEM_DIRECT_VALUE;
		return true;
	}

	return false;
}


bool Assembler::isRegisterIndirectAcess(char* token) {
	char * regToken = new char[3];
	char*  inBracketsToken = new char[7];

	regToken[0] = token[0];
	regToken[1] = token[1];
	regToken[2] = '\0';


	if (!isRegister(regToken))
		return false;

	token[0] = regToken[0];
	token[1] = regToken[1];

	if (token[2] != '[')
		return false;

	int i = 3;
	int j = 0;

	while (token[i] != ']' && token[i] != '\0') {
		inBracketsToken[j] = token[i];
		i++;
		j++;
	}
	inBracketsToken[j] = '\0';


	if (token[i] != ']')
		return false;

	if (token[i + 1] != '\0')
		return false;

	//reg[immediate]
	if (isImmediate(inBracketsToken)) {
		adrType = REG_INDIRECT_VALUE;
		if (immedVal == 0) {
			adrType = REG_INDIRECT_ZERO;
		}
		else if(immedVal >= -128 && immedVal <= 127){
			adrType = REG_INDIRECT_8;
		}
		else if (immedVal >= -32768 && immedVal <=32767) {
			adrType = REG_INDIRECT_16;
		}
		return true;
	}

	//reg[symbol]
	if (isValidSymbolName(inBracketsToken)) {
		adrType = REG_INDIRECT_SYMBOL;
		i = 0;
		while (token[i] != '\0') {
			sym_[i] = token[i];
			i++;
		}
		sym_[i] = '\0';
		return true;
	}

}


bool Assembler::isMemoryLocation(char* token) {

	if (isDirectMemoryAccess(token)) {
		return true;
	}


	if (isRegisterIndirectAcess(token)) {
		return true;
	}

	return false;
}


bool Assembler::isValidImmediateNumber(char* token) {
	bool isNumber = true;
	int i = 0;

	if (token[0] == '-') {
		i = 1;
	}

	while (token[i] != '\0') {
		if (!isdigit(token[i]))
			return false;
		i++;
	}

	int immed = atoi(token);

	if (-32768 <= immed <= 32767) {
		return true;
	}

	return false;

}


int transformHexToDecimal(char* token) {

	int accumulator = 0;
	int i = 0;
	int len;
	char currchar;
	int currdec;
	int base = 1;

	while (token[i] != '\0') {
		++i;
	}
	len = i;


	for (--len ; len >= 0; len--) {
		currchar = token[len];

		if (currchar >= '0' && currchar <= '9') {
			currdec = currchar - '0';
		}

		if (currchar >= 'A' && currchar <= 'F') {
			currdec = 10 + currchar - 'A';
		}

		accumulator += currdec * base;

		base = base * 16;
	}

	return accumulator;
}

bool Assembler::isImmediate(char* token) {


	char first = token[0];
	char second = token[1];

	if (first == '0' && second == 'x') {
		
		int j = 0;
		while(token[j] != '\0') {
			j++;
		}

		char* temp = new char[j];

		j = 0;

		while (token[j] != '\0') {
			temp[j] = token[j];
			j++;
		}
		temp[j] = '\0';


		int i = 2;
		while (temp[i] != '\0') {
			temp[i - 2] = temp[i];
			i++;
		}
		temp[i - 2] = '\0';

		if (isValidImmediateNumber(temp)) {
			adrType = IMMEDIATE_VALUE_HEX;
			immedVal = transformHexToDecimal(temp);
			return true;
		}
	}


	if (isValidImmediateNumber(token)) {
		adrType = IMMEDIATE_VALUE;
		immedVal = atoi(token);
		return true;
	}

	//immediate - &x

	if (first != '&')
		return false;

	int i = 1;
	while (token[i] != '\0') {
		sym_[i - 1] = token[i];
		i++;
	}
	sym_[i - 1] = '\0';

	if (isValidSymbolName(sym_)) {
		adrType = IMMEDIATE_SYMBOL_VALUE;
		return true;
	}


	return false;
}


bool Assembler::isPCRelative(char* token) {
	if (token[0] != '$') {
		return false;
	}

	/*int i = 1;
	while (token[i] != '\0') {
		sym_[i - 1] = token[i];
		i++;
	}
	sym_[i] = '\0';
	*/
	int i = 1;
	while (token[i] != '\0') {
		sym_[i - 1] = token[i];
		i++;
	}
	sym_[i - 1] = '\0';

	if (isValidSymbolName(sym_)) {
		adrType = PC_RELATIVE;
		return true;
	}

	return false;

}


//-------------------------------------------


bool Assembler::isDirective() {
	int i = 0;

	if (!isTokenDirective(firstToken))
		return false;




	if (strcmp(firstToken, ".end") == 0) {
		directive = END;
		if (tokenNum > 1)
			return false;
		return true;
	}


	if (strcmp(firstToken, ".align") == 0) {
		directive = ALIGN;
		if (tokenNum > 1)
			return false;
		return true;
	}


	if (tokenNum < 2)
		return false;

	if (strcmp(firstToken, ".equ") == 0) {
		directive = EQU;

		equsym = new char[MAX_TOKEN_SIZE];

		if (tokenNum != 3) {
			return false;
		}

		int i = 0;
		while (secondToken[i] != '\0') {
			equsym[i] = secondToken[i];
			i++;
		}
		if (secondToken[i - 1] != ',')
			return false;
		secondToken[i - 1] = '\0';
		equsym[i - 1] = '\0';

		if (!isValidSymbolName(secondToken))
			return false;

		if (!isImmediate(thirdToken))
			return false;

		return true;
	}


	if (strcmp(firstToken, ".skip") == 0) {
		directive = SKIP;
		while (secondToken[i] != '\0') {
			if (!isdigit(secondToken[i]))
				return false;
			i++;
		}
		int num = atoi(secondToken);
		skipValue = num;
		return 0 <= num <= 1024;
	}


	if (strcmp(firstToken, ".char") == 0) {
		directive = CHAR;
		return isValidInitSeq(secondToken);
	}

	if (strcmp(firstToken, ".word") == 0) {
		directive = WORD;
		return isValidInitSeq(secondToken);
	}

	if (strcmp(firstToken, ".long") == 0) {
		directive = LONG;
		return isValidInitSeq(secondToken);
	}

	if (strcmp(firstToken, ".global") == 0) {
		directive = GLOBAL;
		return isValidGlobalSeq(secondToken);
	}

	if (strcmp(firstToken, ".extern") == 0) {
		directive = EXTERN;
		return isValidGlobalSeq(secondToken);
	}
}


bool Assembler::isValidGlobalSeq(char* token) {
	numOfGlobal = 0;

	int i = 0;
	int j = 0;
	char* s = new char[MAX_TOKEN_SIZE];
	s[0] = '\0';

	while (token[i] != '\0') {
		if (token[i] == ',') {
			s[j] = '\0';
			if (!isValidSymbolName(s)) {
				return false;
			}
			numOfGlobal++;
			j = 0;
			i++;
		}
		else {
			s[j] = token[i];
			i++;
			j++;
		}
	}
	s[j] = '\0';
	if (!isValidSymbolName(s))
		return false;
	numOfGlobal++;

	return true;

}




bool Assembler::isValidInitSeq(char* token) {
	
	numOfInitValues = 0;
	int i = 0;
	int j = 0;
	char* num = new char[MAX_TOKEN_SIZE];
	num[0] = '\0';

	while (token[i] != '\0') {
		if (token[i] == ',') {
			num[j] = '\0';
			if (!isImmediate(num)) {
				return false;
			}
			numOfInitValues++;
			j = 0;
			i++;
		}
		else {
			num[j] = token[i];
			i++;
			j++;
		}
	}
	num[j] = '\0';

	if (!isImmediate(num))
		return false;
	numOfInitValues++;

	return numOfInitValues;
}


bool Assembler::isLineValid(char* cstr) {

	removeSpaces(cstr);


	if (isSection()) {
		lineType = SECTION;
		delete cstr;
		return true;
	}

	if (isDirective()) {
		lineType = DIRECTIVE;
		delete cstr;
		return true;
	}

	if (isLabel()) {
		//lineType is set as side effect of the upper method
		labelExist = true;
		delete cstr;
		return true;
	}

	//else: line is instruction without, with one or with two operands - or nothing at all

	lineType = INSTRUCTION;

	if (tokenNum == 1) {
		return isInstruction(firstToken);
	}


	if (tokenNum == 2) {
		return isInstruction(firstToken, secondToken);
	}


	if (tokenNum == 3) {
		return isInstruction(firstToken, secondToken, thirdToken);
	}


}


void Assembler::handleValidLineFirstPass() {
	int i = 0;
	char* symTableKey = new char[MAX_TOKEN_SIZE];

	switch (lineType) {
	case SECTION:
		labelExist = false;

		switch (section) {
		case RODATA:
			symTableKey = (char*) ".rodata";
			//locationCounter = &rodataLC;
			break;
		case DATA:
			symTableKey =(char*) ".data";
			break;
		case BSS:
			symTableKey = (char*)  ".bss";
			//locationCounter = &bssLC;
			break;
		case TEXT:
			symTableKey = (char*)  ".text";
			//locationCounter = &textLC;
			break;
		}

		//*locationCounter = 0;

		//check if the section already exists in the symbol table

		if (symTable.hasEntry(symTableKey)) {

			cerr << "Error: section:" << symTableKey << " has already been declared";
			exit(-1);

		}
		else {
			STEntry newEntry = symTable.makeEntry(symTableKey,
				section, globLC, true, tableIndex++);
			symTable.addEntry(newEntry);

			tokenTable.push_back(new TokenTableEntry(SECTION, secondToken, (char*)"", (char*)""));
		}

		break;
		//end of SECTION

	case DIRECTIVE:

		if (directive == CHAR || directive == WORD || directive == LONG) {
			if (!labelExist) {
				cerr << "Error in handling line:" << lineNum << "There must be a LABEL before char,word or long";
				exit(-1);
			}
		}
		switch (directive) {
		case EQU:
			i = 0;
			while (equsym[i] != '\0') {
				symTableKey[i] = equsym[i];
				i++;
			}
			symTableKey[i] = '\0';			
			if (symTable.hasEntry(symTableKey)) {
				cerr << "Error: symbol:" << symTableKey << " has already been defined";
				exit(-1);
			}
			else {				
				STEntry newEntry = symTable.makeEntry(symTableKey,
					section, immedVal, true, tableIndex++);
				newEntry.setConst(true);
				symTable.addEntry(newEntry);
			}
			break;
		case GLOBAL:
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case EXTERN:
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case CHAR:
			globLC += numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case WORD:
			globLC += 2 * numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case LONG:
			globLC += 4 * numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case ALIGN:
			if ((globLC % 2) == 1)
				globLC++;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, (char*)"",(char*)""));
			break;
		case SKIP:
			globLC += skipValue;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, firstToken, secondToken, (char*)""));
			break;
		case END:
			endOfFile = true;
			switch (section) {
			case TEXT:
				textSectionSize = globLC - textSA;
				break;
			case DATA:
				dataSectionSize = globLC - dataSA;
				break;
			case RODATA:
				rodataSectionSize = globLC - rodataSA;
				break;
			case BSS:
				bssSectionSize = globLC - bssSA;
				break;
			default:
				break;
			}
			break;
		default:
			cerr << "Error in handling line:" << lineNum;
			exit(-1);
		}

		break;
		//end of DIRECTIVE


	case INSTRUCTION:

		if (section != TEXT) {
			cerr << "Error in handling line:" << lineNum << ": Instructions must be in .text section";
			exit(-1);
		}

		globLC += instructionSize;

		tokenTable.push_back(new TokenTableEntry(INSTRUCTION, firstToken, secondToken, thirdToken));

		break;
		//end of instruction

	case LABEL:
		labelExist = true;

		i = 0;
		while (label[i] != '\0') {
			symTableKey[i] = label[i];
			i++;
		}
		symTableKey[i - 1] = '\0';

		if (symTable.hasEntry(symTableKey)) {

			cerr << "Error: symbol:" << symTableKey << " has already been defined";
			exit(-1);

		}
		else {
			STEntry newEntry = symTable.makeEntry(symTableKey,
				section, globLC, true, tableIndex++);
			symTable.addEntry(newEntry);
		}



		break;
		//end of LABEL

	case LABEL_AND_DIRECTIVE:
		labelExist = true;

		i = 0;
		while (label[i] != '\0') {
			symTableKey[i] = label[i];
			i++;
		}
		symTableKey[i - 1] = '\0';

		if (symTable.hasEntry(symTableKey)) {

			cerr << "Error: symbol:" << symTableKey << " has already been defined";
			exit(-1);

		}
		else {
			STEntry newEntry = symTable.makeEntry(symTableKey,
				section, globLC, true, tableIndex++);
			symTable.addEntry(newEntry);
		}

		//directive part
		if (directive == CHAR || directive == WORD || directive == LONG) {
			if (!labelExist) {
				cerr << "Error in handling line:" << lineNum << "There must be a LABEL before char,word or long";
				exit(-1);
			}
		}
		switch (directive) {
		case CHAR:
			globLC += numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, secondToken, thirdToken, (char*)""));
			break;
		case WORD:
			globLC += 2 * numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, secondToken, thirdToken, (char*)""));
			break;
		case LONG:
			globLC += 4 * numOfInitValues;
			tokenTable.push_back(new TokenTableEntry(DIRECTIVE, secondToken, thirdToken, (char*)""));
			break;
		case ALIGN:
			if ((globLC % 2) == 1)
				globLC++;
			break;
		case SKIP:
			globLC += skipValue;
			break;
		case END:
			endOfFile = true;
			break;
		default:
			cerr << "Error in handling line:" << lineNum;
			exit(-1);
		}
		break;
		//end of LABEL_AND_DIRECTIVE

	case LABEL_AND_INSTRUCTION:
		labelExist = true;

		i = 0;
		while (label[i] != '\0') {
			symTableKey[i] = label[i];
			i++;
		}
		symTableKey[i - 1] = '\0';

		if (symTable.hasEntry(symTableKey)) {

			cerr << "Error: symbol:" << symTableKey << " has already been defined";
			exit(-1);

		}
		else {
			STEntry newEntry = symTable.makeEntry(symTableKey,
				section, globLC, true, tableIndex++);
			symTable.addEntry(newEntry);
		}

		//instruction part
		if (section != TEXT) {
			cerr << "Error in handling line:" << lineNum << ": Instructions must be in .text section";
			exit(-1);
		}
		globLC += instructionSize;
		tokenTable.push_back(new TokenTableEntry(INSTRUCTION, secondToken, thirdToken, fourthToken));
		break;
		//end of LABEL_AND_INSTRUCTION


	default:
		cerr << "Error in handling line:" << lineNum;
		exit(-1);
		break;

	}

}

void Assembler::firstPass(string inputFile) {

	ifstream infile(inputFile.c_str());
	string line;
	const char* temp;
	char* cstr;

	if (infile.is_open()) {

		while (getline(infile, line) && !endOfFile) {

			if (line.empty()) {
				lineNum++;
				continue;
			}

			temp = line.c_str();
			cstr = new char[line.length() + 1];
			memcpy(cstr, temp, line.length() + 1);


			removeSpaces(cstr);

			if (cstr[0] == '\0') {
				lineNum++;
				continue;
			}
			extractTokens(cstr);



			if (!isLineValid(cstr)) {
				cout << "Error in line " << lineNum << endl;
				exit(-1);
				break;
			}


			handleValidLineFirstPass();

			flushTokens();

			lineNum++;

		}

		//printSymTable();

	}
	else {
		cout << "Error handling input file!";
		exit(-1);
	}

}


void Assembler::printSymTable() {
	symTable.print();
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------



//SECOND PASS

void Assembler::secondPass() {
	textSection = new char[textSectionSize];
	dataSection = new char[dataSectionSize];
	rodataSection = new char[rodataSectionSize];
	bssSection = new char[bssSectionSize];

	globLC = 0;


	/*
	textLC = 0;
	dataLC = 0;
	rodataLC = 0;
	bssLC = 0;
	*/

	//machine code byte
	char byte;

	//machine code byte in a hexadecimal form
	char* temp = new char[3];
	temp[2] = '\0';


	TokenTableEntry * entry;
	for (list<TokenTableEntry*>::iterator it = tokenTable.begin();
		it != tokenTable.end(); it++) {
		entry = *it;

		//HANDLING SECTION
		if (entry->getType() == SECTION) {
			if (strcmp(entry->getFirstToken(), "\".text\"") == 0) {
				section = TEXT;
				//currSection = textSection;
				//currElfSection = &elfTextSection;
				//locationCounter = &textLC;
			}
			else
				if (strcmp(entry->getFirstToken(), "\".rodata\"") == 0) {
					section = RODATA;
					//currSection = rodataSection;
					//currElfSection = &elfRodataSection;
					//locationCounter = &rodataLC;
				}
				else
					if (strcmp(entry->getFirstToken(), "\".data\"") == 0) {
						section = DATA;
						//currSection = dataSection;
						//currElfSection = &elfDataSection;
						//locationCounter = &dataLC;
					}
					else
						if (strcmp(entry->getFirstToken(), "\".bss\"") == 0) {
							section = BSS;
							//currSection = bssSection;
							//currElfSection = &elfBssSection;
							//locationCounter = &bssLC;
						}
						else if (strcmp(entry->getFirstToken(), "\".align\"") == 0) {
						}
		}


		//HANDLING DIRECTIVE
		else if (entry->getType() == DIRECTIVE) {
			if (strcmp(entry->getFirstToken(), ".extern") == 0) {
				list<char*> symbols = extractSymbols(entry->getSecondToken());
				for (list<char*>::iterator iter = symbols.begin(); iter != symbols.end(); ++iter) {
					char* s = *iter;
					if (symTable.hasEntry(s)) {
						exit(1);
					}
				}
			}
			else
			if (strcmp(entry->getFirstToken(), ".global") == 0) {
				list<char*> symbols = extractSymbols(entry->getSecondToken());
				for (list<char*>::iterator iter = symbols.begin(); iter != symbols.end(); ++iter) {
					char* s = *iter;

					if (symTable.hasEntry(s)) {
						STEntry* ste = symTable.getEntryPointer(s);
						ste->setLocal(false);
					}

				}
			}
			else
			if (strcmp(entry->getFirstToken(), ".align") == 0) {
				if (globLC % 2 == 1) {
					byte = 0;
					temp = transformByteToHex(byte);
					currElfSection->push_back(temp);					
					++globLC;
				}
			}

			else
			if (strcmp(entry->getFirstToken(), ".skip") == 0) {
				char* nstring = entry->getSecondToken();
				int n = atoi(nstring);
				for (int i = 0; i < n; i++) {
					byte = 0;
					temp = transformByteToHex(byte);
					currElfSection->push_back(temp);
					++globLC;
				}
			}
			else {
				list<int> initValues = extractNumbers(entry->getSecondToken());

				if (strcmp(entry->getFirstToken(), ".char") == 0) {
					for (list<int>::iterator iter = initValues.begin(); iter!=initValues.end(); ++iter) {
						//currSection[locationCounter] = initValues[i] & 0xFF;

						byte = *iter & 0xFF;

						temp = transformByteToHex(byte);

						currElfSection->push_back(temp);

						globLC++;
					}
				}
				else if (strcmp(entry->getFirstToken(), ".word") == 0) {
					for (list<int>::iterator iter = initValues.begin(); iter != initValues.end(); ++iter) {
						//currSection[locationCounter] = initValues[i] & 0xFF;				//lower byte
						byte = *iter & 0xFF;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						//currSection[locationCounter] = (initValues[i] & 0xFF00) >> 8;		//upper byte
						byte = (*iter & 0xFF00) >> 8;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						globLC += 2;
					}
				}
				else if (strcmp(entry->getFirstToken(), ".long") == 0) {
					for (list<int>::iterator iter = initValues.begin(); iter != initValues.end(); ++iter) {
						//first word
						//currSection[locationCounter] = initValues[i] & 0xFF;		//lower byte
						byte = *iter & 0xFF;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						//currSection[locationCounter] = (initValues[i] & 0xFF00) >> 8;		//upper byte
						byte = (*iter & 0xFF00) >> 8;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						//second word
						//currSection[locationCounter] = (initValues[i] & 0xFF0000) >> 16;		//lower byte
						byte = (*iter & 0xFF0000) >> 16;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						//currSection[locationCounter] = (initValues[i] & 0xFF000000) >> 24;		//upper byte
						byte = (*iter & 0xFF000000) >> 24;
						temp = transformByteToHex(byte);
						currElfSection->push_back(temp);
						globLC += 4;
					}
				}
			}
			continue;
		}

		//HANDLING INSTRUCTION
		else if (entry->getType() == INSTRUCTION) {
			char* codedInstr;

			if (entry->getSecondToken()[0] == '\0') {
				codedInstr = codeInstruction(entry->getFirstToken());
			}
			else if (entry->getThirdToken()[0] == '\0') {
				codedInstr = codeInstruction(entry->getFirstToken(), entry->getSecondToken());
			}
			else {
				codedInstr = codeInstruction(entry->getFirstToken(), entry->getSecondToken(), entry->getThirdToken());
			}


			for (int i = 0; i < instructionSize; i++) {
				//currSection[locationCounter] = codedInstr[i];
				//*(locationCounter)++;
				byte = codedInstr[i];				
				temp = transformByteToHex(byte);
				currElfSection->push_back(temp);
			}


			globLC += instructionSize;

		}



	}
}


char* Assembler::codeInstruction(char* ft) {
	//iret is only instruction without operands
	char* codedInstr = new char[1];
	codedInstr[0] = 0;
	unsigned opcode;

	if (strcmp(ft, "iret")) {
		opcode = 25;
	}
	else {
		opcode = 24;
	}

	codedInstr[0] = (opcode << 3);


	instructionSize = 1;

	return codedInstr;
}



void prepareString(char* dst, char* src) {

	int i = 0;
	char* temp = dst;
	while (src[i] != '[') {
		i++;
	}

	int j = 0;
	++i;
	while (src[i] != ']') {
		temp[j] = src[i];
		++i;
		++j;
	}
	temp[j] = '\0';


}



char* Assembler::codeInstruction(char* ft, char* st) {
	char* codedInstr = new char[5];
	codedInstr[0] = codedInstr[1] = codedInstr[2] = codedInstr[3] = 0;
	codedInstr[4] = '\0';


	unsigned opcode;
	unsigned opersize = 1;

	unsigned dstAddrType;
	unsigned srcAddrType;

	unsigned dstRegNum;
	unsigned srcRegNum;

	char* s = new char[MAX_TOKEN_SIZE];

	//operand size

	int i = 0;
	while (ft[i] != '\0')
		i++;

	char osize = ft[i - 1];
	if (osize == 'w') {
		opersize = 1;
	}

	if (osize == 'b') {
		opersize = 0;
	}


	unsigned regusage = 0;

	if (strcmp(ft, "push") == 0 || strcmp(ft, "pushb") == 0 || strcmp(ft, "pushw") == 0) {

		opcode = 17;

		codedInstr[0] = (opcode << 3);
		codedInstr[0] |= opersize << 2;

		unsigned lowhigh = 0;
		if (isRegister(st)) {
			//if operand is one byte
			if (opersize == 0) {
				i = 0;
				while (st[i] != '\0') {
					i++;
				}
				char h = st[i - 1];
				if (h == 'h') {
					lowhigh = 1;
				}
			}

			instructionSize = 2;
			srcAddrType = 1;
			srcRegNum = st[1] - '0';
			codedInstr[1] |= (srcAddrType << 5);
			codedInstr[1] |= srcRegNum << 1;
			codedInstr[1] |= lowhigh;
			return codedInstr;
		}



		if (isImmediate(st)) {
			srcAddrType = 0;
			codedInstr[1] |= srcAddrType << 5;
			if (adrType == IMMEDIATE_VALUE || adrType == IMMEDIATE_VALUE_HEX) {

				if (opersize == 0) {
					codedInstr[2] = immedVal & 0xFF;
					instructionSize = 3;
				}
				else {
					codedInstr[2] = immedVal & 0xFF;
					codedInstr[3] = (immedVal & 0xFF00) >> 8;
					instructionSize = 4;
				}
			}
			else {
				//make rellocation record for linker
				//errror
				exit(-1);
			}
			return codedInstr;
		}

		if (isMemoryLocation(st)) {
			switch (adrType) {
			case MEM_DIRECT_VALUE:
				instructionSize = 4;
				srcAddrType = 5;
				codedInstr[1] |= (srcAddrType << 5);
				codedInstr[2] = immedVal & 0xFF;
				codedInstr[3] = (immedVal & 0xFF00) >> 8;
				break;
			case MEM_DIRECT_SYMBOL:
			{	
			srcAddrType = 5;
			codedInstr[1] |= (srcAddrType << 5);

			STEntry ste = symTable.getEntry(sym_);
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isConst()) {
				//value actually
				int val = ste.getOffset();
				srcAddrType = 0;
				codedInstr[1] |= srcAddrType << 5;
				if (opersize == 0) {
					codedInstr[2] = val & 0xFF;
					instructionSize = 3;
				}
				else {
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					instructionSize = 4;
				}
			}
			else {								
				instructionSize = 4;
				copyString(s, sym_);

				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, (char*)""));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
			}
			break; 
			}
			case REG_INDIRECT_VALUE: case REG_INDIRECT_ZERO: case REG_INDIRECT_8: case REG_INDIRECT_16:

				if (immedVal == 0) {
					srcAddrType = 2;
				}
				else if (-128 < immedVal < 127) {
					srcAddrType = 3;
				}
				else if (-32768 < immedVal < 32767) {
					srcAddrType = 4;
				}
				else {
					exit(-1);
				}


				srcRegNum = st[1] - '0';
				codedInstr[1] |= (srcAddrType << 5);
				codedInstr[1] |= srcRegNum << 1;
				codedInstr[1] |= lowhigh;
				codedInstr[2] = immedVal & 0xFF;
				codedInstr[3] = (immedVal & 0xFF00) >> 8;

				break;

			case REG_INDIRECT_SYMBOL:
			{srcAddrType = 4;
			srcRegNum = st[1] - '0';
			codedInstr[1] |= (srcAddrType << 5);
			codedInstr[1] |= srcRegNum << 1;
			codedInstr[1] |= lowhigh;
			instructionSize = 4;

			char* temp = new char[MAX_TOKEN_SIZE];
			prepareString(temp, sym_);
			
			
			STEntry ste = symTable.getEntry(temp);
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isConst()) {
				//value actually
				int val = ste.getOffset();
				codedInstr[2] = val & 0xFF;
				codedInstr[3] = (val & 0xFF00) >> 8;
				instructionSize = 4;
			}
			else {
				instructionSize = 4;
				copyString(s, sym_);

				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, (char*)""));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
			
			
			}
			break;
			}
			default:
				exit(-1);

			}
			return codedInstr;
		}

	} //END PUSH


	if (strcmp(ft, "pop") == 0 || strcmp(ft, "popb") == 0 || strcmp(ft, "popw") == 0) {

		opcode = 18;

		codedInstr[0] = (opcode << 3);

		codedInstr[0] |= opersize << 2;

		//reg direct
		if (isRegister(st)) {
			instructionSize = 2;
			dstAddrType = 1;
			dstRegNum = st[1] - '0';
			codedInstr[1] |= dstAddrType << 5;
			codedInstr[1] |= (dstRegNum << 1);
			codedInstr[1] |= regusage;
			return codedInstr;
		}


		instructionSize = 4;

		if (isMemoryLocation(st)) {
			switch (adrType) {
			case MEM_DIRECT_VALUE:
				dstAddrType = 5;
				codedInstr[1] |= dstAddrType << 5;
				codedInstr[2] = immedVal & 0xFF;
				codedInstr[3] = (immedVal & 0xFF00) >> 8;
				break;
			case MEM_DIRECT_SYMBOL:
			{
				dstAddrType = 5;
				codedInstr[1] |= dstAddrType << 5;

				STEntry ste = symTable.getEntry(sym_);

				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && symTable.getEntry(sym_).isConst())
					exit(1);


				copyString(s, sym_);

				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}

				break;
			}
			case REG_INDIRECT_VALUE: case REG_INDIRECT_ZERO: case REG_INDIRECT_8: case REG_INDIRECT_16:
				if (immedVal == 0) {
					srcAddrType = 2;
				}
				else if (-128 < immedVal < 127) {
					srcAddrType = 3;
					codedInstr[2] = immedVal & 0xFF;
					instructionSize = 3;
				}
				else if (-32768 < immedVal < 32767) {
					srcAddrType = 4;
					codedInstr[2] = immedVal & 0xFF;
					codedInstr[3] = (immedVal & 0xFF00) >> 8;
					instructionSize = 4;
				}
				else {
					exit(-1);
				}

				srcRegNum = st[1] - '0';
				codedInstr[1] |= (srcAddrType << 5);
				codedInstr[1] |= srcRegNum << 1;
				codedInstr[1] |= regusage;

				break;

			case REG_INDIRECT_SYMBOL:
			{srcAddrType = 4;
			srcRegNum = st[1] - '0';
			codedInstr[1] |= (srcAddrType << 5);
			codedInstr[1] |= srcRegNum << 1;
			instructionSize = 4;

			char* temp = new char[MAX_TOKEN_SIZE];
			prepareString(temp, sym_);

			STEntry ste = symTable.getEntry(temp);
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isConst()) {
				//value actually
				int val = ste.getOffset();
				codedInstr[2] = val & 0xFF;
				codedInstr[3] = (val & 0xFF00) >> 8;
				instructionSize = 4;
			}
			else {				
				copyString(s, temp);

				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
			
			}
			break;
			}
			default:
				exit(-1);
			}
			return codedInstr;

		}
	}
		// END POP
	unsigned opcode_ = 0;

	if (strcmp(ft, "jmp") == 0 || strcmp(ft, "jne") == 0 || strcmp(ft, "jeq") == 0 || strcmp(ft, "jgt") == 0) {

			if (strcmp(ft, "jmp") == 0) {
				opcode_ = 19;
			}

			if (strcmp(ft, "jeq") == 0) {
				opcode_ = 20;
			}

			if (strcmp(ft, "jne") == 0) {
				opcode_ = 21;
			}

			if (strcmp(ft, "jgt") == 0) {
				opcode_ = 22;
			}

			codedInstr[0] = (opcode_ << 3);

			srcAddrType = 5;
			codedInstr[1] = srcAddrType << 5;

			instructionSize = 4;
			

			if (isValidSymbolName(st)) {
				
				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && symTable.getEntry(sym_).isConst())
					exit(-1);
				
				
				copyString(s, sym_);

				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
			}

			else if (isPCRelative(st)) {
				//if symbol is local - solve it now

				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0) {
					int dst = symTable.getEntry(sym_).getOffset();
					int ofst = dst - (globLC + 4);

					codedInstr[2] = ofst & 0xFF;
					codedInstr[3] = (ofst & 0xFF00) >> 8;

				}
				else {
					int val = -2;
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					
					
					copyString(s, sym_);

					STEntry ste = symTable.getEntry(sym_);
					if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
						val = ste.getOffset();
						val -= 2;
						codedInstr[2] = val & 0xFF;
						codedInstr[3] = (val & 0xFF00) >> 8;
						relocationTable.push_back(new RelocationTableEntry(globLC + 2, REL, s));
					}
					else {
						relocationTable.push_back(new RelocationTableEntry(globLC + 2, REL, s));
					}
				}
			}
			else {
				exit(-1);
			}

			


			return codedInstr;

		}


		if (strcmp(ft, "call") == 0) {

			opcode = 23;

			codedInstr[0] = (opcode << 3);



			if (isRegister(st)) {
				instructionSize = 2;
				srcAddrType = 1;
				srcRegNum = st[1] - '0';
				codedInstr[1] = (srcAddrType << 5);
				codedInstr[1] |= srcRegNum << 1;
				return codedInstr;
			}

			if (isValidSymbolName(st)) {
				instructionSize = 4;
				srcAddrType = 5;
				codedInstr[1] |= (srcAddrType << 5);
				
				
				copyString(s, sym_);
				
				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
				
				return codedInstr;
			}
		}// END CALL




}



char* Assembler::codeInstruction(char* ft, char* st, char* tt) {
	unsigned opcode;

	unsigned dstAddrType;
	unsigned srcAddrType;

	unsigned dstRegNum;
	unsigned srcRegNum;

	char* s = new char[MAX_TOKEN_SIZE];


	char* codedInstr = new char[5];
	codedInstr[0] = codedInstr[1] = codedInstr[2] = codedInstr[3] = 0;
	codedInstr[4] = '\0';

	//----------------------------------------------------------------------

	int i = 0;
	while (ft[i] != '\0') {
		i++;
	}
	char w = ft[i - 1];

	int opersize = -1;

	if (w == 'w') {
		opersize = 1;
	}
	else if (w == 'b' && !(strcmp(ft, "sub") == 0 || strcmp(ft, "subb") == 0)) {   //opcode for sub
		opersize = 0;
	}

	if (w == 'b' && ft[i - 2] == 'b') {
		opersize = 0;
	}

	if (opersize != -1) {
		ft[i - 1] = '\0';
	} 



	if (strcmp(ft, "xchg") == 0) {
		opcode = 2;
	}

	if (strcmp(ft, "add") == 0) {
		opcode = 5;
	}

	if (strcmp(ft, "sub") == 0) {
		opcode = 6;
	}

	if (strcmp(ft, "mul") == 0 )
	{
		opcode = 7;
	}

	if (strcmp(ft, "div") == 0)
	{
		opcode = 8;
	}

	if (strcmp(ft, "cmp") == 0 )
	{
		opcode = 9;
	}

	if (strcmp(ft, "and") == 0 )
	{
		opcode = 11;
	}

	if (strcmp(ft, "or") == 0)
	{
		opcode = 12;
	}


	if (strcmp(ft, "xor") == 0) {
		opcode = 13;
	}

	if (strcmp(ft, "not") == 0)
	{
		opcode = 10;
	}

	if (strcmp(ft, "test") == 0)
	{
		opcode = 14;
	}

	if (strcmp(ft, "mov") == 0)
	{
		opcode = 4;
	}

	if (strcmp(ft, "shl") == 0)
	{
		opcode = 15;
	}

	if (strcmp(ft, "shr") == 0)
	{
		opcode = 16;
	}

	//-----------------------------------------------------------------
	//-----------------------------------------------------------------

	if (opersize == -1) {
		opersize = 1;
	}


	codedInstr[0] = opcode << 3;
	codedInstr[0] |= opersize << 2;
	
	//dst
	unsigned lowhigh = 0;
	if (isRegister(st)) {

		//is regh or regl
		if (opersize == 0) {
			i = 0;
			while (st[i] != '\0') {
				i++;
			}
			char h = st[i - 1];
			if (h == 'h') {
				lowhigh = 1;
			}
		}


		dstAddrType = 1;
		dstRegNum = st[1] - '0';
		codedInstr[1] |= dstAddrType << 5;
		codedInstr[1] |= (dstRegNum << 1);
		codedInstr[1] |= lowhigh;
		//src
		if (isRegister(tt)) {
			instructionSize = 3;

			if (opersize == 0) {
				i = 0;
				while (tt[i] != '\0') {
					i++;
				}
				char h = tt[i - 1];
				if (h == 'h') {
					lowhigh = 1;
				}
			}

			srcAddrType = 1;
			srcRegNum = tt[1] - '0';
			codedInstr[2] |= (srcAddrType << 5);
			codedInstr[2] |= srcRegNum << 1;
			codedInstr[2] |= lowhigh;
			return codedInstr;
		}

		//src
		if (isImmediate(tt)) {

			srcAddrType = 0;
			codedInstr[2] |= (srcAddrType << 5);
			if (adrType == IMMEDIATE_VALUE || adrType == IMMEDIATE_VALUE_HEX ) {

				if (opersize == 0) {
					codedInstr[3] = immedVal & 0xFF;
					instructionSize = 4;
				}
				else {
					codedInstr[3] = immedVal & 0xFF;
					codedInstr[4] = (immedVal & 0xFF00) >> 8;
					instructionSize = 5;
				}

			}
			else {
				
				copyString(s, sym_);

				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[3] = val & 0xFF;
					codedInstr[4] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 3, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 3, ABS, s));
				}
			}
			return codedInstr;
		}

		//src - memory location
		if (isDirectMemoryAccess(tt)) {
			instructionSize = 5;
			srcAddrType = 5;
			codedInstr[2] |= (srcAddrType << 5);
			if (adrType == MEM_DIRECT_VALUE) {
				codedInstr[3] = immedVal & 0xFF;
				codedInstr[4] = (immedVal & 0xFF00) >> 8;
			}
			else {
				
				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(),"_empty_entry_") !=0 && symTable.getEntry(sym_).isConst()) {
					int val = ste.getOffset();
					srcAddrType = 0;
					codedInstr[2] = (srcAddrType << 5);
					codedInstr[3] = val & 0xFF;
					codedInstr[4] = (val & 0xFF00) >> 8;
				}
				else {					
					copyString(s, sym_);

					STEntry ste = symTable.getEntry(sym_);
					if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
						int val = ste.getOffset();
						codedInstr[3] = val & 0xFF;
						codedInstr[4] = (val & 0xFF00) >> 8;
						relocationTable.push_back(new RelocationTableEntry(globLC + 3, OFST, s));
					}
					else {
						relocationTable.push_back(new RelocationTableEntry(globLC + 3, ABS, s));
					}
				
				}
			}
			return codedInstr;
		}

		if (isRegisterIndirectAcess(tt)) {


			if (adrType == REG_INDIRECT_SYMBOL) {
				srcAddrType = 4;
				srcRegNum = tt[1] - '0';
				codedInstr[2] |= (srcAddrType << 5);
				codedInstr[2] |= srcRegNum << 1;
				codedInstr[2] |= lowhigh;

				instructionSize = 5;

				char* temp = new char[MAX_TOKEN_SIZE];
				prepareString(temp, sym_);
				
				STEntry ste = symTable.getEntry(temp);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isConst()) {
					int val = ste.getOffset();					
					codedInstr[3] = val & 0xFF;
					codedInstr[4] = (val & 0xFF00) >> 8;
				}				
				else {
					
					copyString(s, temp);

					STEntry ste = symTable.getEntry(temp);
					if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
						int val = ste.getOffset();
						codedInstr[3] = val & 0xFF;
						codedInstr[4] = (val & 0xFF00) >> 8;
						relocationTable.push_back(new RelocationTableEntry(globLC + 3, OFST, s));
					}
					else {
						relocationTable.push_back(new RelocationTableEntry(globLC + 3, ABS, s));
					}
				}
				
			}

			if (adrType == REG_INDIRECT_VALUE || adrType == REG_INDIRECT_ZERO || adrType == REG_INDIRECT_8 || adrType == REG_INDIRECT_16) {

				if (immedVal == 0) {
					srcAddrType = 2;
					instructionSize = 3;
				}
				else if (immedVal>= -128 && immedVal <= 127) {
					srcAddrType = 3;
					codedInstr[3] = immedVal & 0xFF;
					instructionSize = 4;
				}
				else if (immedVal >= -32768 && immedVal <= 32767) {
					srcAddrType = 4;
					codedInstr[3] = immedVal & 0xFF;
					codedInstr[4] = (immedVal & 0xFF00) >> 8;
					instructionSize = 5;
				}
				else {
					exit(-1);
				}

				srcRegNum = tt[1] - '0';
				codedInstr[2] |= (srcAddrType << 5);
				codedInstr[2] |= srcRegNum << 1;

			}

			return codedInstr;
		}


	}

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------


	

	//dst
	if (isDirectMemoryAccess(st)) {
		dstAddrType = 5;
		codedInstr[1] |= dstAddrType << 5;
		if (adrType == MEM_DIRECT_VALUE) {
			codedInstr[2] = immedVal & 0xFF;
			codedInstr[3] = (immedVal & 0xFF00) >> 8;
		}
		else {
			STEntry ste = symTable.getEntry(sym_);
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && symTable.getEntry(sym_).isConst())
				exit(1);
			
			
			copyString(s, sym_);
			
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
				int val = ste.getOffset();
				codedInstr[2] = val & 0xFF;
				codedInstr[3] = (val & 0xFF00) >> 8;
				relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
			}
			else {
				relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
			}
		
		}

		if (isRegister(tt)) {

			if (opersize == 0) {
				i = 0;
				while (tt[i] != '\0') {
					i++;
				}
				char h = tt[i - 1];
				if (h == 'h') {
					lowhigh = 1;
				}
			}
			
			
			srcAddrType = 1;
			srcRegNum = tt[1] - '0';
			codedInstr[4] |= (srcAddrType << 5);
			codedInstr[4] |= srcRegNum << 1;
			codedInstr[4] |= lowhigh;
			instructionSize = 5;
		}

		if (isImmediate(tt)) {

			srcAddrType = 0;
			codedInstr[4] |= (srcAddrType << 5);
			if (adrType == IMMEDIATE_VALUE || adrType == IMMEDIATE_VALUE_HEX) {

				if (opersize == 0) {
					codedInstr[5] = immedVal & 0xFF;
					instructionSize = 6;
				}
				else {
					codedInstr[5] = immedVal & 0xFF;
					codedInstr[6] = (immedVal & 0xFF00) >> 8;
					instructionSize = 7;
				}

			}
			else {
				
				copyString(s, sym_);
				
				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[5] = val & 0xFF;
					codedInstr[6] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 5, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 5, ABS, s));
				}
			}
			
		}


		return codedInstr;
	}



	//dst

	if (isRegisterIndirectAcess(st)) {
		

		if (adrType == REG_INDIRECT_SYMBOL) {
			dstAddrType = 4;
			dstRegNum = st[1] - '0';
			codedInstr[1] |= (dstAddrType << 5);
			codedInstr[1] |= dstRegNum << 1;

			char* temp = new char[MAX_TOKEN_SIZE];
			prepareString(temp, sym_);

			STEntry ste = symTable.getEntry(temp);
			if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isConst()) {
				int val = ste.getOffset();
				codedInstr[2] = val & 0xFF;
				codedInstr[3] = (val & 0xFF00) >> 8;
			}
			else {				
				copyString(s, temp);
				
				STEntry ste = symTable.getEntry(temp);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[2] = val & 0xFF;
					codedInstr[3] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 2, ABS, s));
				}
			
			}
		}
		if (adrType == REG_INDIRECT_VALUE || adrType == REG_INDIRECT_ZERO || adrType == REG_INDIRECT_8 || adrType == REG_INDIRECT_16) {

			if (immedVal == 0) {
				dstAddrType = 2;
			}
			else if (immedVal >= -128 && immedVal <= 127) {
				dstAddrType = 3;
				codedInstr[2] = immedVal & 0xFF;
			}
			else if (immedVal >= -32768 && immedVal <= 32767) {
				dstAddrType = 4;
				codedInstr[2] = immedVal & 0xFF;
				codedInstr[3] = (immedVal & 0xFF00) >> 8;
			}
			else {
				exit(-1);
			}


			dstRegNum = st[1] - '0';
			codedInstr[1] |= dstAddrType << 5;
			codedInstr[1] |= dstRegNum << 1;

			
		}
		

		//src
		if (isRegister(tt)) {
			
			if (opersize == 0) {
				i = 0;
				while (tt[i] != '\0') {
					i++;
				}
				char h = tt[i - 1];
				if (h == 'h') {
					lowhigh = 1;
				}
			}


			

			srcAddrType = 1;
			srcRegNum = tt[1] - '0';
			

			if (dstAddrType == 2) {
				codedInstr[2] |= (srcAddrType << 5);
				codedInstr[2] |= srcRegNum << 1;
				codedInstr[2] |= lowhigh;
				instructionSize = 3;
			}
			else if (dstAddrType == 3) {
				codedInstr[3] |= (srcAddrType << 5);
				codedInstr[3] |= srcRegNum << 1;
				codedInstr[3] |= lowhigh;
				instructionSize = 4;
			}
			else if (dstAddrType == 4) {
				codedInstr[4] |= (srcAddrType << 5);
				codedInstr[4] |= srcRegNum << 1;
				codedInstr[4] |= lowhigh;
				instructionSize = 5;
			}
			
			
		}


		if (isImmediate(tt)) {

			srcAddrType = 0;

			int decr = 0;
			if (dstAddrType == 2) {
				decr = 2;
			}
			if (dstAddrType == 3) {
				decr = 1;
			}



			codedInstr[4 - decr] |= (srcAddrType << 5);
			if (adrType == IMMEDIATE_VALUE || adrType ==IMMEDIATE_VALUE_HEX) {

				if (opersize == 0) {
					codedInstr[5-decr] = immedVal & 0xFF;
					instructionSize = 6-decr;
				}
				else {
					codedInstr[5-decr] = immedVal & 0xFF;
					codedInstr[6- decr] = (immedVal & 0xFF00) >> 8;
					instructionSize = 7-decr;
				}

			}
			else {				
				copyString(s, sym_);
				
				STEntry ste = symTable.getEntry(sym_);
				if (strcmp(ste.getLabel(), "_empty_entry_") != 0 && ste.isLocal()) {
					int val = ste.getOffset();
					codedInstr[5 - decr] = val & 0xFF;
					codedInstr[6 - decr] = (val & 0xFF00) >> 8;
					relocationTable.push_back(new RelocationTableEntry(globLC + 5 - decr, OFST, s));
				}
				else {
					relocationTable.push_back(new RelocationTableEntry(globLC + 5 - decr, ABS, s));
				}

			}

		}

		return codedInstr;
	}


}


list<char*> Assembler::extractSymbols(char* token) {
	list<char*> symbols;

	int i = 0;
	int j = 0;
	char* s = new char[MAX_TOKEN_SIZE];
	
	char* s1 = new char[MAX_TOKEN_SIZE];

	s[0] = '\0';

	while (token[i] != '\0') {
		if (token[i] == ',') {
			s[j] = '\0';
			copyString(s1,s);
			symbols.push_back(s1);
			s1 = new char[MAX_TOKEN_SIZE];
			j = 0;
			i++;
		}
		else {
			s[j] = token[i];
			i++;
			j++;
		}
	}
	s[j] = '\0';
	symbols.push_back(s);
	

	return symbols;
}

list<int> Assembler::extractNumbers(char* initSeq) {
	char* num = new char[MAX_TOKEN_SIZE];
	num[0] = '\0';

	list<int> numbers;


	int i = 0;
	int j = 0;
	int k = 0;

	numOfInitValues = 0;

	bool isHex = false;

	while (initSeq[i] != '\0') {
		if (initSeq[i] == ',') {
			num[j] = '\0';
			
			if (isHex) {
				isHex = false;
				numbers.push_back(transformHexToDecimal(num));
				numOfInitValues++;
				k++;
				j = 0;
				i++;
				continue;
			}
			else {
				numbers.push_back(atoi(num));
				numOfInitValues++;
				k++;
				j = 0;
				i++;
			}
		}
		else {			
			if (initSeq[i] == '0' && initSeq[i + 1] == 'x') {
				isHex = true;
				++i;
				++i;
				continue;
			}
			else{	
				num[j] = initSeq[i];
				i++;
				j++;
			}
		}
	}
	num[j] = '\0';
	
	if (isHex)
		numbers.push_back(transformHexToDecimal(num));
	else
		numbers.push_back(atoi(num));
	
	numOfInitValues++;

	return numbers;

}

//----------------------------------elf shit


void Assembler::makeELFRelocationTable() {

	char* offst = new char[7];
	offst[7] = '\0';
	char* type;
	char* smb;

	RelocationTableEntry* e;

	for (list<RelocationTableEntry*>::iterator it = relocationTable.begin();
		it != relocationTable.end(); it++) {

		e = *it;

		//ofst
		offst = transformWordToHex(e->getOffset());


		//type
		if (e->getType() == ABS) {
			type = (char*) "ABS";
		}
		else if(e->getType()==REL){
			type = (char*) "REL";
		}
		else {
			type = (char*) "OFS";
		}

		//symname
		smb = e->getSymbol();

		char* newentry = new char[40];

		int i = 0;
		int j = 0;
		while (offst[i] != '\0') {
			newentry[j] = offst[i];
			i++;
			j++;
		}

		newentry[j++] = '\t';
		newentry[j++] = '\t';
		newentry[j++] = '\t';

		i = 0;
		while (type[i] != '\0') {
			newentry[j] = type[i];
			j++;
			i++;
		}

		newentry[j++] = '\t';
		newentry[j++] = '\t';
		newentry[j++] = '\t';
		newentry[j++] = '\t';

		i = 0;
		while (smb[i] != '\0') {
			newentry[j] = smb[i];
			j++;
			i++;
		}
		newentry[j] = '\0';


		elfRelocTable.push_back(newentry);

	}

}


void Assembler::makeELFSymbolTable() {

	char* symname;
	char* sec;
	char* val = new char[6];
	val[5] = '\0';
	char* visibility;

	map<string, STEntry> st = symTable.getTable();
	STEntry entry;

	for (map<string, STEntry>::iterator it = st.begin(); it != st.end(); it++) {
		entry = it->second;

		symname = entry.getLabel();

		switch (entry.getSection()) {
		case TEXT:
			sec = (char*) "text";
			break;
		case DATA:
			sec = (char*) "data";
			break;
		case RODATA:
			sec = (char*) "rodata";
			break;
		case BSS:
			sec = (char*) "bss";
			break;
		default:
			exit(-1);
		}


		val = transformWordToHex(entry.getOffset());


		if (entry.isLocal()) {
			visibility = (char*) "local";
		}
		else {
			visibility = (char*) "global";
		}


		char* newentry = new char[40];

		int i = 0;
		int j = 0;
		while (symname[i] != '\0') {
			newentry[j] = symname[i];
			i++;
			j++;
		}

		for (; i < 21; i++) {
			newentry[j++] = ' ';
		}



		i = 0;
		while (sec[i] != '\0') {
			newentry[j] = sec[i];
			j++;
			i++;
		}

		newentry[j++] = '\t';
		newentry[j++] = '\t';
		newentry[j++] = '\t';

		i = 0;

		/*unsigned tmp = atoi(val);
		if (tmp == 0) {
			newentry[j++] = '0';
			newentry[j++] = '0';
			newentry[j++] = '0';
			newentry[j++] = '0';
		}
		else {*/
		while (val[i] != '\0') {
			newentry[j] = val[i];
			j++;
			i++;
		}
		//}

		newentry[j++] = '\t';
		newentry[j++] = '\t';
		newentry[j++] = '\t';

		i = 0;
		while (visibility[i] != '\0') {
			newentry[j] = visibility[i];
			j++;
			i++;
		}
		newentry[j] = 0;

		elfSymTable.push_back(newentry);

	}

}


void Assembler::makeELF(string filename) {

	ofstream elf;
	elf.open(filename);

	if (elf.is_open()) {


		makeELFSymbolTable();
		makeELFRelocationTable();

		unsigned totalSectionSize = textSectionSize + dataSectionSize + rodataSectionSize + bssSectionSize;

		elf << "#Size " << totalSectionSize << endl << endl;

		char* tab = (char*) "\t\t\t";
		elf << "#Symbol table" << endl;

		elf << "#name" << "                " << "section" << "\t\t" << "value" << "\t\t\t" << "visibility" << endl;
		elf << "\n";

		for (list<char*>::iterator it = elfSymTable.begin(); it != elfSymTable.end(); it++) {
			elf << *it << endl;
		}

		elf << "\n\n";


		elf << "#Relocation table" << endl;

		elf << "#offset" << tab << "type" << tab << "symbol" << endl;
		elf << "\n";

		for (list<char*>::iterator it = elfRelocTable.begin(); it != elfRelocTable.end(); it++) {
			elf << *it << endl;
		}
		elf << "\n\n";

		elf << "#Section header (start, size)" << endl;
		elf << "#text      " << textSA << " " << textSectionSize << endl;
		elf << "#data      " << dataSA << " " << dataSectionSize << endl;
		elf << "#rodata    " << rodataSA << " " << rodataSectionSize << endl;
		elf << "#bss       " << bssSA << " " << bssSectionSize << endl;

		elf << endl;

		elf << "#Section zone" << endl;

		elf << "     | " << "00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F" << endl;

		elf << "-------------------------------------------------------------------------------------------------------";

		elf << endl;

		unsigned i = 0;
		unsigned j = 0;
		for (list<char*>::iterator it = globSection.begin(); it != globSection.end(); it++, i++) {

			if (i == 0) {
				elf << "0000 | ";
			}

			elf << *it << "  ";


			if (++j == 16) {
				j = 0;
				elf << endl;
				char* temp = new char[5];
				temp = transformWordToHex(i + 1);
				elf << temp << " | ";
			}

		}


		elf << endl;

		elf.close();



	}
	else {
		cerr << "Unable to open the output file\n";
		exit(-1);
	}


}


char* Assembler::transformByteToHex(char byte) {

	char* hexByte = new char[3];
	hexByte[2] = '\0';


	char lower, upper;
	char lowerHex, upperHex;

	lower = upper = lowerHex = upperHex = 0;

	lower = (unsigned)byte & 0xF;
	upper = ((unsigned)byte & 0xF0) >> 4;

	switch (lower)
	{
	case 0: case 1: case 2: case 3: case 4: case 5:
	case 6: case 7: case 8: case 9:
		lowerHex = '0' + lower;
		break;
	case 10: case 11: case 12: case 13: case 14: case 15:
		lowerHex = 'A' + (lower - 10);
	default:
		break;
	}


	switch (upper)
	{
	case 0: case 1: case 2: case 3: case 4: case 5:
	case 6: case 7: case 8: case 9:
		upperHex = '0' + upper;
		break;
	case 10: case 11: case 12: case 13: case 14: case 15:
		upperHex = 'A' + (upper - 10);
	default:
		break;
	}


	hexByte[0] = upperHex;
	hexByte[1] = lowerHex;

	return hexByte;

}


char* Assembler::transformWordToHex(unsigned word) {

	char *lowerByte, *upperByte;

	lowerByte = transformByteToHex(word & 0xFF);

	upperByte = transformByteToHex((word & 0xFF00) >> 8);

	char* hexWord = new char[5];
	hexWord[4] = '\0';

	hexWord[0] = upperByte[0];
	hexWord[1] = upperByte[1];
	hexWord[2] = lowerByte[0];
	hexWord[3] = lowerByte[1];

	return hexWord;

}

void Assembler::copyString(char* dst, char* src) {
	int i = 0;
	while (src[i] != '\0') {
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';

}


void Assembler::assemble(string inputFileName, string outputFileName) {

	firstPass(inputFileName);
	secondPass();
	makeELF(outputFileName);


}

void Assembler::freeSpace() {

	for (list<char*>::iterator it = globSection.begin(); it != globSection.end(); it++) {
		delete *it;
	}

	for (list<char*>::iterator it = elfRelocTable.begin(); it != elfRelocTable.end(); it++) {
		delete *it;
	}

	for (list<char*>::iterator it = elfSymTable.begin(); it != elfSymTable.end(); it++) {

	}

	for (list<TokenTableEntry*>::iterator it = tokenTable.begin(); it != tokenTable.end(); it++) {
		delete *it;
	}

	for (list<RelocationTableEntry*>::iterator it = relocationTable.begin(); it != relocationTable.end(); it++) {
		delete *it;
	}

	if (sym_ != nullptr)
		delete sym_;

	if (firstToken != nullptr)
		delete firstToken;
	if (secondToken != nullptr)
		delete secondToken;
	if (thirdToken != nullptr)
		delete thirdToken;
	if (fourthToken != nullptr)
		delete fourthToken;

}