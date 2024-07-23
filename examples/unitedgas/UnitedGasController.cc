#include <drogon/HttpController.h>
#include <unitedgas/Well.h>
#include <iostream>
#include <string>

using namespace drogon;

// HttpControllers are automatically added to Drogon upon Drogon initializing.
class UnitedGas : public HttpController<UnitedGas>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(UnitedGas::index, "/", Get);
    METHOD_ADD(UnitedGas::personalizedHello, "/hello", Get);
    METHOD_LIST_END

  protected:
    void index(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {
        HttpViewData data;
        // Create a user data object
        Well myobj{"2",
                          "1",
                          "295",
                          "0",
                          "0",
                          "0",
                          "10",
                          "48",
                          "20",
                          "0",
                          "0",
                          "Number 2",
                          "8/10/2016",
                          "National 67B",
                          std::string(),
                          "1710??",
                          "2 7/8",
                          "2 3/4",
                          "1697 - 1701",
                          std::string(),
                          std::string(),
                          "Packer @600'"};
        // Use the object's methods to get the name and well number
        data["wellname"] = myobj.m_wellname;
        data["wellno"] = myobj.m_wellno;

        auto resp = HttpResponse::newHttpViewResponse("UnitedGasView", data);
        callback(resp);
    }

    void personalizedHello(
        const HttpRequestPtr &,
        std::function<void(const HttpResponsePtr &)> &&callback)
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody(
            "Hi there, this is another hello from the SayHello Controller "
            "in United Gas");
        callback(resp);
    }
};