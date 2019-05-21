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

#ifndef NAMELTI_PROCESSOR_H_
#define NAMELTI_PROCESSOR_H_

#include <map>
#include <vector>
#include <string>

#include "namelti_transcripter.h"

namespace namelti {

class NameltiProcessor {
public:
  NameltiProcessor();
  virtual ~NameltiProcessor();
  std::vector<std::pair<std::string, float>> ConvertName(std::string, size_t);
  std::map<std::string, std::vector<std::pair<std::string, float>>> ConvertNameList(std::vector<std::string>, size_t);
private:
  NameltiTranscripter transcripter;
};

} // namespace namelti

#endif // NAMELTI_PROCESSOR_H_
