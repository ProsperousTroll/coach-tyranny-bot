#include "coach.hpp"
#include <chrono>
#include <cstdint>
#include <ctime>
#include "mpg123.h"
#include <dpp/cache.h>
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>
#include <dpp/coro/async.h>
#include <dpp/discordclient.h>
#include <dpp/dispatcher.h>
#include <dpp/message.h>
#include <dpp/presence.h>
#include <dpp/timer.h>
#include <random>
#include <string>
#include <thread>
#include <vector>



// ----------- boiler ----------- //
//
// I made the decision to make coach a singleton class, as this would make scaling easier with more complex commands and event handling.
Coach::bot* Coach::bot::instance{nullptr};

Coach::bot* Coach::bot::getInstance(){
   if(instance == nullptr){
      instance = new Coach::bot();
   } return instance;
}

// ------- helpers ------- //

// TODO: Make a better logging system, or maybe find a good logging library.
void Coach::log(std::string const& input){
   std::time_t now{std::time(nullptr)};
   char timeFormat[100];
   std::strftime(timeFormat, sizeof(timeFormat), "%Y-%m-%d %X:%M:%S", std::localtime(&now));
   std::cout << "[" << timeFormat << "] " << input << '\n';
}

int Coach::randInt(int min, int max){
   std::mt19937 rng(std::random_device{}());
   std::uniform_int_distribution<int> range(min, max);
   return range(rng);
}

std::vector<uint8_t> Coach::getSound(std::string file){
	std::vector<uint8_t> pcmdata;

	mpg123_init();

	int err;
	mpg123_handle *mh = mpg123_new(NULL, &err);
	unsigned char* buffer;
	size_t buffer_size;
	size_t done;
	int channels, encoding;
	long rate;

	mpg123_param(mh, MPG123_FORCE_RATE, 48000, 48000.0);

	buffer_size = mpg123_outblock(mh);
	buffer = new unsigned char[buffer_size];

	mpg123_open(mh, file.c_str());
	mpg123_getformat(mh, &rate, &channels, &encoding);

	unsigned int counter = 0;
	for (int totalBtyes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
		for (auto i = 0; i < buffer_size; i++) {
			pcmdata.push_back(buffer[i]);
		}
		counter += buffer_size;
		totalBtyes += done;
	}
	delete buffer;
	mpg123_close(mh);
	mpg123_delete(mh);
	mpg123_exit();
	return pcmdata;
}

// ---------------- commands / command logic ----------------- //

// TODO: More robust randomization with weights.
std::string Coach::bot::guard(){
   // too lazy to use weights sorry
   int chance{Coach::randInt(1, 100)};
   if(chance >= 99){
      Coach::log("Coach is guarding the bank! ...naked.");
      return "https://cdn.imgchest.com/files/739cx8qqon7.png";
   }
   Coach::log("Coach is guarding the bank!");
   return "https://cdn.imgchest.com/files/4jdcvmaarx4.png";
}

std::string Coach::bot::flip(){
   int chance{Coach::randInt(0, 2)};
   if(chance == 1){
      Coach::log("Coach was flipped! user was... pitiful.");
      return "Boy, that's pitiful. Just pitiful.";
   }
   Coach::log("Coach was flipped! user was... tough enough!");
   return "Oh, you know that it's tough. Enough!";
}

std::string Coach::bot::question(std::string userId){

   int chance{randInt(0, 100)};
   std::vector<std::string> rareAnswers{
      //"You should probably kill yourself.",
      "You there, @" + userId + " , why aren't you doing your neck calisthenics?",
   };

   std::vector<std::string> answers{
      "I don't know about that, son.",
      "Pitiful. That's just pitiful.",
      "You sound like you're from Northern Soutwest Easter High. Stop talking.",
      "Sounds like you ain't tough. Enough.",
      "I oughta snap your thin lil' chicken neck for that.",
      "Not by the hair on my big, bulgin' chest.",
      "that's YOUR fault.",
      "Some are wise. You, are otherwise.",
      "I feel so bad for you right now, you can call me fluffy. I wouldn't even care.",
      "You know what son, that's so elite, it's e-lite!",
      "That's what I'm talkin about!",
      "That is frivolosly unneccessary.",
      "It's tough bein' tough.",
      "I'd rather tend to the grass on my football field.",
      "You know how I got this big, son? Neck calisthenics.",
      "Relax.",
      "You wanna be the best? Then don't mess. With the chest.",
      "Nice. Very nice.",
      "Who even says nook and cranny anymore? I don't even know what a cranny is.",
      "That's enough to bring a tear to your eye...",
      "You gotta have calves the size of cantalopes... or honeydew...",
      "You gotta have a chest so big, no one can get within 5 feet of ya!",
      "You need to have a personal relationship with Jesus Christ.",
      "You kids sure do know your bible stories.",
      "...Super.",
   };
   if(chance < 99){
      return rareAnswers[randInt(0, rareAnswers.size())];
   }
   return answers[randInt(0, answers.size())];
}

void Coach::bot::calisthenics(dpp::cluster& coach, dpp::snowflake guildId, std::string(*files)()){
   // TODO: implement calisthenics command   
   dpp::voiceconn* v{coach.get_shard(0)->get_voice(guildId)};
   if(!v || !v->voiceclient || !v->voiceclient->is_ready()){
      Coach::log("Not connected to voice in server");
      return;
   }

   std::ifstream pcmFile(files(), std::ios::binary);
   if (!pcmFile.is_open()){
      Coach::log("PCM file failed to open");
      return;
   }

   constexpr size_t samples_per_frame = 1920; // stereo 20ms frame
   constexpr size_t bytes_per_frame = samples_per_frame * sizeof(uint16_t); // 3840
   
   std::vector<uint16_t> buffer(samples_per_frame);
   
   while (pcmFile.read(reinterpret_cast<char*>(buffer.data()), bytes_per_frame)) {
       // len here is the number of samples, not bytes
       v->voiceclient->send_audio_raw(buffer.data(), samples_per_frame);
   
       std::this_thread::sleep_for(std::chrono::milliseconds(20));
   }
}

std::string Coach::bot::calmDown(std::string user){
   std::string reply[4]{
      "Shut up " + user + ", I'll kill you!",
      "Shut up " + user + ", I'm gonna kill you!",
      "Not this time, " + user + "...",
      "Too late " + user + ", I'm killing you slow!"
   };

   return reply[randInt(0, 3)];
}


// --------- coach helpers ---------- //

std::string Coach::bot::token(){
   std::ifstream dotenv("../.env");
   std::string out;
   if(!(dotenv >> out)){
      Coach::log("Error! No token file found...");
   } else if (out == "") {
      Coach::log("Error! Invalid token...");
   }
   Coach::log("Loaded dotenv... initializing...");
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
         Coach::log("command " + command + " was ran!");
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
        Coach::log(user + " just apologized... Big mistake!");
      }

      if(event.msg.content.find("neck") != std::string::npos){
         dpp::snowflake user{event.msg.author.id};
         coach.direct_message_create(user, dpp::message("Hey. Can we talk?"));
         coach.direct_message_create(user, dpp::message("I noticed you were talking about your neck just now. And I was wonderin' if maybe you'd show me? Maybe do a calisthenic for your ol' coach? Please?"));
         coach.direct_message_create(user, dpp::message("Hello?"));
      }     

      if(event.msg.content.find("calm down coach") != std::string::npos){
         std::string user{event.msg.author.global_name};
         event.reply(calmDown(user));
      }
   });
}

std::string testFiles(){
   return "../audio/OUTRO/DONTMESSWTC.pcm";
}

void Coach::bot::onReady(){
   coach.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, "the big game"));

   cmdHandler.add_prefix("coach! ");
   cmdHandler.add_prefix("Coach! ");
   coach.on_ready([this](dpp::ready_t const& event){
      // commands go here
      // TODO: Add help command
      simpleReply("guard", "Guard the bank!", guardP);
      //simpleReply("flip", "Is it tough, enough? Or just pitiful? Just ask your ol' coach.", flipP); simpleReply("question", "Got a question for the coach? Go ahead and ask.", questionP());
      cmdHandler.add_command(
            "question",
            {},
            [this, event](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
               cmdHandler.reply(dpp::message(questionP(src.issuer.id.str())), src);
            });
      // TODO: make this work. and look nice.
      cmdHandler.add_command(
         "calisthenics",
         //parameters
         {},
                                                                                                                      
         [this, event](std::string const& command, dpp::parameter_list_t const& parameters, dpp::command_source src){
            cmdHandler.reply(dpp::message("Don't worry, I'll be home soon to run you chicken-necks through some calisthenics any day now!"), src);

            //dpp::voiceconn* v{event.from()->get_voice(src.guild_id)};
            /*
            dpp::voiceconn* v{this->coach.get_shard(0)->get_voice(src.guild_id)};
            if(v && v->voiceclient && v->voiceclient->is_ready()){
               cmdHandler.reply(dpp::message("Get ready, 'cuz it ain't gon' be easy!"), src);
               return;
            }

            calisthenics(this->coach, src.guild_id, testFiles);
            
            dpp::guild* g{dpp::find_guild(src.guild_id)};
            if(!g->connect_member_voice(src.issuer.id)){
               cmdHandler.reply(dpp::message("You ain't in a voice call!"), src);
               return;
            }           

            Coach::log("Ready for calisthenics!");
            */
         }
      );
   });
}

// ---------- BOT ENTRY --------- //

void Coach::bot::run(){
   coach.on_log(dpp::utility::cout_logger());

   onReady();
   onMessageCreate();

   coach.start(dpp::st_wait);
}
