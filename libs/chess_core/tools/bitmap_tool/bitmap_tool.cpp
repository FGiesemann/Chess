/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "chesscore/bitmap.h"
#include "chesscore_io/bitmap_io.h"

auto main() -> int {
    chesscore::Bitmap bitmap{};

    bool running = true;
    while (running) {
        std::cout << bitmap;
        std::cout << "\nBitmap: 0x" << as_grouped_hex(bitmap) << " (" << as_ull_hex(bitmap) << ")\n";
        std::cout << "      : 0b" << as_grouped_bits(bitmap) << '\n';
        std::cout << "          HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA\n";

        std::cout << "\nInput: ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "h" || input == "H") {
            std::cout << "Commands:\n"
                      << "  <square> - Toggle the square (e.g., e4)\n"
                      << "  0x<hex>  - Set the bitmap from a hexadecimal string (e.g., 0x0000000000000000)\n"
                      << "  =<num>   - Set the bitmap from a decimal value\n"
                      << "  <<num>   - shift bits left by <num> (e.g., < 2)\n"
                      << "  ><num>   - shift bits right by <num> (e.g., > 2)\n"
                      << "  c        - Clear the bitmap\n"
                      << "  q        - Quit the program\n"
                      << "  h        - Show this help message\n\n";
        } else if (input == "q" || input == "Q") {
            running = false;
        } else if (input == "c" || input == "C") {
            bitmap = chesscore::Bitmap{};
        } else if (input.starts_with("0x")) {
            std::string hex_input = input.substr(2);
            bitmap = chesscore::Bitmap{std::stoull(hex_input, nullptr, 16)};
        }
        if (input.starts_with("=")) {
            std::string dec_input = input.substr(1);
            bitmap = chesscore::Bitmap{std::stoull(dec_input, nullptr, 10)};
        } else if (input.starts_with("<")) {
            std::string num_str = input.substr(1);
            try {
                int num = std::stoi(num_str);
                bitmap <<= num;
            } catch (const std::invalid_argument &) {
                std::cout << "Invalid number for left shift.\n";
            }
        } else if (input.starts_with(">")) {
            std::string num_str = input.substr(1);
            try {
                int num = std::stoi(num_str);
                bitmap >>= num;
            } catch (const std::invalid_argument &) {
                std::cout << "Invalid number for right shift.\n";
            }
        } else {
            if (input.length() == 2 && input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8') {
                int file = input[0] - 'a' + 1;
                int rank = input[1] - '0';
                chesscore::Square square{file, rank};
                bitmap.toggle(square);
            } else {
                std::cout << "Invalid input. Enter a square (e.g., e4), 'c' to clear, or 'q' to quit.\n";
            }
        }
    }
}
