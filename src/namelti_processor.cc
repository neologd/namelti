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
#include <map>
#include <vector>
#include <string>
#include <stdexcept>

#include "namelti_processor.h"
#include "namelti_transcripter.h"

namespace namelti {
NameltiProcessor::NameltiProcessor(): transcripter(NameltiTranscripter()) {

}
NameltiProcessor::~NameltiProcessor() {
}

std::map<std::string, std::vector<std::string>> NameltiProcessor::ConvertNameList(std::vector<std::string> queries) {
  std::map<std::string, std::vector<std::string>> processor_result;

  try{
    std::map<std::string, std::vector<std::string>> transctipter_result = transcripter.GetTranscript(queries);
    if (!transctipter_result.empty()) {
      processor_result = transctipter_result;
    }
  }
  catch(const std::exception& e){
    std::cerr << "error: " << e.what() << std::endl;
    exit(1);
  }

  return processor_result;
}

} // namespace namelti
