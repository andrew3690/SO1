#include "cpu.h"
#include <iostream>
// Trabalho realizado por:
// André Luiz Souza dos Santos (19150871)
// Samuel Vieira Bernardo (19200439)

__BEGIN_API

void CPU::Context::save()
{
    //adicionar implementação
    // Usar a classe getcontext() aqui, salvando ponteiros do contexto atual
    getcontext( &_context); // salvando contexto 1
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
    _stack = 0; // tamanho da pilha vai a zero
    //delete Context();
}

void CPU::switch_context(Context *from, Context *to)
{
     //implementação do método
    swapcontext(&from ->_context,&to ->_context);
}

__END_API
