#include "crow.h"
#include <string>
#include <boost/asio.hpp>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using boost::asio::ip::tcp;
namespace baip=boost::asio::ip;

int main(int argc, char**argv)
{
    if(argc!=5) {  //   0        1         2                3            4
      cerr<<"Syntax: doorbell local-port actuator-address actuator-port code"<<endl;
      exit(EXIT_FAILURE);
    }
    int localport=atoi(argv[1]);
    const char* actuatoraddress=argv[2];
    int actuatorport = atoi(argv[3]);
    string code = argv[4];

    baip::tcp::endpoint endpoint( baip::address::from_string(actuatoraddress), actuatorport);
    
    crow::SimpleApp app;
    boost::asio::io_service io_service;
        
    signal(SIGPIPE, SIG_IGN);
    
    CROW_ROUTE(app, "/")
    .methods("POST"_method)
    ([&io_service,&endpoint,&code](const crow::request& req) -> std::string {
        std::cout<<"code: "<<req.body<<std::endl;
        if(req.body == code) {        
          try {
            tcp::socket socket(io_service);
            socket.connect(endpoint);
            string message("open\r\n");
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            boost::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (error != boost::asio::error::eof)
              return "some error";                        
            return "Welcome!";
          }
          catch(std::exception& e) {
            return string("Error: ")+e.what();
          }
        }
        else {
          sleep(2);
          return "Wrong code!";
        }
    });

    app.port(localport).bindaddr("127.0.0.1").run();
}
