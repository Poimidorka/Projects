#include "Executor.h"

const int32_t FILENAME_END = 256;
const int32_t ONE_MORE_FILE = 257;
const int32_t ARCHIVE_END = 258;

BigCode Executor::GetCode(int32_t x) {
    BigCode ans(block_size_);
    for (size_t i = 0; i < block_size_; ++i) {
        ans[i] = ((x >> i) & 1);
    }
    return ans;
}

size_t Executor::GetDigit(const BigCode& code) {
    int32_t digit = 0;
    for (size_t i = 0; i < code.size(); ++i) {
        digit += code[i] * (1 << i);
    }
    return digit;
}

void FillTable(std::unordered_map<int32_t, size_t>& cnt, std::string& filename) {
    std::ifstream in;
    in.open(filename);
    if (!in) {
        throw std::runtime_error("Something wrong with input file.\n"
                                 "Might it's damaged or something is interrupting my filestream ;(");
    }
    for (char c : filename) {
        ++cnt[static_cast<unsigned char>(c)];
    }
    char c;
    in.read(&c, 1);
    while (in.gcount() == 1) {
        ++cnt[static_cast<unsigned char>(c)];
        in.read(&c, 1);
    }
    in.close();
    ++cnt[FILENAME_END];
    ++cnt[ONE_MORE_FILE];
    ++cnt[ARCHIVE_END];
}

void Executor::WriteAlphabet(std::unordered_map<int32_t, size_t>& cnt,
                             std::unordered_map<int32_t, BigCode>& code_by_digit, std::string& filename) {
    int32_t symbols_count = cnt.size();
    current_stream_.Put(GetCode(symbols_count));
    CellList cells;
    TieArchiver huffman;
    for (auto el : cnt) {
        cells.push_back(Cell(el.first, el.second));
    }
    huffman.CodesByCnt(cells);
    std::unordered_map<size_t, size_t> count_len;
    size_t mx = 0;
    for (auto& el : cells) {
        mx = std::max(mx, el.len);
        ++count_len[el.len];
        code_by_digit[el.c] = el.code;
        current_stream_.Put(GetCode(el.c));
    }
    for (size_t i = 1; i <= mx; i++) {
        current_stream_.Put(GetCode(count_len[i]));
    }
    for (size_t i = 0; i < filename.size(); ++i) {
        current_stream_.Put(code_by_digit[filename[i]]);
    }
    current_stream_.Put(code_by_digit[FILENAME_END]);
}

void Executor::ArchiveFile(std::string filename, bool last) {
    std::unordered_map<int32_t, BigCode> code_by_digit;
    std::unordered_map<int32_t, size_t> cnt;
    FillTable(cnt, filename);
    WriteAlphabet(cnt, code_by_digit, filename);
    std::ifstream in;
    char c;
    in.open(filename);
    in.read(&c, 1);
    while (in.gcount() == 1) {
        current_stream_.Put(code_by_digit[static_cast<unsigned char>(c)]);
        in.read(&c, 1);
    }
    in.close();
    if (!last) {
        current_stream_.Put(code_by_digit[ONE_MORE_FILE]);
    }
    else {
        current_stream_.Put(code_by_digit[ARCHIVE_END]);
    }
}

void Executor::ReadAlphabet(size_t symbol_count, CellList& cells) {
    for (size_t i = 0; i < symbol_count; ++i) {
        cells[i].c = GetDigit(current_stream_.Get(9));
    }
    int32_t cur = symbol_count;
    size_t ptr = 0;
    size_t current_len = 0;
    while (cur) {
        ++current_len;
        int32_t x = GetDigit(current_stream_.Get(9));
        cur -= x;
        for (size_t i = ptr; i < ptr + x; ++i) {
            cells[i].len = current_len;
        }
        ptr += x;
    }
}

void Executor::ReadFileName(std::string& filename, TieArchiver& huffman, std::map<BigCode, int32_t>& digit_by_code) {
    BigCode current;
    while(true) {
        int32_t c = GetDigit(current_stream_.Get(1));
        current.push_back(c);
        if (huffman.GetNext(c)) {
            if (digit_by_code[current] == FILENAME_END) {
                break;
            }
            filename += static_cast<unsigned char>(digit_by_code[current]);
            current.clear();
        }
    }
}

bool Executor::CreateOriginalFile(std::ofstream& out, TieArchiver& huffman, std::map<BigCode, int32_t>& digit_by_code) {
    BigCode current;
    while (true) {
        int32_t c = GetDigit(current_stream_.Get(1));
        current.push_back(c);
        if (huffman.GetNext(c)) {
            if (digit_by_code[current] == ARCHIVE_END || digit_by_code[current] == ONE_MORE_FILE) {
                break;
            }
            current_stream_.CheckOut(&out);
            out.put(static_cast<unsigned char>(digit_by_code.at(current)));
            current.clear();
        }
    }
    return digit_by_code[current] == ONE_MORE_FILE;
}

bool Executor::DisArchiveFile(std::vector<std::string>& file_names) {
    file_names.push_back("");
    std::map<BigCode, int32_t> digit_by_code;
    std::unordered_map<int32_t, BigCode> code_by_digit;
    auto p = current_stream_.Get(9);
    size_t symbol_count = GetDigit(p);
    CellList cells(symbol_count);
    ReadAlphabet(symbol_count, cells);
    TieArchiver huffman;
    huffman.GetCanon(cells);
    for (auto& el : cells) {
        digit_by_code[el.code] = el.c;
        code_by_digit[el.c] = el.code;
    }
    std::string filename;
    huffman.Build(cells);
    ReadFileName(filename, huffman, digit_by_code);
    std::ofstream out;
    out.open(filename);
    file_names.back() = filename;
    bool ans = CreateOriginalFile(out, huffman, digit_by_code);
    out.close();
    return ans;
}

void Executor::Archive(int argc, std::vector<std::string>& args) {
    std::string archive_name = args[2];
    std::ofstream writing;
    writing.open(archive_name);
    current_stream_ = StreamClass(&writing);
    try {
        for (int i = 3; i < argc; ++i) {
            ArchiveFile(args[i], (i == argc - 1));
        }
    } catch(const std::runtime_error& e) {
        std::cout << "File error: " << e.what() << std::endl;
        writing.close();
        std::remove(archive_name.c_str());
        exit(1);
    } catch (...) {
        std::cout << "Undefined Error" << std::endl;
        exit(1);
    }
    current_stream_.Put({});
    writing.close();
}

void Executor::DisArchive(int argc, std::vector<std::string>& args) {
    if (argc > 3) {
        throw std::logic_error("Too many arguments for -d\n");
    }
    std::string archive_name = args[2];
    std::ifstream reading;
    reading.open(archive_name);
    current_stream_ = StreamClass(&reading);
    std::vector<std::string> file_names;
    try {
        while (DisArchiveFile(file_names)) {}
    } catch (const std::runtime_error& e) {
        std::cout << "File error: " << e.what() << std::endl;
        for (auto& name : file_names) {
            if (!name.empty()) {
                std::remove(name.c_str());
            }
        }
        exit(1);
    } catch(...) {
        std::cout << "Unknown Error\n";
        exit(1);
    }
    reading.close();
}

void Executor::Parse(int argc, char** argv) {
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; ++i) {
        args[i] = std::string(argv[i]);
    }
    if (argc == 1) {
        throw std::logic_error("Needs arguments\n");
    }
    if (args[1] == "-h") {
        std::cout << "This is Huffman archiver\n"
                     "Use -h for help\n"
                     "Use -c archive_name file1 file2 file3 ... to create archive from files\n"
                     "Use -d archive_name to restore files from archive" << std::endl;
        if (argc > 2) {
            throw std::logic_error("Too many arguments for -h\n");
        }
        return;
    }
    if (argc == 2) {
        throw("Too few arguments for -d or -c\n");
    }
    if (args[1] == "-c") {
        Archive(argc, args);
        return;
    }
    if (args[1] == "-d") {
        DisArchive(argc, args);
    }
}