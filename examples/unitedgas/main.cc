#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <string>
#include <chrono>

#include <unitedgas/Well.h>
#include <trantor/utils/crypto/sha256.h>

using namespace drogon;
using namespace std::chrono_literals;


int main() {

    /*
  //std::vector<std::shared_ptr<Well>> well_ptrs;
  std::map<std::string, std::shared_ptr<Well>> wellListMap;

  std::string well_name = "12";

  //well_ptrs = initializeWellDB();
  wellListMap = initializeWellDB();

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
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/");
        callback(resp);
      } else {
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas");
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
        resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("UnitedGasView");
      callback(resp);
    },
    {Get});

  // original well
  app().registerHandler(
    "/unitedgas/.*([0-9]*")
    [well_name, wellListMap](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {

        /*
          if (wellListMap["12"] == nullptr)
            std::cout << "wellListMap[\"12\"] is NULL" << std::endl;

     auto it = wellListMap.find(well_name);
        /*
     if (it != wellListMap.end())
     {
     data["wellname"] = it->second->get_wellname();
     // rest of your code
     }
      HttpViewData data;

      data["wellname"] = it->second->get_wellname();
      data["wellno"] = it->second->get_wellno();
      data["dateOfRecentTest"] = it->second->get_dateOfRecentTest();
      data["dailyOil"] = it->second->get_dailyOil();
      data["dailyWater"] = it->second->get_dailyWater();
      data["dailyGas"] = it->second->get_dailyGas();
      data["opPressureTubing"] = it->second->get_opPressureTubing();
      data["opPressureCasing"] = it->second->get_opPressureCasing();
      data["pumpingUnitSize"] = it->second->get_pumpingUnitSize();
      data["strokesPerMin"] = it->second->get_strokesPerMin();
      data["strokeLength"] = it->second->get_strokeLength();
      data["motorHp"] = it->second->get_motorHp();
      data["pumpingRatio"] = it->second->get_pumpingRatio();
      data["unitGearRatio"] = it->second->get_unitGearRatio();
      data["casingSize"] = it->second->get_casingSize();
      data["tubingSize"] = it->second->get_tubingSize();
      data["pumpSize"] = it->second->get_pumpSize();
      data["firstCole"] = it->second->get_firstCole();
      data["secondCole"] = it->second->get_secondCole();
      data["thirdCole"] = it->second->get_thirdCole();
      data["comments"] = it->second->get_comments();

      HttpResponsePtr resp;

      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas");
      else
        resp = HttpResponse::newHttpViewResponse("Twelve", data);
      callback(resp);
    },
    {Get});

  app().registerHandler(
    "/formHandler/.*([0-9]*")
    [well_name, wellListMap](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {

      std::string data, encoded_data;

      auto it = wellListMap.find(well_name);
        /*
      if (wellListMap[well_name] == nullptr)
        std::cout << "wellListMap[7] is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        it->second->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        it->second->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        it->second->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        it->second->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        it->second->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        it->second->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        it->second->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        it->second->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        it->second->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        it->second->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        it->second->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        it->second->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        it->second->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        it->second->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        it->second->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        it->second->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        it->second->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        it->second->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        it->second->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        it->second->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        it->second->set_comments(encoded_data);
      }
      it->second->writetoFile();

      auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/" + well_name);
      callback(resp);
    },
    {Post});

*/
  // Ask Drogon to listen on 127.0.0.1 port 8848. Drogon supports listening
  // on multiple IP addresses by adding multiple listeners. For example, if
  // you want the server also listen on 127.0.0.1 port 5555. Just add another
  // line of addListener("127.0.0.1", 5555)
  LOG_INFO << "Server running on 127.0.0.1:8848";
  app()
      .enableSession(86400)
      .addListener("127.0.0.1", 8848)
      .run();
}
