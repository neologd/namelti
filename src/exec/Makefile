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

BIN_DEST = /usr/local/bin
BINS = namelti_cli

all: $(BINS)

CXX := g++
CXXFLAGS := -Wall -Wextra -O3 -std=c++11 -isystem../../include -I..
CXXLIBS := -lnamelti

debug: CXXFLAGS += -DDEBUG -g
debug: all

SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SRCS))
DEPS = $(patsubst %.cc,%.d,$(SRCS))

NAMELTI_CLI_OBJS = namelti_cli.o

namelti_cli: $(NAMELTI_CLI_OBJS)
	$(CXX) -o $@ $(NAMELTI_CLI_OBJS) $(CXXLIBS)

install:
	install -p $(BINS) $(BIN_DEST)

uninstall:
	cd $(BIN_DEST) && $(RM) $(BINS)

clean:
	$(RM) $(OBJS) $(DEPS) $(BINS)

.PHONY: clean all

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

%.d: %.cc
	@set -e; $(CXX) -MM $(CXXFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@
-include $(DEPS)
