#include <fstream>
#include <iostream>
#include <string>


class Well {
  public:
    Well(const std::string filename)
                  :m_wellno(""), m_dailyOil(""), m_dailyWater(""), m_dailyGas(""),
                  m_opPressureTubing(""), m_opPressureCasing(""), m_strokesPerMin(""),
                  m_strokeLength(""), m_motorHp(""), m_pumpingRatio(""), m_unitGearRatio(""),
                  m_wellname(""), m_dateOfRecentTest(""), m_pumpingUnitSize(""),
                  m_casingSize(""), m_depth(""), m_tubingSize(""), m_pumpSize(""),
                  m_firstCole(""), m_secondCole(""), m_thirdCole(""), m_comments(""),
                  m_filename(filename)
    {
      std::ifstream myfile;

      myfile.open (filename);
      if(!myfile) {
        throw std::runtime_error("error opening file");
      }

      std::getline(myfile, m_wellno);
      std::getline(myfile, m_dailyOil);
      std::getline(myfile, m_dailyWater);
      std::getline(myfile, m_dailyGas);
      std::getline(myfile, m_opPressureTubing);
      std::getline(myfile, m_opPressureCasing);
      std::getline(myfile, m_strokesPerMin);
      std::getline(myfile, m_strokeLength);
      std::getline(myfile, m_motorHp);
      std::getline(myfile, m_pumpingRatio);
      std::getline(myfile, m_unitGearRatio);
      std::getline(myfile, m_wellname);
      std::getline(myfile, m_dateOfRecentTest);
      std::getline(myfile, m_pumpingUnitSize);
      std::getline(myfile, m_casingSize);
      std::getline(myfile, m_depth);
      std::getline(myfile, m_tubingSize);
      std::getline(myfile, m_pumpSize);
      std::getline(myfile, m_firstCole);
      std::getline(myfile, m_secondCole);
      std::getline(myfile, m_thirdCole);
      std::getline(myfile, m_comments);

      myfile.close();
    }

    int writetoFile()
    {
      std::ofstream myfile;

      myfile.open (m_filename);
      if(!myfile) {
        throw std::runtime_error("error opening file");
      }

      myfile << m_wellno << std::endl;
      myfile << m_dailyOil << std::endl;
      myfile << m_dailyWater << std::endl;
      myfile << m_dailyGas << std::endl;
      myfile << m_opPressureTubing << std::endl;
      myfile << m_opPressureCasing << std::endl;
      myfile << m_strokesPerMin << std::endl;
      myfile << m_strokeLength << std::endl;
      myfile << m_motorHp << std::endl;
      myfile << m_pumpingRatio << std::endl;
      myfile << m_unitGearRatio << std::endl;
      myfile << m_wellname << std::endl;
      myfile << m_dateOfRecentTest << std::endl;
      myfile << m_pumpingUnitSize << std::endl;
      myfile << m_casingSize << std::endl;
      myfile << m_depth << std::endl;
      myfile << m_tubingSize << std::endl;
      myfile << m_pumpSize << std::endl;
      myfile << m_firstCole << std::endl;
      myfile << m_secondCole << std::endl;
      myfile << m_thirdCole << std::endl;
      myfile << m_comments << std::endl;

      myfile.close();

      return 0;
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
    m_comments, m_filename;

};

