#pragma once
#include "dpp/dpp.h"
#include "mpg123.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <cstdlib>
#include <fstream>
#include <random>
#include <iostream>
#include <ctime>

namespace Coach {
   void log(std::string const& input);

   class bot {
   // all functions of the bot in a singleton class, for scalability and organization.
      public:
         // singleton boiler
         static bot* getInstance();//{if(instance == nullptr){instance = new bot();} return instance;}
                                   //
         // ------ members ------- //
         dpp::cluster coach{token(), dpp::i_all_intents};
         dpp::commandhandler cmdHandler{&coach};

         // ------ methods ------- //


         // helpers
         //bool checkToken(std::string& TOKEN);
         std::string token();
         void log(std::string const& input){std::cout << input << '\n';}

         // commands & command logic
         void simpleReply(std::string const& name, std::string const& description, std::string (*reply)());
         static std::string flip();
         static std::string guard();
         static void mp3Player();
         static void calisthenics();
         std::string (*flipP)(){flip};
         std::string (*guardP)(){guard}; // todo: think of something smarter

         // events & handling
         void onReady();
         void onMessageCreate(); // what?

         // ------ entry ----- //
         void run();

      private:
         static bot* instance;
         bot(){}
         ~bot(){delete instance;}
   };
}
