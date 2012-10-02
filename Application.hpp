#ifndef _EEH_APP_HPP_
#define _EEH_APP_HPP_

#include <cmath>
#include <cstdint>

#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>

#include <cctype>

namespace App
{
    struct ClosestMatch
    {
      uint32_t r1;
      uint32_t r2;
      float ratio;
      double RatioDifference;

      ClosestMatch(uint32_t r1_, uint32_t r2_, float ratio_, double ratiodiffIn)
          : r1(r1_), r2(r2_), ratio(ratio_), RatioDifference(ratiodiffIn)
      {}
    };

  void Init();
  void Run();
  void Destroy();
}

#endif // _EEH_APP_HPP_


#define VERSION_NUMBER "0.0.1"


#ifdef NEVER
class Application : public Singleton<Application>
{


    std::map<std::string, std::vector<double>> mEValues;
    char        mUserInput; // A buffer variable for storing a single character of user input - eg. a number or y/n answer.
    float       mDesiredRatio;
    float       mSupplyVoltage;
    float       mScaleFactor;
    std::string      mInput;
    std::string mSeries;
    bool mDone;

public:
    Application();

    void Run();
private:
    /*
    * A member function that prints out the main menu options.
    */
    void DisplayMenu();

    /*
    * Performs an action based on the value held in mUserInput.
    *
    * It is assumed that this is called only once for every printing of the main menu, so that input isn't processed twice.
    */
    void UseInput();

    /*
    * Clears the screen, by outputting many newline characters. This improves readability of the console interface.
    */
    inline void ClearScreen()
    {
        std::cout << std::string ( 100, '\n' );
    }

    void CalculateResistors();

    void ChangeSeries();

    void PrintMatches();

    void PrintHelp();

    void ClearMatches()
    {
        for(std::list<ClosestMatch*>::iterator it = mMatches.begin(); it != mMatches.end(); ++it)
        {
            delete (*it);
            (*it) = nullptr;
        }
        mMatches.clear();
    }
};

#endif
