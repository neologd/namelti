// Copyright (C) 2018-2019 Toshinori Sato (@overlast)
//
//       https://github.com/neologd/namelti
//
// Licensed under the Apache License, Version 2.0 (the &quot;License&quot;);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an &quot;AS IS&quot; BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include "namelti_rest_server.h"

using namespace Pistache;

namespace  namelti {

RESTServer::RESTServer(Pistache::Address addr, std::string serverpath)
    : nServerPath(serverpath),
      nHttpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {
}

RESTServer::~RESTServer() {
  std::cout << "Shutting down server" << std::endl;
  shutdown();
}

void RESTServer::init(size_t threads) {
  auto options = Pistache::Http::Endpoint::options()
                 .threads(threads)
                 .flags(Pistache::Tcp::Options::InstallSignalHandler);
  nHttpEndpoint->init(options);
  setupRoutes();
}


void RESTServer::start() {
  nHttpEndpoint->setHandler(router.handler());
  nHttpEndpoint->serve();
}

void RESTServer::shutdown() {
  nHttpEndpoint->shutdown();
}


void RESTServer::setupRoutes() {
  Pistache::Rest::Routes::Post(router, "/api/v1/sentence:convertNameList", Pistache::Rest::Routes::bind(&RESTServer::convertNameListHandler, this));
  Pistache::Rest::Routes::Post(router, "/api/v1/sentence:convertName", Pistache::Rest::Routes::bind(&RESTServer::convertNameHandler, this));
  Pistache::Rest::Routes::Get(router, "/monitor/l7check", Pistache::Rest::Routes::bind(&RESTServer::l7HealthCheckHandler, this));
  Pistache::Rest::Routes::Get(router, "/", Pistache::Rest::Routes::bind(&RESTServer::helloHandler, this));
}


void RESTServer::helloHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  std::string res = request.resource();
  nlohmann::json response_json;
  response_json["response"] = "Hello. I'm Namelti.";
  response.send(Http::Code::Ok, response_json.dump());
  response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}

void RESTServer::l7HealthCheckHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  std::string res = request.resource();
  nlohmann::json response_json;
  response_json["response"] = "OK";
  response.send(Http::Code::Ok, response_json.dump());
  response.headers().add<Http::Header::ContentType>(MIME(Text, Plain));
}

void RESTServer::convertNameListHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  std::string res = request.resource();
  std::string json_str = request.body();
  nlohmann::json request_json = json.parse(json_str);
  std::vector<std::string> queries = request_json["query"];

  std::map<std::string, std::vector<std::pair<std::string, float>>> dict;
  dict = namelti.ConvertNameList(queries);
  nlohmann::json response_json(nlohmann::json::value_t::object);
  if(!dict.empty()){
    for (auto dit = dict.begin(); dit != dict.end(); ++dit) {
      std::string surface = dit->first;
      std::cout << surface << std::endl;
      std::vector<std::pair<std::string, float>> results = dit->second;
      nlohmann::json entry_json;
      for (std::pair<std::string, float>& result: results) {
        std::string yomigana = result.first;
        float score = result.second;
        nlohmann::json item_json;
        item_json["yomigana"] = yomigana;
        item_json["score"] = score;
        entry_json.push_back(item_json);
      }
      response_json[surface] = entry_json;
    }
  }
  response.send(Http::Code::Ok, response_json.dump());
  response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}

void RESTServer::convertNameHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  std::string res = request.resource();
  std::string json_str = request.body();
  nlohmann::json request_json = json.parse(json_str);
  std::string query = request_json["query"];
  std::cout << query << std::endl;
  std::vector<std::pair<std::string, float>> results = namelti.ConvertName(query);

  nlohmann::json response_json(nlohmann::json::value_t::array);
  for (std::pair<std::string, float>& result: results) {
    std::string yomigana = result.first;
    float score = result.second;
    nlohmann::json item_json;
    item_json["yomigana"] = yomigana;
    item_json["score"] = score;
    response_json.push_back(item_json);
  }

  response.send(Http::Code::Ok, response_json.dump());
  response.headers().add<Http::Header::ContentType>(MIME(Application, Json));

}

} // End namespace namelti

int main(int argc, char *argv[]) {

  std::string serverpath = argv[0];
  Pistache::Port port(50964);
  int threads = 2;
  // First arg is port, second is number of threads to use.
  if (argc >= 2) {
    port = Pistache::Port(std::stol(argv[1]));
    if (argc == 3) {
      threads = std::stol(argv[2]);
    }
  }

  std::cout << "Server home " << serverpath << std::endl;
  std::cout << "Cores = " << hardware_concurrency() << std::endl;
  std::cout << "Using " << threads << " threads" << std::endl;
  std::cout << "Port: " << port  << std::endl;

  Pistache::Address addr(Pistache::Ipv4::any(), port);
  namelti::RESTServer server(addr, serverpath);
  server.init(threads);
  server.start();

}
