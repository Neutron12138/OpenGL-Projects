#include <iostream>

#include "no_dll.hpp"

int main()
{
    NoDLL app;
    app.run();
    return app.get_exit_code();
}
