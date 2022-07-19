#include "cpu.h"
#include <iostream>
#include <bits/stdc++.h>
// Trabalho realizado por:
// André Luiz Souza dos Santos (19150871)
// Samuel Vieira Bernardo (19200439)

__BEGIN_API

void CPU::Context::save()
{
    //adicionar implementação
    // Usar a classe getcontext() aqui, salvando ponteiros do contexto atual
    getcontext(&_context); // salvando contexto 1
    }

void CPU::Context::load()
{
    //adicionar implementação
    // provavel uso de setcontext() aqui para carregar um dado contexto previamente salvo com uso de save()
    setcontext( &_context);
}

CPU::Context::~Context()
{
    //adicionar implementação
    if (_stack){ 
        delete(_stack); // tamanho da pilha vai a zero
    }
    //delete Context();
}

int CPU::switch_context(Context *from, Context *to)
{
     //implementação do método
    return swapcontext(&from ->_context,&to ->_context);
}

 int CPU::fdec(volatile int & number){
     int decrementa = -1;
     asm("lock xadd %0, %2" : "=a"(decrementa) : "a" (decrementa), "m"(number): "memory");
     return decrementa;
}

int CPU::finc(volatile int & number){
     int incrementa = 1;
     asm("lock xadd %0, %2" : "=a"(incrementa) : "a" (incrementa), "m"(number): "memory");
     return incrementa;
}


__END_API
