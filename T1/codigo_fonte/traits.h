#ifndef traits_h
#define traits_h

// Trabalho realizado por:
// André Luiz Souza dos Santos (19150871)
// Samuel Vieira Bernardo (19200439)

// Rodar código usando: 
//  g++ *.cc -o output
// ./output

//Não alterar as 3 declarações abaixo

#define __BEGIN_API             namespace SOLUTION {
#define __END_API               }
#define __USING_API            using namespace SOLUTION;

__BEGIN_API

class CPU; //declaração das classes criadas nos trabalhos devem ser colocadas aqui
//class Context;

//declaração da classe Traits
template<typename T>
struct Traits {
};

template<> struct Traits<CPU>
{
    //Adicionar aqui a declaração do tamanho da Pilha
    static const unsigned int STACK_SIZE = 1024 * 128;// tamanho total da pilha
};

__END_API

#endif
