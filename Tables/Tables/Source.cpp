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

	std::string command = "";
	char buf[50];
	
	std::ifstream f("C:\\Users\\sirku\\Desktop\\UberTables\\Tables\\Tables\\Debug\\text.txt", std::ios_base::in);

	if (!f.is_open()) {
		std::cout << "Incorrect file";
		system("PAUSE");
		exit(0);
	}
	else std::cout << "Loaded file" << std::endl;

	record.value = 1;

	while (!f.eof())
	{
		f >> buf;
		record.key = buf;
		
		if (scanTable.Find(buf)) scanTable.SetCurr(scanTable.GetCurr().value + 1);
		if (sortTable.Find(buf)) sortTable.SetCurr(sortTable.GetCurr().value + 1);
		if (hashTable.Find(buf)) hashTable.SetCurr(hashTable.GetCurr().value + 1);
		if (treeTable.Find(buf)) treeTable.SetRes(treeTable.GetRes().value + 1);

		scanTable.Insert(record);
		sortTable.Insert(record);
		hashTable.Insert(record);
		treeTable.Insert(record);
	}

	std::cout << "Initialised tables" << std::endl;

	while (true) {
		std::cin >> command;

		if (command == "PRINT_treeTable") treeTable.Print();
		else if (command == "PRINT_sortTable") sortTable.Print();
		else if (command == "PRINT_hashTable") hashTable.Print();
		else if (command == "PRINT_scanTable") scanTable.Print();

		else if (command == "CLEAR") clrscr();
		else if (command == "EXIT") exit(0);

		else if (command == "INSERT_IN_treeTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;
			std::cout << "value = ";
			std::cin >> record.value;
			std::cout << std::endl;

			treeTable.Insert(record);
		}

		else if (command == "INSERT_IN_sortTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;
			std::cout << "value = ";
			std::cin >> record.value;
			std::cout << std::endl;

			sortTable.Insert(record);
		}

		else if (command == "INSERT_IN_hashTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;
			std::cout << "value = ";
			std::cin >> record.value;
			std::cout << std::endl;

			hashTable.Insert(record);
		}

		else if (command == "INSERT_IN_scanTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;
			std::cout << "value = ";
			std::cin >> record.value;
			std::cout << std::endl;

			scanTable.Insert(record);
		}

		else if (command == "DELETE_FROM_treeTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;

			treeTable.Delete(record.key);
		}

		else if (command == "DELETE_FROM_sortTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;

			sortTable.Delete(record.key);
		}

		else if (command == "DELETE_FROM_hashTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;

			hashTable.Delete(record.key);
		}

		else if (command == "DELETE_FROM_scanTable") {
			std::cout << "key = ";
			std::cin >> record.key;
			std::cout << std::endl;

			scanTable.Delete(record.key);
		}
		else if (command == "HELP") {
			std::cout << "PRINT_tableName" << std::endl;
			std::cout << "INSERT_IN_tableName" << std::endl;
			std::cout << "DELETE_FROM_tableName" << std::endl;
			std::cout << "CLEAR" << std::endl;
			std::cout << "EXIT" << std::endl;
		}
		else if (command == "SHOW_EFF") {
			std::cout << "scan eff = " << scanTable.GetEff() << std::endl;
			std::cout << "sort eff = " << sortTable.GetEff() << std::endl;
			std::cout << "tree eff = " << treeTable.GetEff() << std::endl;
			std::cout << "hash eff = " << hashTable.GetEff() << std::endl;
		}
		else std::cout << "INCORRECT COMMAND" << std::endl;
	}

	system("PAUSE");
}