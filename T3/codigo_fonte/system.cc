#include "traits.h"
#include "debug.h"
#include "system.h"
#include "thread.cc"
__BEGIN_API

void System::init(void (*main)(void *)){
    db<System>(TRC) << "System::init() chamado\n"; // Traits notifcando o inicio do sistema
    setvbuf (stdout, 0, _IONBF, 0);
    db<Thread>(TRC << "Thread::init() chamado \n";
    Thread::init(main)
 }


__END_API