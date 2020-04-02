#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <string>
#include <map>
#include <iostream>
#include "DataTypes.h"
using namespace std;


struct STEntry {
private:

	char* label;
	Section section;
	int offset;
	bool local;
	bool cnst;
	int index;

public:

	STEntry() {
		index = -1;
		label = (char*)"_empty_entry_";
	}

	STEntry(char* lab, Section s, int ofst, bool loc, int ind) {
		label = lab;
		section = s;
		offset = ofst;
		local = loc;
		index = ind;
		cnst= false;
	}

	char* getLabel() {
		return label;
	}

	Section getSection() {
		return section;
	}

	int getOffset() {
		return offset;
	}

	bool isLocal() {
		return local;
	}

	int getIndex() {
		return index;
	}

	bool isConst() {
		return cnst;
	}

	void setLocal(bool l) {
		local = l;
	}

	void setConst(bool c) {
		cnst = c;
	}


	void print() {
		cout << label << "\t\t" << section << "\t\t" << offset << "\t\t" << local << "\t\t" << index;
	}
};


class SymbolTable {

public:

	SymbolTable();

	void addEntry(STEntry entry);

	STEntry getEntry(string key);

	STEntry* getEntryPointer(string key);


	bool hasEntry(string key);

	STEntry makeEntry(char* label, Section section, int offset, bool local, int index);


	void print();


	map<string, STEntry> getTable() {
		return symbolTable;
	}

private:

	map<string,STEntry> symbolTable;


	
};




#endif