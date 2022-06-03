#include "thread.h"
#include "system.h"
#include "main_class.h"

__USING_API 

__BEGIN_API

int main(void)
{
    System::init(&Main::run);
    return 0;
}

__END_API