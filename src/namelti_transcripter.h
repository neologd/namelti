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

#ifndef NAMELTI_TRANSCRIPTER_H_
#define NAMELTI_TRANSCRIPTER_H_

#include <string>
#include <vector>
#include <map>
#include <mecab.h>

namespace namelti {

class NameltiTranscripter {
public:
  NameltiTranscripter();
  virtual ~NameltiTranscripter();

  struct Tagger {
    MeCab::Tagger *mecab;
  };

  Tagger* GetTagger();
  std::map<std::string, std::vector<std::string>> GetTranscript(std::vector<std::string>&);

private:
  NameltiTranscripter::Tagger *tagger;

};


} // namespace namelti

#endif // NAMELTI_TRANSCRIPTER_H_
