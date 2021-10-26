#pragma once

#include "tie_archiver.h"
#include <iostream>
#include "stream_class.h"
#include <unordered_map>
#include <map>

class Executor {
public:
    void Parse(int argc, char** args);
    Executor() {};
private:
    const size_t block_size_ = 9;
    StreamClass current_stream_;
    bool DisArchiveFile(std::vector<std::string>& file_names);
    bool CreateOriginalFile(std::ofstream& out, TieArchiver& huffman, std::map<BigCode, int32_t>& digit_by_code);

    void WriteAlphabet(std::unordered_map<int32_t, size_t>& cnt, std::unordered_map<int32_t, BigCode>& code_by_digit, std::string& filename);
    void ArchiveFile(std::string filename, bool last);
    void ReadAlphabet(size_t symbol_count, CellList& cells);
    void ReadFileName(std::string& file_name, TieArchiver& huffman, std::map<BigCode, int32_t>& digit_by_code);
    void DisArchive(int argc, std::vector<std::string>& args);
    void Archive(int argc, std::vector<std::string>& args);
    BigCode GetCode(int32_t x);
    size_t GetDigit(const BigCode& code);
};