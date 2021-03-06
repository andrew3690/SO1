#ifndef cpu_h
#define cpu_h

#include <ucontext.h>
#include <iostream>
#include "traits.h"

// Trabalho realizado por:
// André Luiz Souza dos Santos (19150871)
// Samuel Vieira Bernardo (19200439)

__BEGIN_API

class CPU
{
    public:
        
        class Context
        {
        private:
            static const unsigned int STACK_SIZE = Traits<CPU>::STACK_SIZE;
        public:
            Context() { _stack = 0; }

            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an);

            ~Context();

            void save();
            void load();

        private:            
            char *_stack;
        public:
            ucontext_t _context;

        };

    public:
        static int fdec(volatile int & number);
        static int finc(volatile int & number);
        static int switch_context(Context *from, Context *to);
        

};

// Instanciação de classe Context
template<typename ... Tn>
CPU::Context::Context(void (* func)(Tn ...), Tn ... an)
{
    save();
    
    _stack = new char[STACK_SIZE];
    
    if (!_stack) {
        std::cout << "Sem stack" << std::endl;
        exit(-1);
    }

    _context.uc_link = 0;
    _context.uc_stack.ss_sp = (void *) _stack;
    _context.uc_stack.ss_size = STACK_SIZE;
    _context.uc_stack.ss_flags = 0;
    
    makecontext(&_context, (void (*)()) func, sizeof...(Tn), an...);
}

__END_API

#endif