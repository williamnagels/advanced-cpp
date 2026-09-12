#pragma once

#include <cstdint>
#include <span>

std::uint64_t raw_pipeline(std::span<const int> values, int minimum);
std::uint64_t ranges_pipeline(std::span<const int> values, int minimum);