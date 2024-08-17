//
// Created by Tristan on 8/11/2024.
//
#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <unitedgas/Well.h>

std::list<std::string> buildWellList() {
  std::list<std::string> wellList;

  wellList.push_back("2");
  wellList.push_back("3");
  wellList.push_back("6");
  wellList.push_back("7");
  wellList.push_back("8");
  wellList.push_back("9");
  wellList.push_back("10");
  wellList.push_back("12");
  wellList.push_back("13");
  wellList.push_back("15");
  wellList.push_back("20");
  wellList.push_back("21");
  wellList.push_back("22");
  wellList.push_back("23");
  wellList.push_back("24B");
  wellList.push_back("25");

  return wellList;
}

std::list<std::string> buildWellFileNames() {
  std::list<std::string> wellListFileNames;

  wellListFileNames.push_back("2.db");
  wellListFileNames.push_back("3.db");
  wellListFileNames.push_back("6.db");
  wellListFileNames.push_back("7.db");
  wellListFileNames.push_back("8.db");
  wellListFileNames.push_back("9.db");
  wellListFileNames.push_back("10.db");
  wellListFileNames.push_back("12.db");
  wellListFileNames.push_back("13.db");
  wellListFileNames.push_back("15.db");
  wellListFileNames.push_back("20.db");
  wellListFileNames.push_back("21.db");
  wellListFileNames.push_back("22.db");
  wellListFileNames.push_back("23.db");
  wellListFileNames.push_back("24B.db");
  wellListFileNames.push_back("25.db");

  return wellListFileNames;
}


//std::vector<std::shared_ptr<Well>> initializeWellDB()
std::map<std::string, std::shared_ptr<Well>> initializeWellDB()
{

  std::list<std::string> wellList = buildWellList();
  std::list<std::string> wellListFileNames = buildWellFileNames();

  std::map<std::string, std::shared_ptr<Well>> wellListMap;

  std::vector<std::string*> filenames;

  // building a vector of filenames
  for (auto& filename : wellListFileNames)
    filenames.push_back(&filename);

  for (auto& well : wellList)
    wellListMap[well] = std::make_shared<Well>(well + ".db");

  return wellListMap;
}
