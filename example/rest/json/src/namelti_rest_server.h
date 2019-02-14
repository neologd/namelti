#ifndef NAMELTI_REST_SERVER_H
#define NAMELTI_REST_SERVER_H

#include <string>
#include <memory>
#include <json.hpp>

#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "pistache/http_headers.h"

#include "namelti_processor.h"

namespace namelti {
  class RESTServer {

 public:

    RESTServer(Pistache::Address addr, std::string serverpath);
    virtual ~RESTServer();
    void init(size_t threads = 2);
    void start();
    void shutdown();

 private:

    void helloHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void convertNameListHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void convertNameHandler(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void setupRoutes();

    const std::string nServerPath;
    namelti::NameltiProcessor namelti;
    nlohmann::json json;
    std::shared_ptr<Pistache::Http::Endpoint> nHttpEndpoint;
    Pistache::Rest::Router router;


  };
}

#endif // NAMELTI_REST_SERVER_H
