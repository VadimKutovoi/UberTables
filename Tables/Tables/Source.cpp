#include"Tables.h"
#include<string>
#include<fstream>
#include<iostream>
#include <windows.h>

void clrscr(void)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!Console) return;

	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(Console, &buf);

	DWORD Count;

	COORD zpos;
	zpos.X = 0;
	zpos.Y = 0;

	FillConsoleOutputCharacter(Console, ' ', buf.dwSize.X * buf.dwSize.Y, zpos, &Count);
	SetConsoleCursorPosition(Console, zpos);
}

void main()
{
	TRecord<std::string, int> record;
	
	TScanTable<std::string, int> scanTable(1000);
	TSortTable<std::string, int> sortTable(1000);
	TArrayHash<std::string, int> hashTable(1000);
	TTreeTable<std::string, int> treeTable;

	char buf[50];
	
	std::ifstream f("C:\\Users\\sirku\\Desktop\\UberTables\\Tables\\Tables\\Debug\\text.txt", std::ios_base::in);

	if (!f.is_open()) {
		std::cout << "incorrect file";
		system("PAUSE");
		exit(0);
	}

	record.value = 1;

	while (!f.eof())
	{
		f >> buf;
		record.key = buf;
		
		if (scanTable.Find(buf)) scanTable.SetCurr(scanTable.GetCurr().value + 1);
		if (sortTable.Find(buf)) sortTable.SetCurr(sortTable.GetCurr().value + 1);
		if (hashTable.Find(buf)) hashTable.SetCurr(hashTable.GetCurr().value + 1);
		/*if (treeTable.Find(buf)) treeTable.SetCurr(treeTable.GetCurr().value + 1);*/

		scanTable.Insert(record);
		sortTable.Insert(record);
		hashTable.Insert(record);
		treeTable.Insert(record);
	}

	scanTable.Print();
	/*sortTable.Print();
	hashTable.Print();
	treeTable.Print();*/

	std::cout << scanTable.GetEff() << std::endl;

	record.key = "Vadim";
	scanTable.Insert(record);

	clrscr();

	scanTable.Print();

	std::cout << scanTable.GetEff() << std::endl;

	system("PAUSE");
}