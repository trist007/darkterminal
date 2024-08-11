//
// Created by Tristan on 8/11/2024.
//
#include <list>
#include <string>
#include <iostream>
#include <vector>

#include <../../lib/inc/unitedgas/Well.h>

#ifdef _MSC_VER
  const std::string filename2 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\2.db";
  const std::string filename3 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\3.db";
  const std::string filename6 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\6.db";
  const std::string filename7 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\7.db";
  const std::string filename8 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\8.db";
  const std::string filename9 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\9.db";
  const std::string filename10 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\10.db";
  const std::string filename12 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\12.db";
  const std::string filename13 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\13.db";
  const std::string filename15 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\15.db";
  const std::string filename20 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\20.db";
  const std::string filename21 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\21.db";
  const std::string filename22 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\22.db";
  const std::string filename23 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\23.db";
  const std::string filename24B = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\24B.db";
  const std::string filename25 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\25.db";
#else
  const std::string filename2 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/2.db";
  const std::string filename3 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/3.db";
  const std::string filename6 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/6.db";
  const std::string filename7 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/7.db";
  const std::string filename8 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/8.db";
  const std::string filename9 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/9.db";
  const std::string filename10 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/10.db";
  const std::string filename12 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/12.db";
  const std::string filename13 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/13.db";
  const std::string filename15 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/15.db";
  const std::string filename20 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/20.db";
  const std::string filename21 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/21.db";
  const std::string filename22 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/22.db";
  const std::string filename23 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/23.db";
  const std::string filename24B = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/24B.db";
  const std::string filename25 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/25.db";
#endif

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

std::list<std::string> unixbuildWellFileNames() {
  std::list<std::string> wellListFileNames;

  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/2.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/3.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/6.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/7.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/8.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/9.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/10.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/12.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/13.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/15.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/20.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/21.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/22.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/23.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/24B.db");
  wellListFileNames.push_back("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/wells/25.db");

  return wellListFileNames;
}

std::list<std::string> winbuildWellFileNames() {
  std::list<std::string> wellListFileNames;

  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\2.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\3.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\6.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\7.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\8.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\9.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\10.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\12.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\13.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\15.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\20.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\21.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\22.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\23.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\24B.db");
  wellListFileNames.push_back("C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\25.db");

  return wellListFileNames;
}

std::list<std::string> buildWellPointerNames() {
  std::list<std::string> wellListPointerNames;

  wellListPointerNames.push_back("well_ptr_2");
  wellListPointerNames.push_back("well_ptr_3");
  wellListPointerNames.push_back("well_ptr_6");
  wellListPointerNames.push_back("well_ptr_7");
  wellListPointerNames.push_back("well_ptr_8");
  wellListPointerNames.push_back("well_ptr_9");
  wellListPointerNames.push_back("well_ptr_10");
  wellListPointerNames.push_back("well_ptr_12");
  wellListPointerNames.push_back("well_ptr_13");
  wellListPointerNames.push_back("well_ptr_15");
  wellListPointerNames.push_back("well_ptr_20");
  wellListPointerNames.push_back("well_ptr_21");
  wellListPointerNames.push_back("well_ptr_22");
  wellListPointerNames.push_back("well_ptr_23");
  wellListPointerNames.push_back("well_ptr_24B");
  wellListPointerNames.push_back("well_ptr_25");

  return wellListPointerNames;
}

/*
std::shared_ptr<Well> well_ptr_2 = std::make_shared<Well>(filename2);
std::shared_ptr<Well> well_ptr_3 = std::make_shared<Well>(filename3);
std::shared_ptr<Well> well_ptr_6 = std::make_shared<Well>(filename6);
std::shared_ptr<Well> well_ptr_7 = std::make_shared<Well>(filename7);
std::shared_ptr<Well> well_ptr_8 = std::make_shared<Well>(filename8);
std::shared_ptr<Well> well_ptr_9 = std::make_shared<Well>(filename9);
std::shared_ptr<Well> well_ptr_10 = std::make_shared<Well>(filename10);
std::shared_ptr<Well> well_ptr_12 = std::make_shared<Well>(filename12);
std::shared_ptr<Well> well_ptr_13 = std::make_shared<Well>(filename13);
std::shared_ptr<Well> well_ptr_15 = std::make_shared<Well>(filename15);
std::shared_ptr<Well> well_ptr_20 = std::make_shared<Well>(filename20);
std::shared_ptr<Well> well_ptr_21 = std::make_shared<Well>(filename21);
std::shared_ptr<Well> well_ptr_22 = std::make_shared<Well>(filename22);
std::shared_ptr<Well> well_ptr_23 = std::make_shared<Well>(filename23);
std::shared_ptr<Well> well_ptr_24B = std::make_shared<Well>(filename24B);
std::shared_ptr<Well> well_ptr_25 = std::make_shared<Well>(filename25);

try {
  well_ptr_2 = std::make_shared<Well>(filename2);
  well_ptr_3 = std::make_shared<Well>(filename3);
  well_ptr_6 = std::make_shared<Well>(filename6);
  well_ptr_7 = std::make_shared<Well>(filename7);
  well_ptr_8 = std::make_shared<Well>(filename8);
  well_ptr_9 = std::make_shared<Well>(filename9);
  well_ptr_10 = std::make_shared<Well>(filename10);
  well_ptr_12 = std::make_shared<Well>(filename12);
  well_ptr_13 = std::make_shared<Well>(filename13);
  well_ptr_15 = std::make_shared<Well>(filename15);
  well_ptr_20 = std::make_shared<Well>(filename20);
  well_ptr_21 = std::make_shared<Well>(filename21);
  well_ptr_22 = std::make_shared<Well>(filename22);
  well_ptr_23 = std::make_shared<Well>(filename23);
  well_ptr_24B = std::make_shared<Well>(filename24B);
  well_ptr_25 = std::make_shared<Well>(filename25);
} catch (std::bad_alloc &ba) {
  std::cout << "unable to allocate memory" << std::endl;
}
*/

std::vector<std::shared_ptr<Well>> initializeWellDB()
{

  std::list<std::string> wellListPointerNames = buildWellPointerNames();
  std::list<std::string> wellListFileNames = winbuildWellFileNames();

  std::vector<std::string*> well_ptr_names;
  std::vector<std::string*> filenames;
  std::vector<std::shared_ptr<Well>> well_ptrs;

  // building a vector of filenames
  for (auto& filename : wellListFileNames)
    filenames.push_back(&filename);

  // building a vector of pointer names
  for (auto& well_ptr : wellListPointerNames)
    well_ptr_names.push_back(&well_ptr);

  // initializing pointers
  //for (auto it1 = well_ptr_names.begin(), it2 = filenames.begin(); it1 != well_ptr_names.end(); ++it1, ++it2)
  //  well_ptrs.push_back(*it1 = std::make_shared<Well>(*it2));
  for(auto& filename : filenames)
    well_ptrs.push_back(std::make_shared<Well>(*filename));

   
  //well_ptr_13 = std::make_shared<Well>(filename13);

  //for (std::size_t i = 0; i < well_ptrs.size()p ++i)k
  //  std::cout << well_ptrs->wellname

  return well_ptrs;
}