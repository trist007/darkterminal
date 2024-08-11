#include <drogon/HttpController.h>
#include <unitedgas/Well.h>
#include <memory>
#include <iostream>
#include <string>

using namespace drogon;

// HttpControllers are automatically added to Drogon upon Drogon initializing.
class UnitedGas : public HttpController<UnitedGas>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(UnitedGas::index, "/", Get);
    METHOD_ADD(UnitedGas::formHandler, "/formHandler", Post);
    METHOD_ADD(UnitedGas::personalizedHello, "/hello", Get);
    METHOD_LIST_END

  protected:
    void index(const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
    {
        std::shared_ptr<Well> well_ptr = std::make_shared<Well>("2",
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
                      "Packer @600'");
        HttpViewData data;

        // Use the object's methods to get the name and well number
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
        callback(resp);
    }

    void formHandler(const HttpRequestPtr &req,
    std::function<void (const HttpResponsePtr &)> &&callback)
    {
        HttpViewData data;
        auto resp=HttpResponse::newHttpResponse();
        //data["wellno"] = req->getParameter("wellno");
        std::string wellno = req->getParameter("wellno");
        callback(resp);
    }
    //REGISTER_INTERFACE("/formHandler","formHandler",{&formHandler},"wellno");

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
