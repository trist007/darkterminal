#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <string>
#include <chrono>

#include <unitedgas/Well.h>
#include <trantor/utils/crypto/sha256.h>

using namespace drogon;
using namespace std::chrono_literals;


int main() {
  // Ask Drogon to listen on 127.0.0.1 port 8848. Drogon supports listening
  // on multiple IP addresses by adding multiple listeners. For example, if
  // you want the server also listen on 127.0.0.1 port 5555. Just add another
  // line of addListener("127.0.0.1", 5555)
  LOG_INFO << "Server running on 127.0.0.1:8848";
  app()
      .enableSession(86400)
      .addListener("127.0.0.1", 8848)
      .run();
}
