#pragma once
#include "dpp/dpp.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <cstdlib>
#include <fstream>
#include <random>
#include <iostream>

namespace Coach {
   class bot {
   // all functions of the bot in a singleton class, for scalability and organization.
      public:
         // ------ members ------- //
         dpp::cluster coach{token(), dpp::i_all_intents};
         dpp::commandhandler cmdHandler{&coach};

         // ------ methods ------- //
         // singleton boiler
         bot(){getInstance();}
         ~bot(){delete instance;}

         // helpers
         //bool checkToken(std::string& TOKEN);
         std::string token();
         void log(std::string const& input){std::cout << input << '\n';}

         // commands & command logic
         void simpleReply(std::string const& name, std::string const& description, std::string (*reply)());
         static std::string flip();
         static std::string guard();
         std::string (*flipP)(){flip};
         std::string (*guardP)(){guard}; // todo: think of something smarter

         // events & handling
         void onReady();
         void onMessageCreate(); // what?

         // ------ entry ----- //
         void run();

      private:
         bot* instance;
         bot* getInstance(){if(!instance){instance = new bot();} return instance;}
   };
}
