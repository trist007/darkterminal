#include <drogon/drogon.h>
#include <iostream>
#include <string>
#include <memory>

#include <unitedgas/Well.h>

using namespace drogon;

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
        "/",
        [well_ptr](const HttpRequestPtr &,
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

        auto resp = HttpResponse::newHttpViewResponse("UnitedGasView", data);
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
          well_ptr->set_dailyOil(req->getParameter("dailyOil"));
        if (!req->getParameter("dailyWater").empty())
          well_ptr->set_dailyWater(req->getParameter("dailyWater"));
        if(!req->getParameter("dailyGas").empty())
          well_ptr->set_dailyGas(req->getParameter("dailyGas"));
        if(!req->getParameter("opPressureTubing").empty())
          well_ptr->set_opPressureTubing(req->getParameter("opPressureTubing"));
        if(!req->getParameter("opPressureCasing").empty())
          well_ptr->set_opPressureCasing(req->getParameter("opPressureCasing"));
        if(!req->getParameter("strokesPerMin").empty())
          well_ptr->set_strokesPerMin(req->getParameter("strokesPerMin"));
        if(!req->getParameter("strokeLength").empty())
          well_ptr->set_strokeLength(req->getParameter("strokeLength"));
        if(!req->getParameter("motorHp").empty())
          well_ptr->set_motorHp(req->getParameter("motorHp"));
        if(!req->getParameter("pumpingRatio").empty())
          well_ptr->set_pumpingRatio(req->getParameter("pumpingRatio"));
        if(!req->getParameter("unitGearRatio").empty())
          well_ptr->set_unitGearRatio(req->getParameter("unitGearRatio"));
        if(!req->getParameter("wellname").empty())
          well_ptr->set_wellname(req->getParameter("wellname"));
        if(!req->getParameter("dateOfRecentTest").empty())
          well_ptr->set_dateOfRecentTest(req->getParameter("dateOfRecentTest"));
        if(!req->getParameter("pumpingUnitSize").empty())
          well_ptr->set_pumpingUnitSize(req->getParameter("pumpingUnitSize"));
        if(!req->getParameter("casingSize").empty())
          well_ptr->set_casingSize(req->getParameter("casingSize"));
        if(!req->getParameter("depth").empty())
          well_ptr->set_depth(req->getParameter("depth"));
        if(!req->getParameter("tubingSize").empty())
          well_ptr->set_tubingSize(req->getParameter("tubingSize"));
        if(!req->getParameter("pumpSize").empty())
          well_ptr->set_pumpSize(req->getParameter("pumpSize"));
        if(!req->getParameter("firstCole").empty())
          well_ptr->set_firstCole(req->getParameter("firstCole"));
        if(!req->getParameter("secondCole").empty())
          well_ptr->set_secondCole(req->getParameter("secondCole"));
        if(!req->getParameter("thirdCole").empty())
          well_ptr->set_thirdCole(req->getParameter("thirdCole"));
        if(!req->getParameter("comments").empty())
          well_ptr->set_comments(req->getParameter("comments"));
        //auto resp = HttpResponse::newHttpResponse();
        well_ptr->writetoFile();
        auto resp = HttpResponse::newRedirectionResponse("http://localhost:8848");
            //auto resp = HttpResponse::newHttpResponse();
            //resp->setBody("Hello");
            callback(resp);
        },
        {Post});
    // `registerHandler()` also supports parsing and passing the path as

   // parameters to the handler. Parameters are specified using {}. The text
    // inside the {} does not correspond to the index of parameter passed to the
    // handler (nor it has any meaning). Instead, it is only to make it easier
    // for users to recognize the function of each parameter.
    app().registerHandler(
        "/user/{user-name}",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback,
           const std::string &name) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, " + name + "!");
            callback(resp);
        },
        {Get});

    // You can also specify that the parameter is in the query section of the
    // URL!
    app().registerHandler(
        "/hello?user={user-name}",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback,
           const std::string &name) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, " + name + "!");
            callback(resp);
        },
        {Get});

    // Or, if you want to, instead of asking drogon to parse it for you. You can
    // parse the request yourselves.
    app().registerHandler(
        "/hello_user",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            auto name = req->getOptionalParameter<std::string>("user");
            if (!name)
                resp->setBody("Please tell me your name");
            else
                resp->setBody("Hello, " + name.value() + "!");
            callback(resp);
        },
        {Get});

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
