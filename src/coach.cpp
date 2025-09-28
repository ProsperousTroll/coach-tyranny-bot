#include "coach.hpp"
#include <dpp/commandhandler.h>
#include <dpp/dispatcher.h>
#include <string>

// ------- helpers for commands ------- //

std::string Coach::bot::guard(){
   // too lazy to use weights sorry
   std::mt19937 rng(std::random_device{}());
   std::uniform_int_distribution<int> hundred(1, 100);
   int chance{hundred(rng)};
   std::cout << chance << '\n';
   if(chance >= 99){
      return "https://images-ext-1.discordapp.net/external/oAx1rkBsCpJNNldTVSx5J_tKdzmqnVtlPihCpl2Zd0s/https/cdn.imgchest.com/files/739cx8qqon7.png?format=webp&quality=lossless&width=1416&height=796";
   }
   return "https://images-ext-1.discordapp.net/external/HE_-a1xuAxUQXGueecDOzQsG14N3EL5QD3te4FmkH00/https/cdn.imgchest.com/files/4jdcvmaarx4.png?format=webp&quality=lossless&width=1416&height=796";
}

std::string Coach::bot::flip(){
   std::mt19937 rng(std::random_device{}());
   std::uniform_int_distribution<int> fiftyFifty(0, 1);
   int chance{fiftyFifty(rng)};
   std::cout << chance << '\n';
   if(chance){
      //log("Coach was flipped! user was... pitiful.");
      return "Boy, that's pitiful. Just pitiful.";
   }
   //log("Coach was flipped! user was... tough enough!");
   return "Oh, you know that it's tough. Enough!";
}

// --------- coach helpers ---------- //

std::string Coach::bot::token(){
   std::ifstream dotenv("../.env");
   std::string out;
   if(!(dotenv >> out)){
      log("Error! No token file found...");
   } else if (out == "") {
      log("Error! Invalid token...");
   }
   return out;
}

// implement simple call / response commands
void Coach::bot::simpleReply(std::string const& name, std::string const& description, std::string (*reply)()){
   cmdHandler.add_command(
      name,
      //parameters. Simple replies don't need any.
      {},

      [this, reply](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
         cmdHandler.reply(dpp::message(reply()), src);
      }
   );
}

// -------------- events / handling -------------- //

void Coach::bot::onMessageCreate(){
   // this method is evil i didn't program this i don't know how it got here what the fuck
   coach.on_message_create([this](dpp::message_create_t const& event){
      if((event.msg.content.find("im sorry") != std::string::npos || event.msg.content.find("i'm sorry") != std::string::npos)){
        event.reply("Don't be sorry, boy. Just do some neck calisthenics and we'll call it even.");
        std::string user{event.msg.author.username};
        log(user + " just apologized... Big mistake!");
      }

      if(event.msg.content.find("neck") != std::string::npos){
         dpp::snowflake user{event.msg.author.id};
         coach.direct_message_create(user, dpp::message("Hey. Can we talk?"));
      }     
   });
}

void Coach::bot::onReady(){
   cmdHandler.add_prefix("coach! ");
   coach.on_ready([this](dpp::ready_t const& event){
      // commands go here
      simpleReply("guard", "Guard the bank!", guardP);
      simpleReply("flip", "Is it tough, enough? Or just pitiful? Just ask your ol' coach.", flipP);
   });
}

// ---------- BOT ENTRY --------- //

void Coach::bot::run(){
   coach.on_log(dpp::utility::cout_logger());

   onReady();
   onMessageCreate();

   coach.start(dpp::st_wait);
}
