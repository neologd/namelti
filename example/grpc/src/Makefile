# Copyright (C) 2018 Toshinori Sato (@overlast)
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

BIN = namelti_server

all: $(BIN)

CXX := g++
CXXFLAGS := -Wall -Wextra -O3 -std=c++11 `pkg-config --cflags grpc++ grpc` -isystem../include -I../../../src -isystem../../../include
CXXLIBS := `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -lpthread -ldl -lnamelti

SUBDIRS = grpc

SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SRCS))
DEPS = $(patsubst %.cc,%.d,$(SRCS))

NAMELTI_GRPC_SRCS = $(wildcard grpc/*.cc)
NAMELTI_GRPC_OBJS = $(patsubst %.cc,%.o,$(NAMELTI_GRPC_SRCS))

_grpc:
	$(MAKE) -C grpc

namelti_server: namelti_server.o _grpc
	$(CXX) $(CXXLIBS) -o $@ namelti_server.o $(NAMELTI_GRPC_OBJS)

clean: $(SUBDIRS)
	$(MAKE) -C grpc clean
	$(RM) $(OBJS) $(DEPS) $(BIN)

.PHONY: clean all $(SUBDIRS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

%.d: %.cc
	@set -e; $(CXX) -MM $(CXXFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@
-include $(DEPS)
