// Copyright (C) 2018 Toshinori Sato (@overlast)
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
        std::vector<std::string> queries;
        std::map<std::string, std::vector<std::string>> dict;
        while (true) {
            std::string raw_input;
            std::getline(std::cin, raw_input);
            if(std::cin.eof() || raw_input == "exit" || raw_input == "quit" || raw_input == "bye"){
                break;
            }
            else if(raw_input.empty()){
                continue;
            }

            queries.push_back(raw_input);

            dict = namelti.ConvertNameList(queries);
            if(!dict.empty()){
              for (auto it = dict.begin(); it != dict.end(); ++it) {
                //std::cout << it->first << " => " << "\n";
                std::vector<std::string> results = it->second;
                for (std::string& result: results) {
                  std::cout << result << ", ";
                }
                std::cout << "\n";
              }
            }
            queries.clear();
            dict.clear();
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
