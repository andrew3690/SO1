#include "thread.h"
#include <iostream>

__BEGIN_API
Thread* Thread::_running = nullptr;

// Retrun id
void Thread::Thread(){
    Thread::init()
}
int Thread::id(){
    return(_id); // implmentação sem erros
}

void Thread::init(void (*main)(void *)){
    ThreadMain = new Thread(run, (char *) main_name.data()); // Criação da ThreadMain, Provável uso do construtor de Threads
    ThreadDispacher = new Thread::dispatcher() // ThreadDispacher mesma coisa que a ThreadMain
    CPU::switch_context(prev ->context(),next -> ThreadMain());

}

// Finalização de thread
void Thread::thread_exit(int exit_code){
    // CPU::switch_context(&prev ->_context,&next ->_context);
    Thread::State(FINISHING) // Alteração de estados da Thread para FINISHING indicando a finalização da Thread
    db<Thread>(TRC) << "Thread::thread_exit() chamado, Finalização da Thread \n";
    delete (Thread::_context);
    thread_counter = thread_counter--;
}

// Dispacher da Thread (ainda a ser desenvolvido)
void Thread::dispatcher(){
    db<Thread>(TRC) << "Thread::dispacher() chamado\n";
    while(Thread::running()){
        // escolha da próxima thread a ser executada, next() ? 
        Thread::State(READY); // Atualiza o status da thread dispatacher
        // reinserir em ready ?
        // atualiza o ponteiro _running para apontar para a próxima thread a ser executada
        Thread::State(RUNNING);
        Thread::switch_context(prev ->context(),next ->context()); // troca de contexto entre a thread atual e a proxima
        if next->Thread::State(FINISHING) {
            // remover next da lista de prontos, como ?
        }
        // muda o estado da thread dispatcher para FINISHING
        // remova a thread dispatcher da fila de prontos
        Thread::switch_context(prev -> Thread::dispatcher(), next -> ThreadMain())// troque o contexto da thread dispatcher para main
        return 0;
    }
}
// Switch context
int Thread::switch_context(Thread * prev, Thread *next){
    _running = next;
    CPU::switch_context(prev ->context(),next ->context());
    return 0;
}
__END_API