#include "include/coach.hpp"

// ------- helpers for commands ------- //

std::mt19937 rng(std::random_device{}());
// different chances
std::uniform_int_distribution<int> hundred(1, 100);
std::uniform_int_distribution<int> fiftyFifty(0, 1);

std::string guard(){
   // too lazy to use weights sorry
   int chance{hundred(rng)};
   std::cout << chance << '\n';
   if(chance >= 99){
      return "https://images-ext-1.discordapp.net/external/oAx1rkBsCpJNNldTVSx5J_tKdzmqnVtlPihCpl2Zd0s/https/cdn.imgchest.com/files/739cx8qqon7.png?format=webp&quality=lossless&width=1416&height=796";
   }
   return "https://images-ext-1.discordapp.net/external/HE_-a1xuAxUQXGueecDOzQsG14N3EL5QD3te4FmkH00/https/cdn.imgchest.com/files/4jdcvmaarx4.png?format=webp&quality=lossless&width=1416&height=796";
}

std::string flip(){
   int chance{fiftyFifty(rng)};
   std::cout << chance << '\n';
   if(chance){
      Coach::log("Coach was flipped! user was... pitiful.");
      return "Boy, that's pitiful. Just pitiful.";
   }
   Coach::log("Coach was flipped! user was... tough enough!");
   return "Oh, you know that it's tough. Enough!";
}

// pointers to functions so they are called at runtime and not initialization.
std::string (*guard_p)(){guard};
std::string (*flip_p)(){flip};

// --------- coach helpers ---------- //

bool Coach::checkToken(std::string& TOKEN){
   std::ifstream dotenv("../.env");
   if(dotenv >> TOKEN){
      log(".env found... Starting coach");
      return true;
   }
   log("ERROR! No token found!");
   return false;
}

// implement simple call / response commands
void simpleReply(dpp::commandhandler& handler, std::string const& name, std::string const& description, std::string (*reply)()){
   handler.add_command(
      //command name
      name,
      // parameters
      {}, 
   
      [&handler, reply](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
         handler.reply(dpp::message(reply()), src);
      },
   
      description
   );
}

// -------------- coach bot ------------ //

void Coach::run(){
   std::string TOKEN;
   if(checkToken(TOKEN)){

      std::cout << "Coach is running!\n";
      // bot stuff here
      dpp::cluster coach(TOKEN, dpp::i_all_intents); // note to self: DO NOT DO THIS.

      coach.on_log(dpp::utility::cout_logger());

      // --------------------- prefix commands because FUCK YOU --------------- //
      
      dpp::commandhandler commandHandler(&coach);
      commandHandler.add_prefix("coach! ");

      coach.on_ready([&commandHandler](const dpp::ready_t& event){

         simpleReply(commandHandler, "guard", "Guard the bank!", guard_p);
         simpleReply(commandHandler, "flip", "Is it tough, enough? Or just pitiful? Just ask your ol' coach.", flip_p);
         
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

   } else { log("Shutting down due to error..."); }
}
