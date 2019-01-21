#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2018-2019 Toshinori Sato (@overlast)
#
#       https://github.com/neologd/namelti
#
# Licensed under the Apache License, Version 2.0 (the &quot;License&quot;);
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an &quot;AS IS&quot; BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import namelti_pb2
import namelti_pb2_grpc

import sys
import grpc

_TIMEOUT_SECONDS = 2

def run(server_address, queries):
  channel = grpc.insecure_channel(server_address)
  namelti = namelti_pb2_grpc.NameltiServiceStub(channel)
  responses = namelti.ConvertNameList(namelti_pb2.NameltiTranscriptRequests(queries=queries), _TIMEOUT_SECONDS).responses

  for query in queries:
      print (query)
      response = responses[query]
      for result in response.results:
          yomi = result.yomi
          score = result.score
          print (yomi + " : " + str(score))

  print ("----------")

  for query in queries:
      print (query)
      response = namelti.ConvertName(namelti_pb2.NameltiTranscriptRequest(query=query), _TIMEOUT_SECONDS)
      print ('query=%s' % response.surface)
      for result in response.results.results:
          yomi = result.yomi
          score = result.score
          print (yomi + " : " + str(score))

if __name__ == '__main__':
  if len(sys.argv) > 1:
      queries = sys.argv[1:]
  else:
      queries = ["佐藤敏紀", "徳川家康", "平清盛"]

  run('localhost:50194', queries)
