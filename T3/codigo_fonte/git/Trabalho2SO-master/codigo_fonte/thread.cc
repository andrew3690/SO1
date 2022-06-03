#pragma once
#include "thread.h"
#include <iostream>
#include "system.cc"
#include <ctime> 
#include <chrono>


__BEGIN_API

Thread* Thread::_running = nullptr;

int Thread::id(){
    return(_id); // implmentação sem erros
}

Thread::init(void (*main)(void *)) {

    db<Thread>(TRC) << "Thread::init() chamado\n";
    
    new (&_main) Thread (main, (void *) "main"); // Criação da Thread Main

    new (&_dispatcher) Thread((void (*) (void*)) &Thread::dispatcher, (void*) NULL); // Criação da Thread Dispatcher
    //eu chamo isso pra nao alocar memoria novamente quando eu criar, pq ele é estatico e ja tem memoria alocada
    
    _main._state = RUNNING; // Troca de estado da Thread Main para Running
    
    _dispatcher.state = READY; // Troca de estado da Thread Dispatcher READY

    _ready.insert(&_dispatcher._link); // insere a thread dispatcher na fila de prontos
    
    new (&_main_context) CPU::Context();
    CPU::switch_context(&_main_context, _main.context());
    return 0;
}

// Finalização de thread
void Thread::thread_exit(int exit_code){
    db<Thread>(TRC) << "Thread::thread_exit() chamado\n";
    // CPU::switch_context(&prev ->_context,&next ->_context);
    _running->_state = FINISHING; // Alteração de estados da Thread para FINISHING indicando a finalização da Thread
    _ready.remove(&_running);
    thread_counter = thread_counter--;


}

// Dispacher da Thread (ainda a ser desenvolvido)
Thread::dispatcher(){
    db<Thread>(TRC) << "Thread::dispacher() chamado\n";
    while (thread_counter > 2) {

        Thread * next = _ready.remove()->object(); // escolha da próxima thread a ser executada
        
        _dispatcher._state = READY; //descobrimos novo jeito de usar
        
        _ready.insert(_dispatcher._link);// reinserir em ready ? isso funciona ??
        
        *_running = & next;// atualiza o ponteiro _running para apontar para a próxima thread a ser executada
        
        next._state = RUNNING; // Atualiza o status da thread dispatacher
        
        CPU::switch_context(_running.context(),next.context())// troca de contexto entre a thread atual e a proxima

        if (next._state = FINISHNING) {
            _ready.remove(&next); // remover next da lista de prontos, como ? não sei se é assim que se faz
            thread_counter = thread_counter--;
        }
    }
    _dispatcher._state = FINISHING; // muda o estado da thread dispatcher para FINISHING

    _ready.remove(&_dispatcher._link); //eu entendi isso, mas tu remove o contexto dela ?? remova a thread dispatcher da fila de prontos
        
    CPU::switch_context(_dispatcher.context(),_main.context())// troque o contexto da thread dispatcher para main
}


// yield
Thread::yield(){
    db<Thread>(TRC) << "Thread::yield() chamado\n"; // imprima informação usando o debug em nível TRC

    Thread * next = _ready.remove()->object(); // escolha da próxima thread a ser executada
    
    if (next-> _state != Thread::State::FINISHING || next == _main) {
        
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
        thread -> _link.rank(now); //atualiza o timestamp
        
        _ready.insert(&thread._link); // reinserindo a thread que estava executando na fila de prontos
    }

    _running._state = READY; //coloca ela como ready 

    next._state = READY;// atualiza o estado da próxima thread a ser executada
    
    CPU::switch_context(_running.context(), next.context());// troque o contexto entre as threads
}

// Switch context
int Thread::switch_context(Thread * prev, Thread *next){
    _running = next;
    CPU::switch_context(prev ->context(),next ->context());
    return 0;
}

/*
~Thread(){
//ver com ele 
    db<Thread>(TRC) << "Thread::~Thread() chamado\n"; // imprima informação usando o debug em nível TRC
    delete(_running.context())
    _ready.remove(_running._link);
}
*/
__END_API