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
  
  app().registerHandler(
    "/unitedgas/12",
    [well_ptr_12](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      if (well_ptr_12 == nullptr)
        std::cout << "well_ptr_12 is NULL" << std::endl;
      
      HttpViewData data;
      
      data["wellname"] = well_ptr_12->get_wellname();
      data["wellno"] = well_ptr_12->get_wellno();
      data["dateOfRecentTest"] = well_ptr_12->get_dateOfRecentTest();
      data["dailyOil"] = well_ptr_12->get_dailyOil();
      data["dailyWater"] = well_ptr_12->get_dailyWater();
      data["dailyGas"] = well_ptr_12->get_dailyGas();
      data["opPressureTubing"] = well_ptr_12->get_opPressureTubing();
      data["opPressureCasing"] = well_ptr_12->get_opPressureCasing();
      data["pumpingUnitSize"] = well_ptr_12->get_pumpingUnitSize();
      data["strokesPerMin"] = well_ptr_12->get_strokesPerMin();
      data["strokeLength"] = well_ptr_12->get_strokeLength();
      data["motorHp"] = well_ptr_12->get_motorHp();
      data["pumpingRatio"] = well_ptr_12->get_pumpingRatio();
      data["unitGearRatio"] = well_ptr_12->get_unitGearRatio();
      data["casingSize"] = well_ptr_12->get_casingSize();
      data["tubingSize"] = well_ptr_12->get_tubingSize();
      data["pumpSize"] = well_ptr_12->get_pumpSize();
      data["firstCole"] = well_ptr_12->get_firstCole();
      data["secondCole"] = well_ptr_12->get_secondCole();
      data["thirdCole"] = well_ptr_12->get_thirdCole();
      data["comments"] = well_ptr_12->get_comments();

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
    "/formHandler",
    [well_ptr_12](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
      
      std::string data, encoded_data;
      if (well_ptr_12 == nullptr)
        std::cout << "well_ptr_12 is NULL" << std::endl;

      if (!req->getParameter("wellname").empty()) {
        data = req->getParameter("wellname");
        encoded_data = encode(data);
        well_ptr_12->set_wellname(encoded_data);
      }
      if (!req->getParameter("wellno").empty()) {
        data = req->getParameter("wellno");
        encoded_data = encode(data);
        well_ptr_12->set_wellno(encoded_data);
      }
      if (!req->getParameter("dateOfRecentTest").empty()) {
        data = req->getParameter("dateOfRecentTest");
        encoded_data = encode(data);
        well_ptr_12->set_dateOfRecentTest(encoded_data);
      }
      if (!req->getParameter("dailyOil").empty()) {
        data = req->getParameter("dailyOil");
        encoded_data = encode(data);
        well_ptr_12->set_dailyOil(encoded_data);
      }
      if (!req->getParameter("dailyWater").empty()) {
        data = req->getParameter("dailyWater");
        encoded_data = encode(data);
        well_ptr_12->set_dailyWater(encoded_data);
      }
      if (!req->getParameter("dailyGas").empty()) {
        data = req->getParameter("dailyGas");
        encoded_data = encode(data);
        well_ptr_12->set_dailyGas(encoded_data);
      }
      if (!req->getParameter("opPressureTubing").empty()) {
        data = req->getParameter("opPressureTubing");
        encoded_data = encode(data);
        well_ptr_12->set_opPressureTubing(encoded_data);
      }
      if (!req->getParameter("opPressureCasing").empty()) {
        data = req->getParameter("opPressureCasing");
        encoded_data = encode(data);
        well_ptr_12->set_opPressureCasing(encoded_data);
      }
      if (!req->getParameter("pumpingUnitSize").empty()) {
        data = req->getParameter("pumpingUnitSize");
        encoded_data = encode(data);
        well_ptr_12->set_pumpingUnitSize(encoded_data);
      }
      if (!req->getParameter("strokesPerMin").empty()) {
        data = req->getParameter("strokesPerMin");
        encoded_data = encode(data);
        well_ptr_12->set_strokesPerMin(encoded_data);
      }
      if (!req->getParameter("strokeLength").empty()) {
        data = req->getParameter("strokeLength");
        encoded_data = encode(data);
        well_ptr_12->set_strokeLength(encoded_data);
      }
      if (!req->getParameter("motorHp").empty()) {
        data = req->getParameter("motorHp");
        encoded_data = encode(data);
        well_ptr_12->set_motorHp(encoded_data);
      }
      if (!req->getParameter("pumpingRatio").empty()) {
        data = req->getParameter("pumpingRatio");
        encoded_data = encode(data);
        well_ptr_12->set_pumpingRatio(encoded_data);
      }
      if (!req->getParameter("unitGearRatio").empty()) {
        data = req->getParameter("unitGearRatio");
        encoded_data = encode(data);
        well_ptr_12->set_unitGearRatio(encoded_data);
      }
      if (!req->getParameter("casingSize").empty()) {
        data = req->getParameter("casingSize");
        encoded_data = encode(data);
        well_ptr_12->set_casingSize(encoded_data);
      }
      if (!req->getParameter("tubingSize").empty()) {
        data = req->getParameter("tubingSize");
        encoded_data = encode(data);
        well_ptr_12->set_tubingSize(encoded_data);
      }
      if (!req->getParameter("pumpSize").empty()) {
        data = req->getParameter("pumpSize");
        encoded_data = encode(data);
        well_ptr_12->set_pumpSize(encoded_data);
      }
      if (!req->getParameter("firstCole").empty()) {
        data = req->getParameter("firstCole");
        encoded_data = encode(data);
        well_ptr_12->set_firstCole(encoded_data);
      }
      if (!req->getParameter("secondCole").empty()) {
        data = req->getParameter("secondCole");
        encoded_data = encode(data);
        well_ptr_12->set_secondCole(encoded_data);
      }
      if (!req->getParameter("thirdCole").empty()) {
        data = req->getParameter("thirdCole");
        encoded_data = encode(data);
        well_ptr_12->set_thirdCole(encoded_data);
      }
      if (!req->getParameter("comments").empty()) {
        data = req->getParameter("comments");
        encoded_data = encode(data);
        well_ptr_12->set_comments(encoded_data);
      }
      well_ptr_12->writetoFile();
      auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/12");
      callback(resp);
    },
    {Post});


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
