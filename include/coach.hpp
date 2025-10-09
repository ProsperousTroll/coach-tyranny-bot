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
         void playSound(std::string const& file);

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

// hiding these away because it's ugly
namespace Audio{
   inline std::vector<std::string> intro {
      "../audio/MP3/INTRO/CALISTHENICS.mp3",
      "../audio/MP3/INTRO/FA.mp3",
      "../audio/MP3/INTRO/LOOKSLIKE.mp3",
      "../audio/MP3/INTRO/PAN.mp3",
   };

   inline std::vector<std::string> directions{
      "../audio/MP3/DIR/RIGHT.mp3",
      "../audio/MP3/DIR/LEFT.mp3",
      "../audio/MP3/DIR/UP.mp3",
   };
   inline std::vector<std::string> faster{
      "../audio/MP3/FASTER/RIGHT.mp3",
      "../audio/MP3/FASTER/LEFT.mp3",
      "../audio/MP3/FASTER/UP.mp3",
   };

   inline std::vector<std::string> outro{
      "../audio/MP3/OUTRO/DOWN.mp3",
      "../audio/MP3/OUTRO/RELAX.mp3",
      "../audio/MP3/OUTRO/DAISY.mp3",
      "../audio/MP3/OUTRO/DONTMESSWTC.mp3",
      "../audio/MP3/OUTRO/OXBOWSONG.mp3",
      "../audio/MP3/OUTRO/PITIFUL.mp3",
      "../audio/MP3/OUTRO/THATSBEAUTY.mp3",
      "../audio/MP3/OUTRO/SUCCSONG.mp3",
      "../audio/MP3/OUTRO/THATSMORELIKEIT.mp3",
      "../audio/MP3/OUTRO/THATSRIGHTBABY.mp3",
      "../audio/MP3/OUTRO/UNFORGIVEN.mp3",
   };

   // TODO: Add misc voice lines
   inline std::vector<std::string> misc {};
}
