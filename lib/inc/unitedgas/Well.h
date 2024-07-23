#include <string>

class Well {
public:
    Well(std::string wellno, std::string dailyOil, std::string dailyWater, std::string dailyGas, std::string opPressureTubing, std::string opPressureCasing, std::string strokesPerMin, std::string strokeLength, std::string motorHp, std::string pumpingRatio, std::string unitGearRatio, std::string wellname, std::string dateOfRecentTest, std::string pumpingUnitSize, std::string casingSize, std::string depth, std::string tubingSize, std::string pumpSize, std::string firstCole, std::string secondCole, std::string thirdCole, std::string comments)
    {
          m_wellno = wellno;
          m_dailyOil = dailyOil;
          m_dailyWater = dailyWater;
          m_dailyGas = dailyGas;
          m_opPressureTubing = opPressureTubing;
          m_opPressureCasing = opPressureCasing;
          m_strokesPerMin = strokesPerMin;
          m_strokeLength = strokeLength;
          m_motorHp = motorHp;
          m_pumpingRatio = pumpingRatio;
          m_unitGearRatio = unitGearRatio;
          m_wellname = wellname;
          m_dateOfRecentTest = dateOfRecentTest;
          m_pumpingUnitSize = pumpingUnitSize;
          m_casingSize = casingSize;
          m_depth = depth;
          m_tubingSize = tubingSize;
          m_pumpSize = pumpSize;
          m_firstCole = firstCole;
          m_secondCole = secondCole;
          m_thirdCole = thirdCole;
          m_comments = comments;
        }

    std::string m_wellno, m_dailyOil, m_dailyWater, m_dailyGas, m_opPressureTubing, m_opPressureCasing,
    m_strokesPerMin, m_strokeLength, m_motorHp, m_pumpingRatio, m_unitGearRatio, m_wellname, m_dateOfRecentTest,
    m_pumpingUnitSize, m_casingSize, m_depth, m_tubingSize, m_pumpSize, m_firstCole, m_secondCole, m_thirdCole,
    m_comments;

    std::string getName() { return "Some Name"; }
    int getWellNo() { return 12345; }
};

