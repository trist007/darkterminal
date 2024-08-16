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
  std::vector<std::shared_ptr<Well>> well_ptrs;

  // building a vector of filenames
  for (auto& filename : wellListFileNames)
    filenames.push_back(&filename);

  // building a vector of pointer names
    /*
  for (auto& well_ptr : wellListPointerNames)
    well_ptr_names.push_back(&well_ptr);
    */


  for (auto& well : wellList)
    wellListMap[well] = std::make_shared<Well>(well + ".db");

  // initializing pointers
  //for (auto it1 = well_ptr_names.begin(), it2 = filenames.begin(); it1 != well_ptr_names.end(); ++it1, ++it2)
  //  well_ptrs.push_back(*it1 = std::make_shared<Well>(*it2));
  for(auto& filename : filenames)
    well_ptrs.push_back(std::make_shared<Well>(*filename));


  //well_ptr_13 = std::make_shared<Well>(filename13);

  //for (std::size_t i = 0; i < well_ptrs.size()p ++i)k
  //  std::cout << well_ptrs->wellname

  //return well_ptrs;
  return wellListMap;
}
