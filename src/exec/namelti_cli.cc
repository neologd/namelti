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
#include <string>
#include <vector>
#include <stdexcept>

#include "namelti_processor.h"

int main(int argc, char* argv[]) {
    try {
        namelti::NameltiProcessor namelti;
        while (true) {
            std::string query;
            std::getline(std::cin, query);
            if(std::cin.eof() || query == "exit" || query == "quit" || query == "bye"){
                break;
            }
            else if(query.empty()){
                continue;
            }
            size_t maxResults = 10;
            std::vector<std::pair<std::string, float>> results = namelti.ConvertName(query, maxResults);
            if(!results.empty()){
              //std::cout << it->first << " => " << "\n";
              for (std::pair<std::string, float>& result : results) {
                std::cout << result.first << "(" << result.second << "), ";
              }
              std::cout << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        exit(1);
    }
    (void)argc;
    (void)argv;
    return 0;
}
