#include "Application.hpp"

using namespace std;

namespace App
{
  namespace
  {
    map<string, uint8_t> series_map = {{"E3",0},{"E6",1},{"E12",2},{"E24",3},{"E48",4},{"E96",5},{"E192",6}};

    array<uint16_t,24> E_two_dp = {{100, 110, 120, 130, 150, 160, 180, 200, 220, 240, 270, 300, 330, 360, 390, 430, 470, 510, 560, 620, 680, 750, 820, 910}};
    array<uint16_t,192> E_three_dp = {{100, 101, 102, 104, 105, 106, 107, 109, 110, 111, 113, 114, 115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140, 142, 143, 145, 162, 164, 165, 167, 169, 172, 174, 176, 178, 180, 182, 184, 187, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213, 215, 218, 221, 223, 226, 229, 232, 234, 237, 240, 243, 246, 249, 252, 255, 258, 261, 264, 267, 271, 274, 277, 280, 284, 287, 291, 294, 298, 301, 305, 309, 312, 316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 357, 361, 365, 370, 374, 379, 383, 388, 392, 397, 402, 407, 412, 417, 422, 427, 432, 437, 442, 448, 453, 459, 464, 470, 475, 481, 487, 493, 499, 505, 511, 517, 523, 530, 536, 542, 549, 556, 562, 569, 576, 583, 590, 597, 604, 612, 619, 626, 634, 642, 649, 657, 665, 673, 681, 690, 698, 706, 715, 723, 732, 741, 750, 759, 768, 777, 787, 796, 806, 816, 825, 835, 845, 856, 866, 876, 887, 898, 909, 919, 931, 942, 953, 965, 976, 988}};

    uint8_t current_series = series_map["E12"];

    bool done = false;

    float desired_ratio = 0.5f;

    std::list<ClosestMatch*> mMatches;

    float supply = 0.0f;

    void DisplayMenu()
    {
      cout << endl << endl;
      cout << "Menu"                                                                              << endl;
      cout << "-----------"                                                                       << endl;

      cout << "1. Enter Voltage Proportion"                                                       << endl;
      cout << "2. Enter Supply Voltage and Required Voltage"                                      << endl;
      cout << "3. Change Resistor Series"                                                         << endl;
      cout << "4. Print Matches"                                                                  << endl;
      cout << "5. Print Help"                                                                     << endl;
      cout << "6. Exit"                                                                           << endl;
      cout << endl;

      cout << "Enter your option: ";

      return;
    }

    void CLS()
    {
        cout << string ( 100, '\n' );
    }

  void CalculateResistors()
  {
    if(desired_ratio >= 1.0f)
        return;

    if(desired_ratio < 0.0f)
        return;

    //ClearMatches();

    uint16_t * series_ptr = nullptr;
    uint8_t size = 3 << current_series;
    uint8_t inc;

    if(current_series > 3)
    {
      series_ptr = E_three_dp.data();
      inc = 192 / size;
      size = 192;
    }
    else
    {
      series_ptr = E_two_dp.data();
      inc = 24 / size;
      size = 24;
    }

    cout << int(size) << endl;
    cout << int(inc) << endl;

    for(uint8_t i = 0; i != size; i += inc)
    {
      cout << series_ptr[i] << ", ";
    }

    float scale_factor = 1.0f;
    float temp_ratio = desired_ratio;
    while(temp_ratio < 0.1f)
    {
        scale_factor *= 10.0f;
        temp_ratio *= 10.0f;
    }

    float ratio, ratioDifference;

    //Add a value to matches, so that we can ignore the empty case in the for loop.
    ratio = series_ptr[0] / (series_ptr[0] * scale_factor + series_ptr[0]);
    ratioDifference = fabs(desired_ratio - ratio);
    mMatches.push_back(new ClosestMatch( series_ptr[0] * scale_factor, series_ptr[0], ratio, ratioDifference ));

    for( uint8_t i = 0; i != size; i += inc )
    {
        for( uint8_t j = 0; j != size; j += inc )
        {
            if( i == 0 && j == 0 )
                continue;

            ratio = series_ptr[j] / ( series_ptr[i] * scale_factor + series_ptr[j] );
            ratioDifference = fabs( desired_ratio - ratio );

            auto it = mMatches.begin(), end = mMatches.end();
            for( ; it != end; ++it)
            {
                if((*it)->RatioDifference <= ratioDifference)
                {
                    //Then the value at k is a closer match.
                    mMatches.insert( it, new ClosestMatch( series_ptr[i] * scale_factor, series_ptr[j], ratio, ratioDifference ) );
                    break;
                }
            }

            if(it == end)
            {
                //Then the value at k is a closer match.
                mMatches.insert( it, new ClosestMatch( series_ptr[i] * scale_factor, series_ptr[j], ratio, ratioDifference ) );
            }

            if( mMatches.size() > 10 )
                mMatches.pop_front();
        }
    }

    cout << "Use resistor values: Top: "
         << mMatches.back()->r1
         << " Bottom: "
         << mMatches.back()->r2;

    if(supply == 0.0f)
    {
      cout    << " for a ratio of: "
              << mMatches.back()->ratio << endl;
    }
    else
    {
      cout   << " for a voltage of: "
             << mMatches.back()->ratio*supply << endl;
    }
  }

    void ChangeSeries()
    {
      string str;

      bool good = false;
      while(good == false)
      {
        cout << endl << "Enter new Resistor Series: ";
        cin >> str;

        if(str.empty() == false)
        {
          str[0] = toupper(str[0]);

          auto it = series_map.find(str);
          if(it == series_map.end())
          {
            cout << "Invalid Series!";
          }
          else
          {
            cout << str << " series selected!" << endl;
            current_series = it->second;
            good = true;
          }
        }
        else
        {
          cout << "String in incorrect format!";
        }
      }

      //ClearMatches();
    }

    void PrintHelp()
    {
      cout << "Help" << endl
           << "-----------"
           << endl
           << "To use this tool, first select a resistor series, by entering the third\noption on the menu, then a resistor series, such as \"E24\". By default, the resistor series is set to E12."
           << endl << endl
           << "Then enter either a proportion of the supply voltage you'd like at the\ncentre of the potential divider, or enter a supply voltage\nand the voltage output from the potential divider."
           << endl << endl
           << "Simple as that. :)"
           << endl << endl;
    }
    void PrintMatches()
    {
      int num = mMatches.size();

      cout << setprecision(3) << endl;
      for(auto it = mMatches.begin(); it != mMatches.end(); ++it)
      {
        cout << setw(2) << num-- << ": Resistor Values: Top: "
             << (*it)->r1
             << " Bottom: "
             << (*it)->r2;

        if(supply == 0.0f)
        {
          cout    << " for a ratio of: "
                  << (*it)->ratio << endl;
        }
        else
        {
          cout   << " for a voltage of: "
                 << (*it)->ratio*supply << endl;
        }
      }
      cout << "Matches from the E" << int(3 << current_series) << " series." << endl;
    }


    void UseInput(char input_char)
    {
      string input;

      switch( input_char )
      {
      case '1':
        cout << "Proportion of Supply: ";
        cin >> input;
        stringstream(input) >> desired_ratio;
        supply = 0.0f;
        CalculateResistors();
        break;
      case '2':
        cout << "Supply Voltage: ";
        cin >> input;
        stringstream(input) >> supply;
        cout << "Desired Voltage: ";
        cin >> input;
        stringstream(input) >> desired_ratio;
        desired_ratio /= supply;
        CalculateResistors();
        break;
      case '3':
        ChangeSeries();
        break;
      case '4':
        PrintMatches();
        break;
      case '5':
        PrintHelp();
        break;
      case '6':
        done = true;
        break;
      default:
        cout << "Invalid Entry!";
        break;
      };
    }
  }

  void Init()
  {
    CLS();
    cout << ">>> ";
    cout << "EE Helper : Version " << VERSION_NUMBER << " by Ben Ockmore, " << __DATE__   << endl;
  }

  void Run()
  {
    char input_char;

    while(done == false)
    {
      DisplayMenu();

      cin >> input_char;
      cout << endl;

      CLS();
      cout << ">>> ";
      UseInput(input_char);

    }
  }

  void Destroy()
  {

  }
}

#ifdef NEVER

void Application::PrintMatches()
{
    int num = mMatches.size();

    for(auto it = mMatches.begin(); it != mMatches.end(); ++it)
    {
        cout << "Match " << num-- << ": " << endl;
        cout << "Resistor Values: Top: "
         << (*it)->r1
         << " Bottom: "
         << (*it)->r2;

         if(mSupplyVoltage == 0.0f)
         {
            cout    << " for a ratio of: "
                    << (*it)->Ratio << endl;
         }
         else
         {
             cout   << " for a voltage of: "
                    << (*it)->Ratio*mSupplyVoltage << endl;
         }
    }
    cout << "Matches from the E" << uint8_t(3 << current_series) << " series." << endl;
}



#endif
