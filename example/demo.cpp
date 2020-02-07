#include <tpl/tpltasks.hpp>
#include <ppltasks.h>
#include <iostream>
#include <string>
#include <memory>

tpl::task<std::string> write_to_string()
{
    // Create a shared pointer to a string that is  
    // assigned to and read by multiple tasks. 
    // By using a shared pointer, the string outlives 
    // the tasks, which can run in the background after 
    // this function exits.
    auto s = std::make_shared<std::string>("Value 1");

    return tpl::create_task([s]
        {
            // Print the current value.
            std::cout << "Current value: " << *s << std::endl;
            // Assign to a new value.
            *s = "Value 2";
            return *s;
        });
    /*
    .then([s]
        {
            // Print the current value.
            std::cout << "Current value: " << *s << std::endl;
            // Assign to a new value and return the string.
            *s = "Value 3";
            return *s;
        });
    */
}

int main()
{
    // Create a chain of tasks that work with a string.
    auto t = write_to_string();

    // Wait for the tasks to finish and print the result.
    std::cout << "Final value: " << t.get() << std::endl;
}

/* Output:
    Current value: Value 1
    Current value: Value 2
    Final value: Value 3
*/