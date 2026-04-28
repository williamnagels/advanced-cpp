#include <format>
#include <string_view>
#include <type_traits>

namespace {
    // Scale Tags with Metadata
    struct Celsius    { static constexpr std::string_view suffix = "°C"; };
    struct Fahrenheit { static constexpr std::string_view suffix = "°F"; };
    struct Kelvin     { static constexpr std::string_view suffix = "K";  };

    template <typename Scale>
    struct Temperature : public Scale {
        double value;
        explicit constexpr Temperature(double v) : value(v) {}

        // TODO: Implement a helper that returns the value in Celsius regardless of the current Scale.
        constexpr double toCelsius() const {
            return 0.0; 
        }

        // TODO: Implement a conversion bridge that returns a NEW Temperature object of TargetScale.
        // Hint: Use toCelsius() as an intermediate step.
        template <typename TargetScale>
        constexpr auto as() const {
            return *this; // Placeholder
        }
    };
} 

/**
 * GOAL: Specialize std::formatter to handle unit-aware formatting.
 * parse formatter of {C}, {F}, and {K} flags. 
 * look-up the meaning of std::format_parse_context and how it can do this.
 * Ensure the format() method performs the requested conversion.
 */
template <typename Scale>
struct std::formatter<Temperature<Scale>> {
    // TODO: Define a member variable to store the requested presentation scale.

    constexpr auto parse(std::format_parse_context& ctx) {
        // TODO: Parse the format specifier. If 'C', 'F', or 'K' is found, 
        // store it. Throw std::format_error for invalid characters.
        return ctx.begin();
    }

    auto format(const Temperature<Scale>& t, std::format_context& ctx) const {
        // TODO: Based on the stored presentation scale:
        // 1. Convert 't' to the target scale using t.template as<TargetScale>().
        // 2. Use std::format_to to write the value and the suffix to ctx.out().
        return ctx.out();
    }
};

namespace {
/*
GOAL: Demonstrate a Strong Type that integrates with std::format.
The type should handle internal conversions automatically when a 
format specifier is provided.
*/
void test_1() {
    Temperature<Celsius> roomTemp{25.0};

    // TODO: Uncomment these asserts once the Temperature class and 
    // std::formatter specialization are implemented.

    /*
    auto s1 = std::format("Standard: {}", roomTemp);
    assert(s1 == "Standard: 25°C");

    auto s2 = std::format("In Fahrenheit: {:F}", roomTemp);
    assert(s2 == "In Fahrenheit: 77°F");

    auto s3 = std::format("In Kelvin: {:K}", roomTemp);
    assert(s3 == "In Kelvin: 298.15K");
    */
}
}

void type_safe_ex4() {
    test_1();
}