/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "chesscore/bitboard.h"

auto display_board(const chesscore::Bitmap &bitmap) -> void {
    std::cout << "  a b c d e f g h\n";
    for (int rank = chesscore::Rank::max_rank; rank >= chesscore::Rank::min_rank; --rank) {
        std::cout << rank << ' ';
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            const chesscore::Square square{file, rank};
            if (bitmap.get(square)) {
                std::cout << 'X';
            } else {
                std::cout << '.';
            }
            std::cout << ' ';
        }
        std::cout << rank << '\n';
    }
    std::cout << "  a b c d e f g h\n";
}

auto as_hex(const chesscore::Bitmap &bitmap) -> std::string {
    std::stringstream oss;
    static constexpr int byte_count = 8;

    for (int i = 0; i < byte_count; ++i) {
        const auto byte = (bitmap.bits() >> (56 - i * 8)) & 0xff;
        if (i > 0) {
            oss << '\'';
        }
        oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << byte;
    }

    return oss.str();
}

auto as_bits(const chesscore::Bitmap &bitmap) -> std::string {
    std::stringstream oss;

    static constexpr int square_count = 64;

    for (int i = 0; i < square_count; ++i) {
        if (i > 0 && i % chesscore::File::max_file == 0) {
            oss << '\'';
        }
        oss << ((bitmap.bits() >> (square_count - 1 - i)) & 1);
    }

    return oss.str();
}

auto main() -> int {
    chesscore::Bitmap bitmap{};

    bool running = true;
    while (running) {
        display_board(bitmap);
        std::cout << "\nBitmap: 0x" << as_hex(bitmap) << '\n';
        std::cout << "      : 0b" << as_bits(bitmap) << '\n';
        std::cout << "          HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA HGFEDCBA\n";

        std::cout << "\nInput: ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "h" || input == "H") {
            std::cout << "Commands:\n"
                      << "  <square> - Toggle the square (e.g., e4)\n"
                      << "  0x<hex> - Set the bitmap from a hexadecimal string (e.g., 0x0000000000000000)\n"
                      << "  < <num>  - shift bits left by <num> (e.g., < 2)\n"
                      << "  > <num>  - shift bits right by <num> (e.g., > 2)\n"
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
        if (input.starts_with("<")) {
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
