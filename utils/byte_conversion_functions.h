#ifndef BYTE_CONVERSION_FUNCTIONS_H
#define BYTE_CONVERSION_FUNCTIONS_H

#include <cstdint>
#include <vector>

namespace byte_utils
{
enum class Endianness
{
    little,
    big
};

std::uint32_t to32BitInt(
    const std::vector<std::uint8_t>& byteArray, Endianness endianness);
std::vector<std::uint8_t> toByteArray(
    std::uint32_t int32Bit, Endianness endianness);
}

#endif// BYTE_CONVERSION_FUNCTIONS_H
