#include "include/coach.hpp"

int main(){
   Coach::bot* inst{Coach::bot::getInstance()};
   inst->run();
   Coach::log("Don't mess... With the chest! (Signing off)");
   //
   return 0;
}
