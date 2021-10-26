#include "Executor.h"

int main(int argc, char* argv[]) {
    try {
        Executor exec;
        exec.Parse(argc, argv);
    } catch (const std::logic_error& el) {
        std::cout << "Argument error: " << el.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Undefined error " << std::endl;
        return 1;
    }
    std::cout << "Finished correctly" << std::endl;
    return 0;
}