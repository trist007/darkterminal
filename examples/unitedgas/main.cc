#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <string>
#include <chrono>

#include <unitedgas/Well.h>
#include <trantor/utils/crypto/sha256.h>

using namespace drogon;
using namespace std::chrono_literals;

std::string encode(std::string &data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch (data[pos])
        {
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
    // data.swap(buffer);
    return buffer;
}

int main()
{
  std::shared_ptr<Well> well_ptr;
//Well *well_ptr;
    try
    {
        well_ptr = std::make_shared<Well>("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/12.db");
        //well_ptr = new Well("/Users/trist007/CLionProjects/darkterminal/examples/unitedgas/12.db");
    }
    catch (std::bad_alloc& ba)
    {
        std::cout << "unable to allocate memory" << std::endl;
    }
    // `registerHandler()` adds a handler to the desired path. The handler is
    // responsible for generating a HTTP response upon an HTTP request being
    // sent to Drogon
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
            //resp = HttpResponse::newHttpViewResponse("LogoutPage");
            //resp->setBody("<script>window.location.href = \"/\";</script>");
            resp->setBody("Logged out");
            callback(resp);
        },
        {Post});

    app().registerHandler(
        "/login",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            HttpResponsePtr resp = HttpResponse::newHttpResponse();
            std::string user = req->getParameter("user");
            std::string passwd = req->getParameter("passwd");
            //trantor::utils::Hash256 hash;
            //trantor::utils::Hash256 hash;
            std::string hash;

            // NOTE: Do not use MD5 for the password hash under any
            // circumstances. We only use it because Drogon is not a
            // cryptography library, so it does not include a better hash
            // algorithm. Use Argon2 or BCrypt in a real product. username:
            // user, password: password123
            /*
            if (user == "user" && trantor::utils::sha256("jadsjhdsajkh" + passwd) ==
                                      "227cf1f3a63637b746cb3223925c6ddf733213a174571b7ad128688ebed5782e")
                                      */
            hash = utils::getSha256(passwd);
            if (hash == "1C1F1024D78CBFA8DF28545FA462D9FA461D0D6874A2B5E2FCC7214D6C78B9BA")
            {
                req->session()->insert("loggedIn", true);
                //resp->setBody("<script>window.location.href = \"localhost:8848/unitedgas\";</script>");
                auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/");
                callback(resp);
            }
            else
            {
                resp->setStatusCode(k401Unauthorized);
                //resp->setBody("<script>window.location.href = \"/\";</script>");
                auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas");
                //resp->setBody("403 Forbidden");
                callback(resp);
            }
        },
        {Post});

    app().registerHandler(
        "/unitedgas/",
        [well_ptr](const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback) {

        if (well_ptr == nullptr)
            std::cout << "well_ptr is NULL" << std::endl;
        HttpViewData data;
        data["wellno"] = well_ptr->get_wellno();
        data["dailyOil"] = well_ptr->get_dailyOil();
        data["dailyWater"] = well_ptr->get_dailyWater();
        data["dailyGas"] = well_ptr->get_dailyGas();
        data["opPressureTubing"] = well_ptr->get_opPressureTubing();
        data["opPressureCasing"] = well_ptr->get_opPressureCasing();
        data["strokesPerMin"] = well_ptr->get_strokesPerMin();
        data["strokeLength"] = well_ptr->get_strokeLength();
        data["motorHp"] = well_ptr->get_motorHp();
        data["pumpingRatio"] = well_ptr->get_pumpingRatio();
        data["unitGearRatio"] = well_ptr->get_unitGearRatio();
        data["wellname"] = well_ptr->get_wellname();
        data["dateOfRecentTest"] = well_ptr->get_dateOfRecentTest();
        data["pumpingUnitSize"] = well_ptr->get_pumpingUnitSize();
        data["casingSize"] = well_ptr->get_casingSize();
        data["depth"] = well_ptr->get_depth();
        data["tubingSize"] = well_ptr->get_tubingSize();
        data["pumpSize"] = well_ptr->get_pumpSize();
        data["firstCole"] = well_ptr->get_firstCole();
        data["secondCole"] = well_ptr->get_secondCole();
        data["thirdCole"] = well_ptr->get_thirdCole();
        data["comments"] = well_ptr->get_comments();

        HttpResponsePtr resp;
        bool loggedIn =
                req->session()->getOptional<bool>("loggedIn").value_or(false);
        if (loggedIn == false)
          resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas");
        else
          resp = HttpResponse::newHttpViewResponse("UnitedGasView", data);
        //auto resp = HttpResponse::newHttpViewResponse("UnitedGasView", data);
            //auto resp = HttpResponse::newHttpResponse();
            //resp->setBody("Hello");
        callback(resp);
        },
        {Get});

        app().registerHandler(
        "/formHandler",
        [well_ptr](const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback) {

        std::string data, encoded_data;

        if (!req->getParameter("wellno").empty())
        {
            data = req->getParameter("wellno");
            encoded_data = encode(data);
            well_ptr->set_wellno(encoded_data);
        }
        if (!req->getParameter("dailyOil").empty())
        {
            data = req->getParameter("dailyOil");
            encoded_data = encode(data);
            well_ptr->set_dailyOil(encoded_data);
        }
        if (!req->getParameter("dailyWater").empty())
        {
            data = req->getParameter("dailyWater");
            encoded_data = encode(data);
            well_ptr->set_dailyWater(encoded_data);
        }
        if(!req->getParameter("dailyGas").empty())
        {
            data = req->getParameter("dailyGas");
            encoded_data = encode(data);
            well_ptr->set_dailyGas(encoded_data);
        }
        if(!req->getParameter("opPressureTubing").empty())
        {
            data = req->getParameter("opPressureTubing");
            encoded_data = encode(data);
            well_ptr->set_opPressureTubing(encoded_data);
        }
        if(!req->getParameter("opPressureCasing").empty())
        {
            data = req->getParameter("opPressureCasing");
            encoded_data = encode(data);
            well_ptr->set_opPressureCasing(encoded_data);
        }
        if(!req->getParameter("strokesPerMin").empty())
        {
            data = req->getParameter("strokesPerMin");
            encoded_data = encode(data);
            well_ptr->set_strokesPerMin(encoded_data);
        }
        if(!req->getParameter("strokeLength").empty())
        {
            data = req->getParameter("strokeLength");
            encoded_data = encode(data);
            well_ptr->set_strokeLength(encoded_data);
        }
        if(!req->getParameter("motorHp").empty())
        {
            data = req->getParameter("motorHp");
            encoded_data = encode(data);
            well_ptr->set_motorHp(encoded_data);
        }
        if(!req->getParameter("pumpingRatio").empty())
        {
            data = req->getParameter("pumpingRatio");
            encoded_data = encode(data);
            well_ptr->set_pumpingRatio(encoded_data);
        }
        if(!req->getParameter("unitGearRatio").empty())
        {
            data = req->getParameter("unitGearRatio");
            encoded_data = encode(data);
            well_ptr->set_unitGearRatio(encoded_data);
        }
        if(!req->getParameter("wellname").empty())
        {
            data = req->getParameter("wellname");
            encoded_data = encode(data);
            well_ptr->set_wellname(encoded_data);
        }
        if(!req->getParameter("dateOfRecentTest").empty())
        {
            data = req->getParameter("dateOfRecentTest");
            encoded_data = encode(data);
            well_ptr->set_dateOfRecentTest(encoded_data);
        }
        if(!req->getParameter("pumpingUnitSize").empty())
        {
            data = req->getParameter("pumpingUnitSize");
            encoded_data = encode(data);
            well_ptr->set_pumpingUnitSize(encoded_data);
        }
        if(!req->getParameter("casingSize").empty())
        {
            data = req->getParameter("casingSize");
            encoded_data = encode(data);
            well_ptr->set_casingSize(encoded_data);
        }
        if(!req->getParameter("depth").empty())
        {
            data = req->getParameter("depth");
            encoded_data = encode(data);
            well_ptr->set_depth(encoded_data);
        }
        if(!req->getParameter("tubingSize").empty())
        {
            data = req->getParameter("tubingSize");
            encoded_data = encode(data);
            well_ptr->set_tubingSize(encoded_data);
        }
        if(!req->getParameter("pumpSize").empty())
        {
            data = req->getParameter("pumpSize");
            encoded_data = encode(data);
            well_ptr->set_pumpSize(encoded_data);
        }
        if(!req->getParameter("firstCole").empty())
        {
            data = req->getParameter("firstCole");
            encoded_data = encode(data);
            well_ptr->set_firstCole(encoded_data);
        }
        if(!req->getParameter("secondCole").empty())
        {
            data = req->getParameter("secondCole");
            encoded_data = encode(data);
            well_ptr->set_secondCole(encoded_data);
        }
        if(!req->getParameter("thirdCole").empty())
        {
            data = req->getParameter("thirdCole");
            encoded_data = encode(data);
            well_ptr->set_thirdCole(encoded_data);
        }
        if(!req->getParameter("comments").empty())
        {
            data = req->getParameter("comments");
            encoded_data = encode(data);
            well_ptr->set_comments(encoded_data);
        }
        //auto resp = HttpResponse::newHttpResponse();
        well_ptr->writetoFile();
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848/unitedgas/");
            //auto resp = HttpResponse::newHttpResponse();
            //resp->setBody("Hello");
            callback(resp);
        },
        {Post});
    // `registerHandler()` also supports parsing and passing the path as


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
