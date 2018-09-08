// PlayerStructMaker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <vector>
#include <algorithm>

std::string ReadFile(const char* filepath) {
	FILE* file;
	fopen_s(&file,filepath, "rt");
	fseek(file, 0, SEEK_END);
	unsigned long lenght = ftell(file);
	char* data = new char[lenght + 1];
	memset(data, 0, lenght + 1);
	fseek(file, 0, SEEK_SET);
	fread(data, 1, lenght, file);
	fclose(file);

	std::string result(data);
	delete[] data;
	return result;
}

std::vector<std::string> offset;

void setOffsets(std::string input) {
	do {
		std::string firstoffset = input.substr(0, input.find('\n'));
		input = input.substr(input.find('\n') + 1, input.size());
		const char *firstOffset = firstoffset.c_str();
		offset.push_back(firstoffset.c_str());
	} while (input.find('\n') != std::string::npos);
}

class offsetClass {
public:
	const char* name;
	int add;
	offsetClass() {};
	offsetClass(const char* name, int address) {
		this->name = name;
		this->add = address;
	};
};

std::vector<offsetClass> OffClass;

void WriteFile(const char* filepath) {
	FILE* file;
	fopen_s(&file, filepath, "w");
	int savedInt = 0;

	int savedINT = 0;
	const char* savedNAME = "";
	int savedADD = 0;
	std::string NextFrameName;

	fprintf(file, "struct playerClass {");

	for (int i = 0; i < OffClass.size(); i++) {
		std::string savedNextFrame;
		int IntPrint = OffClass[i].add - savedInt;

		if (savedINT != 0) {
			fprintf(file, "char _0x%0X[%i]; \n", OffClass[i].add, savedINT);
		}
		if (strncmp(OffClass[i].name, "m_i", 3) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "int";
		}
		if (strncmp(OffClass[i].name, "m_vec", 5) == 0) {
			savedInt = OffClass[i].add + 12;
			savedNextFrame = "vec3";
		}
		if (strncmp(OffClass[i].name, "m_f", 3) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "float";
		}
		if (strncmp(OffClass[i].name, "m_b", 3) == 0) {
			savedInt = OffClass[i].add + 1;
			savedNextFrame = "unsigned char";
		}
		if (strncmp(OffClass[i].name, "m_dw", 4) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "unsigned long";
		}
		if (strncmp(OffClass[i].name, "m_MoveType", 10) == 0) {
			savedInt = OffClass[i].add + 1;
			savedNextFrame = "unsigned char";
		}
		if (strncmp(OffClass[i].name, "m_h", 3) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "int";
		}
		if (strncmp(OffClass[i].name, "m_lifeState", 11) == 0) {
			savedInt = OffClass[i].add + 1;
			savedNextFrame = "unsigned char";
		}
		if (strncmp(OffClass[i].name, "m_iItem", 7) == 0) {
			savedInt = OffClass[i].add + 1;
			savedNextFrame = "unsigned char";
		}
		if (savedNextFrame.size() == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "unsigned long";
		}
		if (strncmp(OffClass[i].name, "m_aimPunchAngle", 15) == 0) {
			savedInt = OffClass[i].add + 12;
			savedNextFrame = "vec3";
		}
		if (strncmp(OffClass[i].name, "m_aimPunchAngleVel", 18) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "int";
		}
		if (strncmp(OffClass[i].name, "m_fFlags", 8) == 0) {
			savedInt = OffClass[i].add + 4;
			savedNextFrame = "int";
		}

		fprintf(file, NextFrameName.c_str());
		fprintf(file, " %s;\n", savedNAME);

		NextFrameName = savedNextFrame;
		savedINT = IntPrint;
		savedNAME = OffClass[i].name;
		savedADD = OffClass[i].add;

	}
	fprintf(file, "}");
	fclose(file);
}

float offsetSorter(offsetClass a, offsetClass b) {
	return a.add < b.add;
}

int main()
{
	// get offsets from https://github.com/frk1/hazedumper/blob/master/csgo.cs
	std::string str = "Offsets.txt";
	std::string input = ReadFile(str.c_str());
	setOffsets(input);

	for (int i = 0; i < offset.size(); i++) {
		int offsetfind11 = offset[i].find("Int32");
		offsetfind11 += 6;
		offset[i].erase(offset[i].begin(), offset[i].begin() + offsetfind11);

		int offsetfind = offset[i].find('=');
		int offsetEndfind = offset[i].find(';');

		std::string offsetArea = offset[i].substr(offsetfind + 4, offset[i].size());
		offsetArea.erase(offsetArea.size() - 1, offsetArea.size());
		int offsetnumb = std::strtoul(offsetArea.c_str(), NULL, 16);

		int end = offset[i].size();
		offset[i].erase(offsetfind - 1, end);
		OffClass.emplace_back(offset[i].c_str(), offsetnumb);
	}

	std::sort(OffClass.begin(), OffClass.end(), offsetSorter);

	WriteFile("playerClass.txt");

	return 0;
}
