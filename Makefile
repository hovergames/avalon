BOOST_DIR=vendors/boost
BOOST_LIB_DIR=$(BOOST_DIR)/stage/lib

tests: build-tests darwin-copy-dylib
	@cd tests && ./runner

clean:
	@rm -f tests/runner
	@rm -f tests/libboost_unit_test_framework.dylib
	@cd $(BOOST_DIR) && [ -x b2 ] && ./b2 --clean > /dev/null

darwin-copy-dylib: build-boost
	@if [ "Darwin" == "$$(uname)" ]; then \
		cp $(BOOST_LIB_DIR)/libboost_unit_test_framework.dylib tests/; \
	fi

build-tests: build-boost
	@g++ tests/runner.cpp \
		-I . \
		-L$(BOOST_LIB_DIR) \
		-lboost_unit_test_framework \
		-o tests/runner

build-boost:
	@if [ ! -f $(BOOST_LIB_DIR)/libboost_unit_test_framework.a ]; then \
		echo "!! trigged compilation of boost UTF library"; \
		cd $(BOOST_DIR) && \
		./bootstrap.sh --with-libraries=test > /dev/null && \
		./b2 > /dev/null; \
	fi

.PHONY: tests clean darwin-copy-dylib build-tests build-boost
