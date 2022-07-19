#include "semaphore.h"
#include "thread.h"
#include <bits/stdc++.h>

__BEGIN_API
//Semaphore::_sleep = false;

void Semaphore::p(){
    // Decrementar o inteiro do semáforo de forma atomica (chamando fdec)
    // idéia: inserir em uma fila de threads adormercidas,
    // Colocar a thread para dormir caso a mesma não conseguir acessar o semáforo (caso o semáforo já esteja em uso por outra thread)
    db<Semaphore>(TRC) << "decremento realizado v= "<< Semaphore::_v<<"\n"; // imprima informação usando o debug em nível TRC
    if (fdec(_v) < 1){
        Semaphore::sleep();
    }
}

void Semaphore::v(){
    // Incrementar o inteiro do semáforo de forma atomica (chamando finc)
    // "Acordar" uma thread que esteja dormindo no semáforo
    db<Semaphore>(TRC) << "incremento realizado v= "<< Semaphore::_v<<"\n"; // imprima informação usando o debug em nível TRC
    if(finc(_v) < 0){ // semáforo está livre
        // db<Semaphore>(TRC) << "Entrei em fdec com v ="<< Semaphore::_v <<"\n"; // imprima informação usando o debug em nível TRC
        //_v = finc(_v);
        Semaphore::wakeup();
    }
}

int Semaphore::fdec(volatile int & number){
    return CPU::fdec(number);
    // int decrementa = -1;
    // asm("lock xadd %0, %2" : "=a"(decrementa) : "a" (decrementa), "m"(number): "memory");
    // return decrementa;
}


int Semaphore::finc(volatile int & number){
    return CPU::finc(number);
    // int incrementa = 1;
    // asm("lock xadd %0, %2" : "=a"(incrementa) : "a" (incrementa), "m"(number): "memory");
    // return incrementa;
}

// void Semaphore::sleep(){
//     db<Semaphore>(TRC) << "chamando sleep "<< Semaphore::_v<<"\n"; // imprima informação usando o debug em nível TRC
//     Thread::sleep(&_wait);
//     //Thread * _sleep = Thread::running();                               // inserção de thread na lista de threads que estão esperando
//     //_sleep->sleep(&_wait);
//     // _atual = Thread::running();
//     // _atual->sleep();
//     //Thread::sleep(); // chamando sleep() em Thread
// }

// void Semaphore::wakeup(){
//     db<Semaphore>(TRC) << "chamando wakeup "<< Semaphore::_v<<"\n"; // imprima informação usando o debug em nível TRC
//     //Thread::wawakeup(&_wait);
//     //Thread * _wakeup = _wait.remove()->object();
//     //_wakeup->wakeup(&_wait);
//     // _wakeup = _wait.remove()->object();
//     // _wakeup->wakeup();
//     //Thread::wakeup(); // chamando wakeup() em Thread
// }

Semaphore::~Semaphore(){
    db<Semaphore>(TRC) << "Terminando semáforo \n"; // imprima informação usando o debug em nível TRC
}

// void Semaphore::wakeup_all(){
//     while(!_wait.empty()) {
//         Thread * _wakeup = _wait.remove()->object();
//         _wakeup->wakeup(&_wait);
//     }
// }

__END_API