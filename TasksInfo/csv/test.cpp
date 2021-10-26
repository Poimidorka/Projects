#include <catch.hpp>

#include "csv_parser.h"

#include <sstream>

TEST_CASE("CsvParser") {
    std::stringstream input_stream(
        "name,city,zip\n"
        "John,Riverside,8075\n"
        ",SomeTown,\n"
        "\"Joan \"\"the bone\"\", Anne\",Desert City,123");

    CsvParser<std::string, std::string, int> csv_parser(input_stream, ',', 1);

    std::stringstream output_stream;

    for (const auto& row : csv_parser) {
        output_stream << row << '\n';
    }

    const std::string expected =
        "John, Riverside, 8075\n"
        ", SomeTown, 0\n"
        "Joan \"the bone\", Anne, Desert City, 123\n";

    REQUIRE(expected == output_stream.str());
}

TEST_CASE("CsvParserError") {
    try {
        std::stringstream input_stream(
            "name,city,zip\n"
            "John,Riverside,ERROR");

        CsvParser<std::string, std::string, int> csv_parser(input_stream, ',', 1);

        std::stringstream output_stream;

        for (const auto& row : csv_parser) {
            output_stream << row << '\n';
        }
    } catch (const CsvParserError&) {
        return;
    }

    FAIL("Expected CsvParserError");
}
