#include <drogon/HttpController.h>
#include <drogon/utils/Utilities.h>
#include <drogon/drogon.h>
#include <string>

#include <unitedgas/Well.h>

using namespace drogon;
using namespace std::chrono_literals;
using namespace drogon;

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

//std::vector<std::shared_ptr<Well>> well_ptrs;
std::map<std::string, std::shared_ptr<Well>> wellListMap = initializeWellDB();


// HttpControllers are automatically added to Drogon upon Drogon initializing.
class unitedgas : public HttpController<unitedgas>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(unitedgas::home, "/", Get);
    METHOD_ADD(unitedgas::login, "/login", Post);
    METHOD_ADD(unitedgas::logout, "/logout", Get);
    METHOD_ADD(unitedgas::index, "/index", Get);
    METHOD_ADD(unitedgas::view, "/{1}", Get);
    METHOD_ADD(unitedgas::modify, "/{1}", Post);
    METHOD_LIST_END

  protected:
    void home(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {
      bool loggedIn =
      req->session()->getOptional<bool>("loggedIn").value_or(false);
      HttpResponsePtr resp;
      if (loggedIn == false)
        resp = HttpResponse::newHttpViewResponse("LoginPage");
      else
        resp = HttpResponse::newHttpViewResponse("LogoutPage");
      callback(resp);
    }

    void login(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {

      std::string user = req->getParameter("user");
      std::string passwd = req->getParameter("passwd");
      std::string sal = "419d777683667fe0b811bbafd03c1fb1";
      std::string hash;
      const char* env_variable = "cuidado";
      char* value;
      value = getenv(env_variable);
      if (value == nullptr)
        throw std::runtime_error("wellListMap[well] is NULL");

      // NOTE: Do not use MD5 for the password hash under any
      // circumstances. We only use it because Drogon is not a
      // cryptography library, so it does not include a better hash
      // algorithm. Use Argon2 or BCrypt in a real product.
      //
      hash = utils::getSha256(passwd + sal);
      if (value == hash) {
        req->session()->insert("loggedIn", true);
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/index");
        callback(resp);
      } else {
        auto clientIP = req->getPeerAddr();
        LOG_INFO << "Authentication failed from " << clientIP.toIp();
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/");
        callback(resp);
      }
    }

    void logout(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {
      HttpResponsePtr resp = HttpResponse::newHttpResponse();
      req->session()->erase("loggedIn");
      resp->setBody("Logged out");
      callback(resp);
    }

    void index(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {
      HttpResponsePtr resp;
      bool loggedIn =
          req->session()->getOptional<bool>("loggedIn").value_or(false);
      if (loggedIn == false)
        resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/login");
      else
        resp = HttpResponse::newHttpViewResponse("UnitedGasView");
      callback(resp);
    }

    void view(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
                        std::string well)
    {
     if (wellListMap[well] == nullptr)
      throw std::runtime_error("wellListMap[well] is NULL");

     auto it = wellListMap.find(well);

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
        resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/");
      else
        resp = HttpResponse::newHttpViewResponse("Well", data);
      callback(resp);
    }


    void modify(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
                        std::string well)
    {
      if (wellListMap[well] == nullptr)
        throw std::runtime_error("wellListMap[well] is NULL");

      std::string data, encoded_data;

      auto it = wellListMap.find(well);

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

      auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/" + well);
      callback(resp);
    }
};
