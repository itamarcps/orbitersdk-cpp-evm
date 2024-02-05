#include "../../src/libs/catch2/catch_amalgamated.hpp"
#include "../../src/utils/dynamicexception.h"
#include <iostream>

namespace TDynamicException {
    TEST_CASE("DynamicException Class", "[utils][dynamicexception]") {
        SECTION("Exception message is set and retrieved correctly") {
            const std::string filename = "test.cpp";
            const int line = 42;
            const std::string function = "testFunction";
            const std::string message = "Test message";
            DynamicException exception("Function " + function + " failed: " + message + " at " + filename + ":", line);

            try {
                throw exception;
            } catch (const DynamicException& e) {
                REQUIRE(e.what() == "Function " + function + " failed: " + message + " at " + filename + ":" + std::to_string(line));
                REQUIRE(e.getFile() == "");
                REQUIRE(e.getLine() == 0);
                REQUIRE(e.getFunction() == "");
            }
        }

        SECTION("Exception with file, line, and function information") {
            const std::string filename = "test.cpp";
            const int line = 42;
            const std::string function = "testFunction";
            const std::string message = "Error in file " + filename + " at line " + std::to_string(line) + " in function " + function;
            DynamicException exception(message, filename, line, function);

            try {
                throw exception;
            } catch (const DynamicException& e) {
                REQUIRE(e.what() == message);
                REQUIRE(e.getFile() == filename);
                REQUIRE(e.getLine() == line);
                REQUIRE(e.getFunction() == function);
            }
        }


        SECTION("Timestamp is correctly formatted") {
            const std::string message = "Error with timestamp";
            DynamicException exception(message);

            try {
                throw exception;
            } catch (const DynamicException& e) {
                std::string timestamp = e.getTimestamp();
                REQUIRE(!timestamp.empty());
                REQUIRE(timestamp.length() == 19); // Checking the length of the timestamp
                REQUIRE(timestamp[4] == '-');
                REQUIRE(timestamp[7] == '-');
                REQUIRE(timestamp[10] == ' ');
                REQUIRE(timestamp[13] == ':');
                REQUIRE(timestamp[16] == ':');
            }
        }

        SECTION("Exception with single message") {
            const std::string message = "Error with single string message";
            DynamicException exception(message);

            try {
                throw exception;
            } catch (const DynamicException& e) {
                REQUIRE(e.what() == message);
                REQUIRE(e.getFile() == "");
                REQUIRE(e.getLine() == 0);
                REQUIRE(e.getFunction() == "");
            }
        }

        SECTION("Exception with multiple messages") {
            int a = 5;
            int b = 10;
            const std::string message = "Error with multiple messages: " + std::to_string(a) + " and " + std::to_string(b);
            DynamicException exception("Error with multiple messages: ", a, " and ", b);

            try {
                throw exception;
            } catch (const DynamicException& e) {
                REQUIRE(e.what() == message);
                REQUIRE(e.getFile() == "");
                REQUIRE(e.getLine() == 0);
                REQUIRE(e.getFunction() == "");
            }
        }

    }
}
