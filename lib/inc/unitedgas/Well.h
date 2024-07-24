#include <string>

class Well {
  public:
    Well(const std::string& wellno, const std::string& dailyOil, const std::string& dailyWater, const std::string& dailyGas, const std::string& opPressureTubing, const std::string& opPressureCasing, const std::string& strokesPerMin, const std::string& strokeLength, const std::string& motorHp, const std::string& pumpingRatio, const std::string& unitGearRatio, const std::string& wellname, const std::string& dateOfRecentTest, const std::string& pumpingUnitSize, const std::string& casingSize, const std::string& depth, const std::string& tubingSize, const std::string& pumpSize, const std::string& firstCole, const std::string& secondCole, const std::string& thirdCole, const std::string& comments)
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
    // getters
    const std::string get_wellno() const { return m_wellno; }
    const std::string get_dailyOil() const { return m_dailyOil; }
    const std::string get_dailyWater() const { return m_dailyWater; }
    const std::string get_dailyGas() const { return m_dailyGas; }
    const std::string get_opPressureTubing() const { return m_opPressureTubing; }
    const std::string get_opPressureCasing() const { return m_opPressureCasing; }
    const std::string get_strokesPerMin() const { return m_strokesPerMin; }
    const std::string get_strokeLength() const { return m_strokeLength; }
    const std::string get_motorHp() const { return m_motorHp; }
    const std::string get_pumpingRatio() const { return m_pumpingRatio; }
    const std::string get_unitGearRatio() const { return m_unitGearRatio; }
    const std::string get_wellname() const { return m_wellname; }
    const std::string get_dateOfRecentTest() const { return m_dateOfRecentTest; }
    const std::string get_pumpingUnitSize() const { return m_pumpingUnitSize; }
    const std::string get_casingSize() const { return m_casingSize; }
    const std::string get_depth() const { return m_depth; }
    const std::string get_tubingSize() const { return m_tubingSize; }
    const std::string get_pumpSize() const { return m_pumpSize; }
    const std::string get_firstCole() const { return m_firstCole; }
    const std::string get_secondCole() const { return m_secondCole; }
    const std::string get_thirdCole() const { return m_thirdCole; }
    const std::string get_comments() const { return m_comments; }

    // setters
    void set_wellno(const std::string& newvalue) { m_wellno = newvalue; }
    void set_dailyOil(const std::string& newvalue) { m_dailyOil = newvalue; }
    void set_dailyWater(const std::string& newvalue) { m_dailyWater = newvalue; }
    void set_dailyGas(const std::string& newvalue) { m_dailyGas = newvalue; }
    void set_opPressureTubing(const std::string& newvalue) { m_opPressureTubing = newvalue; }
    void set_opPressureCasing(const std::string& newvalue) { m_opPressureCasing = newvalue; }
    void set_strokesPerMin(const std::string& newvalue) { m_strokesPerMin = newvalue; }
    void set_strokeLength(const std::string& newvalue) { m_strokeLength = newvalue; }
    void set_motorHp(const std::string& newvalue) { m_motorHp = newvalue; }
    void set_pumpingRatio(const std::string& newvalue) { m_pumpingRatio = newvalue; }
    void set_unitGearRatio(const std::string& newvalue) { m_unitGearRatio = newvalue; }
    void set_wellname(const std::string& newvalue) { m_wellname = newvalue; }
    void set_dateOfRecentTest(const std::string& newvalue) { m_dateOfRecentTest = newvalue; }
    void set_pumpingUnitSize(const std::string& newvalue) { m_pumpingUnitSize = newvalue; }
    void set_casingSize(const std::string& newvalue) { m_casingSize = newvalue; }
    void set_depth(const std::string& newvalue) { m_depth = newvalue; }
    void set_tubingSize(const std::string& newvalue) { m_tubingSize = newvalue; }
    void set_pumpSize(const std::string& newvalue) { m_pumpSize = newvalue; }
    void set_firstCole(const std::string& newvalue) { m_firstCole = newvalue; }
    void set_secondCole(const std::string& newvalue) { m_secondCole = newvalue; }
    void set_thirdCole(const std::string& newvalue) { m_thirdCole = newvalue; }
    void set_comments(const std::string& newvalue) { m_comments = newvalue; }

  private:
    std::string m_wellno, m_dailyOil, m_dailyWater, m_dailyGas, m_opPressureTubing, m_opPressureCasing,
    m_strokesPerMin, m_strokeLength, m_motorHp, m_pumpingRatio, m_unitGearRatio, m_wellname, m_dateOfRecentTest,
    m_pumpingUnitSize, m_casingSize, m_depth, m_tubingSize, m_pumpSize, m_firstCole, m_secondCole, m_thirdCole,
    m_comments;
};

