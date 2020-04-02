#include "SymbolTable.h"


SymbolTable::SymbolTable() {
	
}


STEntry SymbolTable::makeEntry(char* label, Section section, int offset, bool local, int index) {
	
	return STEntry(label, section, offset, local, index);

}


STEntry* SymbolTable::getEntryPointer(string key) {
	if (hasEntry(key))
		return &symbolTable[key];
	else
		return new STEntry();
}

STEntry SymbolTable::getEntry(string key) {
	if (hasEntry(key))
		return symbolTable[key];
	else
		return STEntry();
}



void SymbolTable::addEntry(STEntry entry) {

	symbolTable[entry.getLabel()] = entry;

}


void SymbolTable::print() {
	map<string, STEntry>::iterator it;
	STEntry entry;


	cout << "SYMBOL" << "\t\t" << "SECTION" << "\t\t" << "OFFSET" << "\t\t" << "LOCAL" << "\t\t" << "INDEX" << '\n';

	for (it = symbolTable.begin(); it != symbolTable.end(); it++) {
		it->second.print();
		cout << '\n';
	}

}


bool SymbolTable::hasEntry(string key) {
	map<string, STEntry>::iterator it = symbolTable.find(key);

	if (it == symbolTable.end())
		return false;
	else
		return true;

}