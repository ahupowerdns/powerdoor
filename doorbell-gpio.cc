#include <signal.h>
#include <syslog.h>
#include <stdio.h>
#include <wiringPi.h>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
 
using namespace std;
 
int main(int argc, char** argv)
{
  if(argc != 2) {
    cerr<<"Wrong number or arguments "<<argc<<endl;
    cerr<<"Syntax: doorbell-gpio gpio-number"<<endl;
    exit(EXIT_FAILURE);
  }
  
  signal(SIGPIPE, SIG_IGN);
  if (wiringPiSetup () == -1) {
    cerr<<"Error setting up GPIO"<<endl;
    return 1 ;
  }
 
  const int gpio = atoi(argv[1]);
  
  pinMode (gpio, OUTPUT) ;         

  openlog("doorbell-gpio", LOG_NDELAY, LOG_AUTH);
  string line;
  if(getline(cin, line)) {
    boost::trim(line);
    if(line=="open") {
      syslog(LOG_WARNING, "Opening the door on gpio %d", gpio);
      digitalWrite (gpio, 1) ;       // On
      delay (2000) ;                 // mS
      digitalWrite (gpio, 0) ;       // Off
      delay(100);
    }
    else {
      syslog(LOG_WARNING, "Wrong command '%s'", line.c_str());    
      cout<<"Wrong command"<<endl;
    }
  }
  return 0 ;
}
