#pragma once

#include "dpp/dpp.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace Coach {

   bool checkToken(std::string& TOKEN);

   // entry point
   void run();

   // Helpers
   inline void log(std::string const& input){std::cout << input << '\n';}
}

