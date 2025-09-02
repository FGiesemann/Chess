/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/square.h"

namespace chesscore {

auto File::name() const -> char {
    return static_cast<char>(file + 'a' - 1);
}

auto operator<(const File &lhs, const File &rhs) -> bool {
    return lhs.file < rhs.file;
}

auto operator<(const Rank &lhs, const Rank &rhs) -> bool {
    return lhs.rank < rhs.rank;
}

// NOLINTBEGIN(readability-identifier-length,modernize-use-designated-initializers)
const Square Square::A1{File{'a'}, Rank{1}};
const Square Square::A2{File{'a'}, Rank{2}};
const Square Square::A3{File{'a'}, Rank{3}};
const Square Square::A4{File{'a'}, Rank{4}};
const Square Square::A5{File{'a'}, Rank{5}};
const Square Square::A6{File{'a'}, Rank{6}};
const Square Square::A7{File{'a'}, Rank{7}};
const Square Square::A8{File{'a'}, Rank{8}};

const Square Square::B1{File{'b'}, Rank{1}};
const Square Square::B2{File{'b'}, Rank{2}};
const Square Square::B3{File{'b'}, Rank{3}};
const Square Square::B4{File{'b'}, Rank{4}};
const Square Square::B5{File{'b'}, Rank{5}};
const Square Square::B6{File{'b'}, Rank{6}};
const Square Square::B7{File{'b'}, Rank{7}};
const Square Square::B8{File{'b'}, Rank{8}};

const Square Square::C1{File{'c'}, Rank{1}};
const Square Square::C2{File{'c'}, Rank{2}};
const Square Square::C3{File{'c'}, Rank{3}};
const Square Square::C4{File{'c'}, Rank{4}};
const Square Square::C5{File{'c'}, Rank{5}};
const Square Square::C6{File{'c'}, Rank{6}};
const Square Square::C7{File{'c'}, Rank{7}};
const Square Square::C8{File{'c'}, Rank{8}};

const Square Square::D1{File{'d'}, Rank{1}};
const Square Square::D2{File{'d'}, Rank{2}};
const Square Square::D3{File{'d'}, Rank{3}};
const Square Square::D4{File{'d'}, Rank{4}};
const Square Square::D5{File{'d'}, Rank{5}};
const Square Square::D6{File{'d'}, Rank{6}};
const Square Square::D7{File{'d'}, Rank{7}};
const Square Square::D8{File{'d'}, Rank{8}};

const Square Square::E1{File{'e'}, Rank{1}};
const Square Square::E2{File{'e'}, Rank{2}};
const Square Square::E3{File{'e'}, Rank{3}};
const Square Square::E4{File{'e'}, Rank{4}};
const Square Square::E5{File{'e'}, Rank{5}};
const Square Square::E6{File{'e'}, Rank{6}};
const Square Square::E7{File{'e'}, Rank{7}};
const Square Square::E8{File{'e'}, Rank{8}};

const Square Square::F1{File{'f'}, Rank{1}};
const Square Square::F2{File{'f'}, Rank{2}};
const Square Square::F3{File{'f'}, Rank{3}};
const Square Square::F4{File{'f'}, Rank{4}};
const Square Square::F5{File{'f'}, Rank{5}};
const Square Square::F6{File{'f'}, Rank{6}};
const Square Square::F7{File{'f'}, Rank{7}};
const Square Square::F8{File{'f'}, Rank{8}};

const Square Square::G1{File{'g'}, Rank{1}};
const Square Square::G2{File{'g'}, Rank{2}};
const Square Square::G3{File{'g'}, Rank{3}};
const Square Square::G4{File{'g'}, Rank{4}};
const Square Square::G5{File{'g'}, Rank{5}};
const Square Square::G6{File{'g'}, Rank{6}};
const Square Square::G7{File{'g'}, Rank{7}};
const Square Square::G8{File{'g'}, Rank{8}};

const Square Square::H1{File{'h'}, Rank{1}};
const Square Square::H2{File{'h'}, Rank{2}};
const Square Square::H3{File{'h'}, Rank{3}};
const Square Square::H4{File{'h'}, Rank{4}};
const Square Square::H5{File{'h'}, Rank{5}};
const Square Square::H6{File{'h'}, Rank{6}};
const Square Square::H7{File{'h'}, Rank{7}};
const Square Square::H8{File{'h'}, Rank{8}};
// NOLINTEND(readability-identifier-length,modernize-use-designated-initializers)

auto operator+(const Square &square, int squares) -> Square {
    Square result{square};
    result += squares;
    return result;
}

auto operator-(const Square &square, int squares) -> Square {
    Square result{square};
    result -= squares;
    return result;
}

auto to_string(const Square &square) -> std::string {
    return std::string{square.file().name()} + std::to_string(square.rank().rank);
}

} // namespace chesscore
