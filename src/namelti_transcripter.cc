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

#include "namelti_transcripter.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <stdexcept>

#include <mecab.h>

namespace namelti {

NameltiTranscripter::NameltiTranscripter(): tagger(GetTagger()) {
}
NameltiTranscripter::~NameltiTranscripter() {
}

NameltiTranscripter::Tagger* NameltiTranscripter::GetTagger() {
  Tagger *t = new Tagger;
  std::string _mecab_options = "-d /usr/lib64/mecab/dic/mecab-ipadic-neologd";
  t->mecab = MeCab::createTagger(_mecab_options.c_str());
  return t;
}

bool has_key_using_find(std::map<std::string, std::vector<std::pair<std::string, float> > > &m, std::string s){
  if (m.find(s) == m.end()){
    return false;
  } else {
    return true;
  }
}

std::map<std::string, std::vector<std::pair<std::string, float> > > NameltiTranscripter::GetTranscript(std::vector<std::string> &queries) {
  std::map<std::string, std::vector<std::pair<std::string, float> > > transcripter_result;
  size_t nbest_num = 5;
  try {
    for (std::string& query : queries) {

      if (has_key_using_find(transcripter_result, query)) {
        continue;
      }

      const char *start = tagger->mecab->parseNBest(nbest_num, query.c_str());

      //bool is_general_name = false;
      //bool is_first_family_name = false;
      std::map<std::string, int> yomigana_cache;
      std::vector<std::pair<std::string, float> > result_vec;

      std::string yomigana_buf = "";
      //bool is_first_name = false;
      //bool is_famiry_name = false;
      for(const char* end = start; *end != '\0'; ++end){
        if(*end == '\n'){
          if(start < end){
            const char *line_start = start;
            bool has_tab = false;
            for(const char* line_end = line_start; *line_end != '\n'; ++line_end){
              if(*line_end == '\t'){
                has_tab = true;
                if(line_start < line_end){
                  //std::cout << std::string(line_start, line_end) << "\n"; // surface
                  line_start = line_end + 1;
                  //std::cout << std::string(line_start, end) << "\n"; // features
                  const char *feature_start = line_start;
                  std::vector<std::string> features;
                  for(const char* feature_end = line_start; *feature_end != '\n'; feature_end++){
                    if(*feature_end == ','){
                      if(feature_start < feature_end){
                        //std::cout << std::string(feature_start, feature_end) << "\n"; // each feature
                        features.push_back(std::string(feature_start, feature_end));
                      }
                      feature_start = feature_end + 1;
                    }
                  }
                  //std::cout << std::string(feature_start, end) << "\n";
                  features.push_back(std::string(feature_start, end));

                  //std::cout << features.size() << "\n";
                  if (features.size() == 9) {
                    yomigana_buf += features[7];
                  } else {
                    yomigana_buf += features[0];
                  }
                }
              }
            }
            if (!has_tab) {
              if (yomigana_buf != "") {
                decltype(yomigana_cache)::iterator finder = yomigana_cache.find(yomigana_buf);
                if (finder == yomigana_cache.end()) {
                  float score = 1.0;
                  std::pair<std::string, float> res = std::make_pair(yomigana_buf, score);
                  result_vec.push_back(res);
                  yomigana_cache.insert(std::make_pair(yomigana_buf, 1));
                }
                yomigana_buf = "";
              }
            }
            start = end + 1;
          }
        }
      }
      if (!result_vec.empty()) {
        transcripter_result[query] = result_vec;
      }
    }
  }
  catch(const std::exception& e){
    std::cerr << "error: " << e.what() << std::endl;
    exit(1);
  }


  return transcripter_result;
}

} // namespace namelti
