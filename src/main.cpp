#include "dpp/dpp.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

std::string guardTheBank(){
   // too lazy to use weights sorry
   int chance{rand() % 100};
   if(chance >= 99){
      return "https://images-ext-1.discordapp.net/external/oAx1rkBsCpJNNldTVSx5J_tKdzmqnVtlPihCpl2Zd0s/https/cdn.imgchest.com/files/739cx8qqon7.png?format=webp&quality=lossless&width=1416&height=796";
   }
   return "https://images-ext-1.discordapp.net/external/HE_-a1xuAxUQXGueecDOzQsG14N3EL5QD3te4FmkH00/https/cdn.imgchest.com/files/4jdcvmaarx4.png?format=webp&quality=lossless&width=1416&height=796";
}

std::string goodOrBad(){
   int chance{rand() % 2};
   if(chance >= 1){
      return "Boy, that's pitiful. Just pitiful.";
   }
   return "Oh, you know that it's tough. Enough!";
}

std::ifstream dotenv("../.env");
std::string TOKEN;

int main(){
   if( dotenv >> TOKEN ){
      std::cout << "Coach is running!\n";
      // bot stuff here
      dpp::cluster coach(TOKEN, dpp::i_all_intents);

      coach.on_log(dpp::utility::cout_logger());

      // --------------------- prefix commands because FUCK YOU --------------- //
      
      dpp::commandhandler commandHandler(&coach);
      commandHandler.add_prefix("coach! ");

      coach.on_ready([&commandHandler](const dpp::ready_t& event){
         commandHandler.add_command(
            //command name
            "guard",
            // parameters
            {}, 

            [&commandHandler](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
               commandHandler.reply(dpp::message(guardTheBank()), src);
            },

            "Guard the bank!"
         );

         commandHandler.add_command(
            //command name
            "flip",
            // parameters
            {}, 

            // handler
            [&commandHandler](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
               commandHandler.reply(dpp::message(goodOrBad()), src);
            },

            // description 
            "Is it tough, enough? Or just pitiful? Just ask you ol' coach."
         );

         commandHandler.slash_commands_enabled = true;
         commandHandler.register_commands();
      });



      
      /* SLASH COMMANDS FOR NORMIEs
       
      coach.on_slashcommand([](const dpp::slashcommand_t& event) {
         if(event.command.get_command_name() == "ping") {
            event.reply("Pong!");
         }
      });

      coach.on_slashcommand([](const dpp::slashcommand_t& event) {
         if(event.command.get_command_name() == "guard") {
            event.reply(guardTheBank());
         }
      });      

      // ---------- commands list ----------- //
      coach.on_ready([&coach](const dpp::ready_t& event){
         if(dpp::run_once<struct register_bot_commands>()){
            coach.global_command_create(dpp::slashcommand("ping", "Ping pong!", coach.me.id));
            coach.global_command_create(dpp::slashcommand("guard", "Guard the bank!", coach.me.id));
         } 
      });

      */

      coach.start(dpp::st_wait);

   } else {
      std::cout << "ERROR: No token!\n";
      return 1;
   }
   //
   return 0;
}
