#ifndef semaphore_h
#define semaphore_h

#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"

__BEGIN_API

class Semaphore
{
public:
    typedef Thread::Ready_Queue Waiting_Queue;

    Semaphore(int v = 1): _v(v){
        //new (&_wait) Waiting_Queue();
        db<Semaphore>(TRC) << "Semaphore "<< Semaphore::_v<<"\n"; // imprima informação usando o debug em nível TRC
    };
    ~Semaphore();
    //Semaphore(): _v(1){};
    void p();
    void v();
private:
    // Atomic operations
    int finc(volatile int & number) ;
    int fdec(volatile int & number);

    // Thread operations
    void sleep() {Thread::sleep(&_wait);}
    void wakeup() {Thread::wakeup(&_wait);}
    void wakeup_all();

private:
    //DECLARAÇÃO DOS ATRIBUTOS DO SEMÁFORO
    //static bool _sem;       // flag de uso do semaforo
    volatile int _v;
    Waiting_Queue _wait; // lista de semáforos (para cada thread)
    Thread * _wakeup;
    Thread * _sleep;
};

__END_API
#endif

