#include "include/coach.hpp"

int main(){
   Coach::bot* inst{Coach::bot::getInstance()};
   inst->run();
   //
   return 0;
}
