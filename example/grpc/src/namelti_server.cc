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

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "namelti_processor.h"
#include "namelti.grpc.pb.h"

using namespace namelti;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class NameltiServiceImpl: public server::NameltiService::Service
{
protected:
  const std::shared_ptr<NameltiProcessor> namelti;

public:
  NameltiServiceImpl(std::shared_ptr<NameltiProcessor>& namelti):
        server::NameltiService::Service(), namelti(namelti)
  {}


  Status ConvertNameList(ServerContext*, const server::NameltiTranscriptRequests* request, server::NameltiTranscriptResponses* response) override
  {
    namelti::NameltiProcessor namelti;
    std::vector<std::string> queries;
    std::map<std::string, std::vector<std::pair<std::string, float>>> dict;
    auto res = response->mutable_responses();
    for(const auto& q: request->queries()){
      queries.push_back(q);
    }
    dict = namelti.ConvertNameList(queries);
    if(!dict.empty()){
      for (auto dit = dict.begin(); dit != dict.end(); ++dit) {
        std::string surface = dit->first;
        std::vector<std::pair<std::string, float>> results = dit->second;
        server::ResultList res_list;
        size_t j = 0;
        for (std::pair<std::string, float>& result: results) {
          std::string yomigana = result.first;
          float score = result.second;
          res_list.add_results();
          auto* entry = res_list.mutable_results(j++);
          entry->set_yomi(yomigana);
          entry->set_score(score);
        }
        (*res)[surface] = res_list;
      }
    }
    return Status::OK;
  }

  Status ConvertName(ServerContext*, const server::NameltiTranscriptRequest* request, server::NameltiTranscriptResponse* response) override
  {
    namelti::NameltiProcessor namelti;
    std::string query = request->query();
    auto res_surface = response->mutable_surface();
    auto res_results = response->mutable_results();

    std::vector<std::pair<std::string, float>> results = namelti.ConvertName(query);

    server::ResultList res_list;
    size_t j = 0;
    for (std::pair<std::string, float>& result: results) {
      std::string yomigana = result.first;
      float score = result.second;
      res_list.add_results();
      auto* entry = res_list.mutable_results(j++);
      entry->set_yomi(yomigana);
      entry->set_score(score);
    }

    (*res_surface) = query;
    (*res_results) = res_list;

    return Status::OK;
  }
};

void RunServer(const std::string& server_address, std::shared_ptr<NameltiProcessor> namelti) {
    NameltiServiceImpl service(namelti);

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cerr << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    try{
        auto namelti = std::make_shared<NameltiProcessor>();
        std::string grpc_server_address = "localhost:50194";
        RunServer(grpc_server_address, namelti);
    }
    catch(const std::exception& e){
        std::cerr << "error: " << e.what() << std::endl;
        exit(1);
    }

    return 0;
}
