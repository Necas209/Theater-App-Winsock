#include "Theater.h"

Theater::Theater()
{
}

Theater::~Theater()
{
	for (auto& show : shows)
	{
		delete show;
	}
	shows.clear();
}

void Theater::Write()
{
	std::cout << name << "; " << location << "; ";
	for (const auto& show : shows)
	{
		show->Write();
	}
}

void Theater::WriteFile(std::ofstream& ofs)
{
	for (const auto& show : shows)
	{
		ofs << '"' << name << "\",";
		ofs << '"' << location << "\",";
		show->WriteFile(ofs);
		ofs << '\n';
	}
}