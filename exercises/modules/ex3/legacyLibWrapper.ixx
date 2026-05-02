module; 
#include "legacyLib.h"
// TODO: Export a module Converter that wraps the legacy library functions and constants

// TODO: Create a namespace Conv that encapsulates the legacy library’s functions and constants.
// In main.cpp, replace direct usage with a function applyPower, which internally delegates to legacy_power.
// Expose legacy_power within the Conv namespace so it can still be invoked directly for validation/testing purposes.
// Provide a templated function getMagicValue(multiplier) that returns MAGIC_NUMBER * multiplier, with the return type deduced from the multiplier (e.g., int, double).
