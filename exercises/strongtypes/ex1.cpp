#include <type_traits>
#include <sstream>
#include <cassert>
namespace
{
// Define the Command struct
struct Command
{
    //TODO: create a universal constructor which takes one argument 'input' that is only part of overload resolution
    //when 'input' can be used to construct an std::string. Throw a readable compiler error in 
    //all other cases. Initialize m_command with the value of 'input'. call setMayPrintArguments in the body of this constructor.

    // Override the default printing behaviour.
    //TODO: when printArguments is false, look for the start of the first argument (first occurance of a space ' ') and 
    //update the end iterator class member 'm_endIterator'.
    void setMayPrintArguments(bool printArguments) 
    {
    }
    
    std::string::const_iterator begin() const 
    {
        return m_command.begin();
    }
    
    std::string::const_iterator end() const 
    {
        return m_endIterator;
    }
    
    //TODO: Implement this function so that it writes the command 'm_command' to 
    //the outputstream os. Respect m_endIterator set by setMayPrintArguments.
    void printCommand(std::ostream& os) const 
    {
    }
private:
    std::string m_command;
    std::string::const_iterator m_endIterator;
    // Friend function to enable ostream overload
    friend std::ostream& operator<<(std::ostream& os, const Command& cmd);
};

// ostream command overload definition, No changes needed
std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
    cmd.printCommand(os);
    return os;
}

// GOAL: The goal of this exercise is to demonstrate how wrapping an std::string
// in a custom command class can help protect sensitive data.
// Passing a boolean like `bSensitive` alongside a string is risky because the 
// relationship between the two is not enforced by the type system. 
// Another developer might not realize that the boolean and the string are intended 
// to be used together, leading to potential misuse or security vulnerabilities.
void test_1()
{
    //TODO uncomment the code when the command class has been implemented.
    //Command command("run --option value --secret williamIsAwesome");
    //std::stringstream ss;
    //ss << command;
    // assert(ss.str() == "run");
    //ss.clear();
    //command.setMayPrintArguments(true);
    //ss << command;
    // assert(ss.str() == "run --option value --secret williamIsAwesome");
}
}
void type_safe_ex1()
{
  test_1();
}
