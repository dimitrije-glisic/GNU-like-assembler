#ifndef DATA_TYPES_H
#define DATA_TYPES_H

enum LineType {
	SECTION, DIRECTIVE, LABEL, LABEL_AND_DIRECTIVE, LABEL_AND_INSTRUCTION, INSTRUCTION, INSTRUCTION_WITHOUT_OPERANDS,
	INSTRUCTION_WITH_ONE_OPERAND, INSTRUCTION_WITH_TWO_OPERANDS
};


enum Directive { CHAR, WORD, LONG, SKIP, ALIGN, END , GLOBAL,EXTERN ,EQU};

enum Section { RODATA, DATA, BSS, TEXT, dummy };


enum AddressingType {
	REG_DIRECT, MEM_DIRECT_VALUE, MEM_DIRECT_SYMBOL, IMMEDIATE_VALUE, IMMEDIATE_VALUE_HEX, IMMEDIATE_SYMBOL_VALUE,REG_INDIRECT_VALUE,
	REG_INDIRECT_ZERO, REG_INDIRECT_8, REG_INDIRECT_16, 
	REG_INDIRECT_SYMBOL, PC_RELATIVE
};


enum RelocationType {
	ABS,REL,OFST
};


struct RelocationTableEntry {
private:	
	char* _symbol;
	Section _section;
	unsigned _offset;
	RelocationType _type;
	
public:
	RelocationTableEntry(unsigned ofst, RelocationType t, char* sym) {
		_symbol = sym;
		_offset = ofst;
		_type = t;
	}

	char* getSymbol() {
		return _symbol;
	}

	Section getSection() {
		return _section;
	}

	unsigned getOffset() {
		return _offset;
	}

	RelocationType getType() {
		return _type;
	}

};

struct TokenTableEntry{
private:
	LineType type;
	char* _firstToken;
	char* _secondToken;
	char* _thirdToken;

public:

	TokenTableEntry(LineType t, char* ft, char* st, char* tt) {
		type = t;
		_firstToken = new char[21];
		_secondToken = new char[21];
		_thirdToken = new char[21];

		
		int i = 0;
		while (ft[i] != '\0') {
			_firstToken[i] = ft[i];
			i++;
		}
		_firstToken[i] = '\0';

		i = 0;
		while (st[i] != '\0') {
			_secondToken[i] = st[i];
			i++;
		}
		_secondToken[i] = '\0';

		i = 0;
		while (tt[i] != '\0') {
			_thirdToken[i] = tt[i];
			i++;
		}
		_thirdToken[i] = '\0';
		


	}

	~TokenTableEntry() {
		delete _firstToken;
		delete _secondToken;
		delete _thirdToken;
	}


	LineType getType() {
		return type;
	}

	char* getFirstToken() {
		return _firstToken;
	}

	char* getSecondToken() {
		return _secondToken;
	}

	char* getThirdToken() {
		return _thirdToken;
	}

};


#endif