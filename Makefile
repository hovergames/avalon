BOOST_DIR := vendors/boost
BOOST_LIB_DIR := $(BOOST_DIR)/stage/lib
BOOST_LIB_UTF := $(BOOST_LIB_DIR)/libboost_unit_test_framework.a
CXX ?= g++

install:
	git submodule update --init --recursive

test: build-tests
	@./tests/runner

clean:
	rm -f tests/runner
	cd $(BOOST_DIR) && [ -x b2 ] && ./b2 --clean > /dev/null

build-tests: build-boost
	@$(CXX) -std=c++0x tests/runner.cpp -I . $(BOOST_LIB_UTF) -o tests/runner

build-boost:
	@if [ ! -f "$(BOOST_LIB_UTF)" ]; then \
		echo "!! compilation of boost UTF library triggered"; \
		cd $(BOOST_DIR) && ./bootstrap.sh --with-libraries=test > /dev/null && ./b2 > /dev/null; \
	fi

.PHONY: install test clean build-tests build-boost
