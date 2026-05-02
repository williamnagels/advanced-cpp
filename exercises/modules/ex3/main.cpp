#include <iostream>
import Converter;

int main() {
    std::cout << "Power: " << Conv::applyPower(5.0) << "\n";
    std::cout << "Power: " << Conv::legacy_power(5.0) << "\n";
    std::cout << "Magic: " << Conv::getMagicValue(2) << "\n";

    // CRITICAL TEST: 
    // Even though legacyLib.h is used inside the module, 
    // MAGIC_NUMBER should not be visible here. It is contained,
    // in the module. You should see the correct 'Success' message.
    #ifdef MAGIC_NUMBER
        std::cout << "Error: Macro Leaked!\n";
    #else
        std::cout << "Success: Macro Isolated.\n";
    #endif

    return 0;
}