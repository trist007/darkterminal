#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <string>
#include <chrono>

#include <unitedgas/Well.h>
#include <trantor/utils/crypto/sha256.h>

using namespace drogon;
using namespace std::chrono_literals;

#ifdef _MSC_VER
  const std::string filename12 = "C:\\Users\\Tristan\\CLionProjects\\darkterminal\\examples\\unitedgas\\wells\\12.db";
#else
  const std::string filename12 = "/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/12.db";
#endif

std::string encode(std::string &data) {
  std::string buffer;
  buffer.reserve(data.size());
  for (size_t pos = 0; pos != data.size(); ++pos) {
    switch (data[pos]) {
      case '&':
        buffer.append("&amp;");
        break;
      case '\"':
        buffer.append("&quot;");
        break;
      case '\'':
        buffer.append("&apos;");
        break;
      case '<':
        buffer.append("&lt;");
        break;
      case '>':
        buffer.append("&gt;");
        break;
      default:
        buffer.append(&data[pos], 1);
        break;
    }
  }
  return buffer;
}

int main() {

  std::vector<std::shared_ptr<Well>> well_ptrs;
  
  well_ptrs = initializeWellDB();

  // `registerHandler()` adds a handler to the desired path. The handler is
  // responsible for generating a HTTP response upon an HTTP request being
  // sent to Drogon
  //
  /*
  std::shared_ptr<Well> well_ptr_12;
  try {
    well_ptr_12 = std::make_shared<Well>(filename12);
  } catch (std::bad_alloc &ba) {
    std::cout << "unable to allocate memory" << std::endl;
  }
  */
  
  app().registerHandler(
    "/unitedgas",
    [](const HttpRequestPtr &req,
       std::function<void(const HttpResponsePtr &)> &&callback) {
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      HttpResponsePtr resp;
      if (loggedIn == false)
        resp = HttpResponse::newHttpViewResponse("LoginPage");
      else
        resp = HttpResponse::newHttpViewResponse("LogoutPage");
      callback(resp);
    });

  app().registerHandler(
    "/logout",
    [](const HttpRequestPtr &req,
       std::function<void(const HttpResponsePtr &)> &&callback) {
      HttpResponsePtr resp = HttpResponse::newHttpResponse();
      req->session()->erase("loggedIn");
      resp->setBody("Logged out");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/login",
    [](const HttpRequestPtr &req,
       std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string user = req->getParameter("user");
      std::string passwd = req->getParameter("passwd");
      std::string hash;

      // NOTE: Do not use MD5 for the password hash under any
      // circumstances. We only use it because Drogon is not a
      // cryptography library, so it does not include a better hash
      // algorithm. Use Argon2 or BCrypt in a real product.
      //
      hash = utils::getSha256(passwd);
      if (hash == "1C1F1024D78CBFA8DF28545FA462D9FA461D0D6874A2B5E2FCC7214D6C78B9BA") {
        req->session()->insert("loggedIn", true);
        auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/");
        callback(resp);
      } else {
        auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
        callback(resp);
      }
    },
    {Post});

    app().registerHandler(
    "/unitedgas/",
    [](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      HttpResponsePtr resp;
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("UnitedGasView");
      callback(resp);
    },
    {Get});
  
  // original well
  app().registerHandler(
    "/unitedgas/12",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[7] == nullptr)
        std::cout << "well_ptr[7] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[7]->get_wellname();
      data["wellno"] = well_ptrs[7]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[7]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[7]->get_dailyOil();
      data["dailyWater"] = well_ptrs[7]->get_dailyWater();
      data["dailyGas"] = well_ptrs[7]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[7]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[7]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[7]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[7]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[7]->get_strokeLength();
      data["motorHp"] = well_ptrs[7]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[7]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[7]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[7]->get_casingSize();
      data["tubingSize"] = well_ptrs[7]->get_tubingSize();
      data["pumpSize"] = well_ptrs[7]->get_pumpSize();
      data["firstCole"] = well_ptrs[7]->get_firstCole();
      data["secondCole"] = well_ptrs[7]->get_secondCole();
      data["thirdCole"] = well_ptrs[7]->get_thirdCole();
      data["comments"] = well_ptrs[7]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Twelve", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/12",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[7] == nullptr)
        std::cout << "well_ptrs[7] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[7]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[7]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[7]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[7]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[7]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[7]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[7]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[7]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[7]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[7]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[7]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[7]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[7]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[7]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[7]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[7]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[7]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[7]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[7]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[7]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[7]->set_comments(encoded_data);
      }
      well_ptrs[7]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/12");
      callback(resp);
    },
    {Post});

  // start of transmutation
  app().registerHandler(
    "/unitedgas/2",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[0] == nullptr)
        std::cout << "well_ptr_2 is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[0]->get_wellname();
      data["wellno"] = well_ptrs[0]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[0]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[0]->get_dailyOil();
      data["dailyWater"] = well_ptrs[0]->get_dailyWater();
      data["dailyGas"] = well_ptrs[0]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[0]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[0]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[0]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[0]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[0]->get_strokeLength();
      data["motorHp"] = well_ptrs[0]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[0]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[0]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[0]->get_casingSize();
      data["tubingSize"] = well_ptrs[0]->get_tubingSize();
      data["pumpSize"] = well_ptrs[0]->get_pumpSize();
      data["firstCole"] = well_ptrs[0]->get_firstCole();
      data["secondCole"] = well_ptrs[0]->get_secondCole();
      data["thirdCole"] = well_ptrs[0]->get_thirdCole();
      data["comments"] = well_ptrs[0]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Two", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/2",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[0] == nullptr)
        std::cout << "well_ptrs[0] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[0]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[0]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[0]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[0]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[0]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[0]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[0]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[0]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[0]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[0]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[0]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[0]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[0]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[0]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[0]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[0]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[0]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[0]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[0]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[0]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[0]->set_comments(encoded_data);
      }
      well_ptrs[0]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/2");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/3",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[1] == nullptr)
        std::cout << "well_ptr[1] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[1]->get_wellname();
      data["wellno"] = well_ptrs[1]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[1]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[1]->get_dailyOil();
      data["dailyWater"] = well_ptrs[1]->get_dailyWater();
      data["dailyGas"] = well_ptrs[1]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[1]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[1]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[1]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[1]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[1]->get_strokeLength();
      data["motorHp"] = well_ptrs[1]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[1]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[1]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[1]->get_casingSize();
      data["tubingSize"] = well_ptrs[1]->get_tubingSize();
      data["pumpSize"] = well_ptrs[1]->get_pumpSize();
      data["firstCole"] = well_ptrs[1]->get_firstCole();
      data["secondCole"] = well_ptrs[1]->get_secondCole();
      data["thirdCole"] = well_ptrs[1]->get_thirdCole();
      data["comments"] = well_ptrs[1]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Three", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/3",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[1] == nullptr)
        std::cout << "well_ptrs[1] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[1]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[1]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[1]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[1]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[1]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[1]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[1]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[1]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[1]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[1]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[1]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[1]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[1]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[1]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[1]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[1]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[1]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[1]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[1]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[1]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[1]->set_comments(encoded_data);
      }
      well_ptrs[1]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/3");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/6",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[2] == nullptr)
        std::cout << "well_ptrs[2] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[2]->get_wellname();
      data["wellno"] = well_ptrs[2]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[2]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[2]->get_dailyOil();
      data["dailyWater"] = well_ptrs[2]->get_dailyWater();
      data["dailyGas"] = well_ptrs[2]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[2]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[2]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[2]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[2]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[2]->get_strokeLength();
      data["motorHp"] = well_ptrs[2]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[2]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[2]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[2]->get_casingSize();
      data["tubingSize"] = well_ptrs[2]->get_tubingSize();
      data["pumpSize"] = well_ptrs[2]->get_pumpSize();
      data["firstCole"] = well_ptrs[2]->get_firstCole();
      data["secondCole"] = well_ptrs[2]->get_secondCole();
      data["thirdCole"] = well_ptrs[2]->get_thirdCole();
      data["comments"] = well_ptrs[2]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Six", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/6",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[2] == nullptr)
        std::cout << "well_ptrs[2] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[2]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[2]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[2]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[2]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[2]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[2]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[2]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[2]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[2]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[2]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[2]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[2]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[2]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[2]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[2]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[2]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[2]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[2]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[2]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[2]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[2]->set_comments(encoded_data);
      }
      well_ptrs[2]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/6");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/7",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[3] == nullptr)
        std::cout << "well_ptrs[3] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[3]->get_wellname();
      data["wellno"] = well_ptrs[3]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[3]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[3]->get_dailyOil();
      data["dailyWater"] = well_ptrs[3]->get_dailyWater();
      data["dailyGas"] = well_ptrs[3]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[3]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[3]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[3]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[3]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[3]->get_strokeLength();
      data["motorHp"] = well_ptrs[3]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[3]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[3]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[3]->get_casingSize();
      data["tubingSize"] = well_ptrs[3]->get_tubingSize();
      data["pumpSize"] = well_ptrs[3]->get_pumpSize();
      data["firstCole"] = well_ptrs[3]->get_firstCole();
      data["secondCole"] = well_ptrs[3]->get_secondCole();
      data["thirdCole"] = well_ptrs[3]->get_thirdCole();
      data["comments"] = well_ptrs[3]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Seven", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/7",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[3] == nullptr)
        std::cout << "well_ptrs[3] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[3]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[3]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[3]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[3]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[3]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[3]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[3]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[3]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[3]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[3]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[3]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[3]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[3]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[3]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[3]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[3]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[3]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[3]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[3]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[3]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[3]->set_comments(encoded_data);
      }
      well_ptrs[3]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/7");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/8",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[4] == nullptr)
        std::cout << "well_ptrs[4] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[4]->get_wellname();
      data["wellno"] = well_ptrs[4]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[4]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[4]->get_dailyOil();
      data["dailyWater"] = well_ptrs[4]->get_dailyWater();
      data["dailyGas"] = well_ptrs[4]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[4]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[4]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[4]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[4]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[4]->get_strokeLength();
      data["motorHp"] = well_ptrs[4]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[4]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[4]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[4]->get_casingSize();
      data["tubingSize"] = well_ptrs[4]->get_tubingSize();
      data["pumpSize"] = well_ptrs[4]->get_pumpSize();
      data["firstCole"] = well_ptrs[4]->get_firstCole();
      data["secondCole"] = well_ptrs[4]->get_secondCole();
      data["thirdCole"] = well_ptrs[4]->get_thirdCole();
      data["comments"] = well_ptrs[4]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Eight", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/8",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[4] == nullptr)
        std::cout << "well_ptrs[4] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[4]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[4]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[4]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[4]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[4]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[4]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[4]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[4]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[4]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[4]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[4]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[4]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[4]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[4]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[4]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[4]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[4]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[4]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[4]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[4]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[4]->set_comments(encoded_data);
      }
      well_ptrs[4]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/8");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/9",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[5] == nullptr)
        std::cout << "well_ptrs[5] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[5]->get_wellname();
      data["wellno"] = well_ptrs[5]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[5]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[5]->get_dailyOil();
      data["dailyWater"] = well_ptrs[5]->get_dailyWater();
      data["dailyGas"] = well_ptrs[5]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[5]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[5]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[5]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[5]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[5]->get_strokeLength();
      data["motorHp"] = well_ptrs[5]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[5]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[5]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[5]->get_casingSize();
      data["tubingSize"] = well_ptrs[5]->get_tubingSize();
      data["pumpSize"] = well_ptrs[5]->get_pumpSize();
      data["firstCole"] = well_ptrs[5]->get_firstCole();
      data["secondCole"] = well_ptrs[5]->get_secondCole();
      data["thirdCole"] = well_ptrs[5]->get_thirdCole();
      data["comments"] = well_ptrs[5]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Nine", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/9",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[5] == nullptr)
        std::cout << "well_ptrs[5] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[5]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[5]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[5]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[5]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[5]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[5]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[5]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[5]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[5]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[5]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[5]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[5]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[5]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[5]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[5]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[5]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[5]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[5]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[5]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[5]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[5]->set_comments(encoded_data);
      }
      well_ptrs[5]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/9");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/10",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[6] == nullptr)
        std::cout << "well_ptrs[6] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[6]->get_wellname();
      data["wellno"] = well_ptrs[6]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[6]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[6]->get_dailyOil();
      data["dailyWater"] = well_ptrs[6]->get_dailyWater();
      data["dailyGas"] = well_ptrs[6]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[6]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[6]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[6]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[6]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[6]->get_strokeLength();
      data["motorHp"] = well_ptrs[6]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[6]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[6]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[6]->get_casingSize();
      data["tubingSize"] = well_ptrs[6]->get_tubingSize();
      data["pumpSize"] = well_ptrs[6]->get_pumpSize();
      data["firstCole"] = well_ptrs[6]->get_firstCole();
      data["secondCole"] = well_ptrs[6]->get_secondCole();
      data["thirdCole"] = well_ptrs[6]->get_thirdCole();
      data["comments"] = well_ptrs[6]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Ten", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/10",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[6] == nullptr)
        std::cout << "well_ptrs[6] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[6]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[6]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[6]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[6]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[6]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[6]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[6]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[6]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[6]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[6]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[6]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[6]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[6]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[6]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[6]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[6]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[6]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[6]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[6]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[6]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[6]->set_comments(encoded_data);
      }
      well_ptrs[6]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/10");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/13",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[8] == nullptr)
        std::cout << "well_ptrs[8] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[8]->get_wellname();
      data["wellno"] = well_ptrs[8]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[8]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[8]->get_dailyOil();
      data["dailyWater"] = well_ptrs[8]->get_dailyWater();
      data["dailyGas"] = well_ptrs[8]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[8]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[8]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[8]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[8]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[8]->get_strokeLength();
      data["motorHp"] = well_ptrs[8]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[8]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[8]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[8]->get_casingSize();
      data["tubingSize"] = well_ptrs[8]->get_tubingSize();
      data["pumpSize"] = well_ptrs[8]->get_pumpSize();
      data["firstCole"] = well_ptrs[8]->get_firstCole();
      data["secondCole"] = well_ptrs[8]->get_secondCole();
      data["thirdCole"] = well_ptrs[8]->get_thirdCole();
      data["comments"] = well_ptrs[8]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Thirteen", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/13",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[8] == nullptr)
        std::cout << "well_ptrs[8] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[8]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[8]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[8]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[8]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[8]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[8]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[8]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[8]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[8]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[8]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[8]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[8]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[8]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[8]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[8]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[8]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[8]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[8]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[8]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[8]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[8]->set_comments(encoded_data);
      }
      well_ptrs[8]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/13");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/15",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[9] == nullptr)
        std::cout << "well_ptrs[9] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[9]->get_wellname();
      data["wellno"] = well_ptrs[9]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[9]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[9]->get_dailyOil();
      data["dailyWater"] = well_ptrs[9]->get_dailyWater();
      data["dailyGas"] = well_ptrs[9]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[9]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[9]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[9]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[9]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[9]->get_strokeLength();
      data["motorHp"] = well_ptrs[9]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[9]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[9]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[9]->get_casingSize();
      data["tubingSize"] = well_ptrs[9]->get_tubingSize();
      data["pumpSize"] = well_ptrs[9]->get_pumpSize();
      data["firstCole"] = well_ptrs[9]->get_firstCole();
      data["secondCole"] = well_ptrs[9]->get_secondCole();
      data["thirdCole"] = well_ptrs[9]->get_thirdCole();
      data["comments"] = well_ptrs[9]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Fifteen", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/15",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[9] == nullptr)
        std::cout << "well_ptrs[9] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[9]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[9]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[9]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[9]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[9]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[9]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[9]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[9]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[9]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[9]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[9]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[9]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[9]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[9]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[9]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[9]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[9]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[9]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[9]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[9]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[9]->set_comments(encoded_data);
      }
      well_ptrs[9]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/15");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/20",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[10] == nullptr)
        std::cout << "well_ptrs[10] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[10]->get_wellname();
      data["wellno"] = well_ptrs[10]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[10]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[10]->get_dailyOil();
      data["dailyWater"] = well_ptrs[10]->get_dailyWater();
      data["dailyGas"] = well_ptrs[10]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[10]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[10]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[10]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[10]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[10]->get_strokeLength();
      data["motorHp"] = well_ptrs[10]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[10]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[10]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[10]->get_casingSize();
      data["tubingSize"] = well_ptrs[10]->get_tubingSize();
      data["pumpSize"] = well_ptrs[10]->get_pumpSize();
      data["firstCole"] = well_ptrs[10]->get_firstCole();
      data["secondCole"] = well_ptrs[10]->get_secondCole();
      data["thirdCole"] = well_ptrs[10]->get_thirdCole();
      data["comments"] = well_ptrs[10]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Twenty", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/20",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[10] == nullptr)
        std::cout << "well_ptrs[10] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[10]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[10]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[10]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[10]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[10]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[10]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[10]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[10]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[10]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[10]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[10]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[10]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[10]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[10]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[10]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[10]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[10]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[10]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[10]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[10]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[10]->set_comments(encoded_data);
      }
      well_ptrs[10]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/20");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/21",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[11] == nullptr)
        std::cout << "well_ptrs[11] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[11]->get_wellname();
      data["wellno"] = well_ptrs[11]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[11]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[11]->get_dailyOil();
      data["dailyWater"] = well_ptrs[11]->get_dailyWater();
      data["dailyGas"] = well_ptrs[11]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[11]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[11]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[11]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[11]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[11]->get_strokeLength();
      data["motorHp"] = well_ptrs[11]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[11]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[11]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[11]->get_casingSize();
      data["tubingSize"] = well_ptrs[11]->get_tubingSize();
      data["pumpSize"] = well_ptrs[11]->get_pumpSize();
      data["firstCole"] = well_ptrs[11]->get_firstCole();
      data["secondCole"] = well_ptrs[11]->get_secondCole();
      data["thirdCole"] = well_ptrs[11]->get_thirdCole();
      data["comments"] = well_ptrs[11]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("TwentyOne", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/21",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[11] == nullptr)
        std::cout << "well_ptrs[11] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[11]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[11]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[11]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[11]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[11]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[11]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[11]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[11]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[11]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[11]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[11]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[11]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[11]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[11]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[11]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[11]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[11]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[11]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[11]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[11]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[11]->set_comments(encoded_data);
      }
      well_ptrs[11]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/21");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/22",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[12] == nullptr)
        std::cout << "well_ptrs[12] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[12]->get_wellname();
      data["wellno"] = well_ptrs[12]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[12]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[12]->get_dailyOil();
      data["dailyWater"] = well_ptrs[12]->get_dailyWater();
      data["dailyGas"] = well_ptrs[12]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[12]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[12]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[12]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[12]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[12]->get_strokeLength();
      data["motorHp"] = well_ptrs[12]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[12]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[12]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[12]->get_casingSize();
      data["tubingSize"] = well_ptrs[12]->get_tubingSize();
      data["pumpSize"] = well_ptrs[12]->get_pumpSize();
      data["firstCole"] = well_ptrs[12]->get_firstCole();
      data["secondCole"] = well_ptrs[12]->get_secondCole();
      data["thirdCole"] = well_ptrs[12]->get_thirdCole();
      data["comments"] = well_ptrs[12]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("TwentyTwo", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/22",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[12] == nullptr)
        std::cout << "well_ptrs[12] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[12]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[12]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[12]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[12]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[12]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[12]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[12]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[12]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[12]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[12]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[12]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[12]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[12]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[12]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[12]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[12]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[12]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[12]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[12]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[12]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[12]->set_comments(encoded_data);
      }
      well_ptrs[12]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/22");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/23",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[13] == nullptr)
        std::cout << "well_ptrs[13] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[13]->get_wellname();
      data["wellno"] = well_ptrs[13]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[13]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[13]->get_dailyOil();
      data["dailyWater"] = well_ptrs[13]->get_dailyWater();
      data["dailyGas"] = well_ptrs[13]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[13]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[13]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[13]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[13]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[13]->get_strokeLength();
      data["motorHp"] = well_ptrs[13]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[13]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[13]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[13]->get_casingSize();
      data["tubingSize"] = well_ptrs[13]->get_tubingSize();
      data["pumpSize"] = well_ptrs[13]->get_pumpSize();
      data["firstCole"] = well_ptrs[13]->get_firstCole();
      data["secondCole"] = well_ptrs[13]->get_secondCole();
      data["thirdCole"] = well_ptrs[13]->get_thirdCole();
      data["comments"] = well_ptrs[13]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("TwentyThree", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/23",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[13] == nullptr)
        std::cout << "well_ptrs[13] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[13]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[13]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[13]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[13]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[13]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[13]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[13]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[13]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[13]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[13]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[13]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[13]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[13]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[13]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[13]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[13]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[13]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[13]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[13]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[13]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[13]->set_comments(encoded_data);
      }
      well_ptrs[13]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/23");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/24B",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[14] == nullptr)
        std::cout << "well_ptrs[14] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[14]->get_wellname();
      data["wellno"] = well_ptrs[14]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[14]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[14]->get_dailyOil();
      data["dailyWater"] = well_ptrs[14]->get_dailyWater();
      data["dailyGas"] = well_ptrs[14]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[14]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[14]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[14]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[14]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[14]->get_strokeLength();
      data["motorHp"] = well_ptrs[14]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[14]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[14]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[14]->get_casingSize();
      data["tubingSize"] = well_ptrs[14]->get_tubingSize();
      data["pumpSize"] = well_ptrs[14]->get_pumpSize();
      data["firstCole"] = well_ptrs[14]->get_firstCole();
      data["secondCole"] = well_ptrs[14]->get_secondCole();
      data["thirdCole"] = well_ptrs[14]->get_thirdCole();
      data["comments"] = well_ptrs[14]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("TwentyFourB", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/24B",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[14] == nullptr)
        std::cout << "well_ptrs[14] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[14]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[14]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[14]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[14]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[14]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[14]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[14]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[14]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[14]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[14]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[14]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[14]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[14]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[14]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[14]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[14]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[14]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[14]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[14]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[14]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[14]->set_comments(encoded_data);
      }
      well_ptrs[14]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/24B");
      callback(resp);
    },
    {Post});

  app().registerHandler(
    "/unitedgas/25",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptrs[15] == nullptr)
        std::cout << "well_ptrs[15] is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptrs[15]->get_wellname();
      data["wellno"] = well_ptrs[15]->get_wellno();
      data["dateOfRecentTest"] = well_ptrs[15]->get_dateOfRecentTest();
      data["dailyOil"] = well_ptrs[15]->get_dailyOil();
      data["dailyWater"] = well_ptrs[15]->get_dailyWater();
      data["dailyGas"] = well_ptrs[15]->get_dailyGas();
      data["opPressureTubing"] = well_ptrs[15]->get_opPressureTubing();
      data["opPressureCasing"] = well_ptrs[15]->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptrs[15]->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptrs[15]->get_strokesPerMin();
      data["strokeLength"] = well_ptrs[15]->get_strokeLength();
      data["motorHp"] = well_ptrs[15]->get_motorHp();
      data["pumpingRatio"] = well_ptrs[15]->get_pumpingRatio();
      data["unitGearRatio"] = well_ptrs[15]->get_unitGearRatio();
      data["casingSize"] = well_ptrs[15]->get_casingSize();
      data["tubingSize"] = well_ptrs[15]->get_tubingSize();
      data["pumpSize"] = well_ptrs[15]->get_pumpSize();
      data["firstCole"] = well_ptrs[15]->get_firstCole();
      data["secondCole"] = well_ptrs[15]->get_secondCole();
      data["thirdCole"] = well_ptrs[15]->get_thirdCole();
      data["comments"] = well_ptrs[15]->get_comments();

      HttpResponsePtr resp;
      
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("TwentyFive", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/25",
    [well_ptrs](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptrs[15] == nullptr)
        std::cout << "well_ptrs[15] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptrs[15]->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptrs[15]->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptrs[15]->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptrs[15]->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptrs[15]->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptrs[15]->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptrs[15]->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptrs[15]->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptrs[15]->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptrs[15]->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptrs[15]->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptrs[15]->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptrs[15]->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptrs[15]->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptrs[15]->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptrs[15]->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptrs[15]->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptrs[15]->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptrs[15]->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptrs[15]->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptrs[15]->set_comments(encoded_data);
      }
      well_ptrs[15]->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://darkterminal.net/unitedgas/25");
      callback(resp);
    },
    {Post});

  // Ask Drogon to listen on 127.0.0.1 port 8848. Drogon supports listening
  // on multiple IP addresses by adding multiple listeners. For example, if
  // you want the server also listen on 127.0.0.1 port 5555. Just add another
  // line of addListener("127.0.0.1", 5555)
  LOG_INFO << "Server running on 0.0.0.0:80";
  app()
      .enableSession(86400)
      .addListener("0.0.0.0", 80)
      .run();
}
