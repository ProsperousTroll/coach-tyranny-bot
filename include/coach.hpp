#pragma once
#include "dpp/dpp.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/discordclient.h>
#include <dpp/discordvoiceclient.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <cstdlib>
#include <dpp/snowflake.h>
#include <fstream>
#include <future>
#include <random>
#include <iostream>
#include <ctime>
#include <vector>

namespace Coach {
   void log(std::string const& input);
   int randInt(int min, int max);

   class bot {
   // all functions of the bot in a singleton class, for scalability and organization.
      public:
         // singleton boiler
         static bot* getInstance();
         // ------ entry ----- //
         void run();

      private:
         // ------------ boiler ---------- //
         static bot* instance;
         bot(){}
         ~bot(){delete instance;}

         // ------ members ------- //
         dpp::cluster coach{token(), dpp::i_all_intents};
         dpp::commandhandler cmdHandler{&coach};
         dpp::voiceconn* voiceClient;

         // ------ methods ------- //

         // helpers
         std::string token();
         void log(std::string const& input){std::cout << input << '\n';}
         std::vector<uint8_t> getSound(std::string file);
         void playSound(dpp::voiceconn* v, std::string const& file);

         // commands & command logic
         void simpleReply(std::string const& name, std::string const& description, std::string (*reply)());
         static std::string flip();
         static std::string guard();
         static std::string question();
         static std::string calmDown(std::string user);
         std::string (*questionP)(){question};
         std::string (*flipP)(){flip};
         std::string (*guardP)(){guard}; // todo: think of something smarter

         // events & handling
         void onReady();
         void onVoiceReady();
         void onMessageCreate(); // what?
   };
}
