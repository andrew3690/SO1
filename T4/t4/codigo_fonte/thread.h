#ifndef thread_h
#define thread_h

#include "cpu.h"
#include "traits.h"
#include "debug.h"
#include "list.h"
#include <ctime> 
#include <chrono>


__BEGIN_API

class Thread
{
protected:
    typedef CPU::Context Context;
public:

    typedef Ordered_List<Thread> Ready_Queue;
    typedef Ordered_List<Thread> Suspended_Queue;

    // Thread State
    enum State {
        RUNNING,
        READY,
        FINISHING,
        SUSPENDED
    };

    /*
     * Construtor vazio. Necessário para inicialização, mas sem importância para a execução das Threads.
     */ 
    Thread() { }

    /*
     * Cria uma Thread passando um ponteiro para a função a ser executada
     * e os parâmetros passados para a função, que podem variar.
     * Cria o contexto da Thread.
     * PS: devido ao template, este método deve ser implementado neste mesmo arquivo .h
     */ 
    template<typename ... Tn>
    Thread(void (* entry)(Tn ...), Tn ... an);

    /*
     * Retorna a Thread que está em execução.
     */ 
    static Thread * running() { return _running; }

    /*
     * Método para trocar o contexto entre duas thread, a anterior (prev)
     * e a próxima (next).
     * Deve encapsular a chamada para a troca de contexto realizada pela class CPU.
     * Valor de retorno é negativo se houve erro, ou zero.
     */ 
    static int switch_context(Thread * prev, Thread * next);

    /*
     * Termina a thread.
     * exit_code é o código de término devolvido pela tarefa (ignorar agora, vai ser usado mais tarde).
     * Quando a thread encerra, o controle deve retornar à main. 
     */  
    void thread_exit (int _exit_code);

    /*
     * Retorna o ID da thread.
     */ 
    int id();

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Daspachante (disptacher) de threads. 
     * Executa enquanto houverem threads do usuário.
     * Chama o escalonador para definir a próxima tarefa a ser executada.
     */
    static void dispatcher(); 

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Realiza a inicialização da class Thread.
     * Cria as Threads main e dispatcher.
     */ 
    static void init(void (*main)(void *));


    /*
     * Devolve o processador para a thread dispatcher que irá escolher outra thread pronta
     * para ser executada.
     */
    static void yield(); 

    /*
     * Destrutor de uma thread. Realiza todo os procedimentos para manter a consistência da classe.
     */ 
    ~Thread();
    
    /*
     *Método que deve suspender a thread em execução até que a thread "alvo" finalize.
     *O inteiro retornado por join() é o argumento recebido por thread_exit().
    */
   int join();

    /*
     *
     */
    void suspend();

    /*
     *
     */
    void resume();

    /*
     * Qualquer outro método que você achar necessário para a solução.
     */ 
    CPU::Context * context();

private:
    int _id;
    Context * volatile _context;
    static Thread * _running;
    
    static Thread _main; 
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ready_Queue _ready;
    Ready_Queue::Element _link;
    volatile State _state;
    int static thread_counter;
    int static _exit_code; // codigo de saída dado pelo método join e repassado para o método thread_exit
    static Suspended_Queue _sus; // instancição da fila de threads suspensas
    Suspended_Queue::Element _elm; // fila de threads suspensas
    Thread * _joining;
    /*
     * Qualquer outro atributo que você achar necessário para a solução.
     */ 

};


template<typename ... Tn>
inline Thread::Thread(void (* entry)(Tn ...), Tn ... an) : _link(this, (std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::high_resolution_clock::now().time_since_epoch()).count()))
{
    db<Thread>(TRC) << "Thread::Criando Thread\n";

    //IMPLEMENTAÇÃO DO CONSTRUTOR
    _context = new CPU::Context(entry, an ... ); // criação de contexto
    
    if (!_context) {
        std::cout << "Sem contexto" << std::endl;
        exit(-1);
    }

    _state = READY;
    _id = thread_counter++; // incremento do contador d~Threade threads
    
    //_link(Thread(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    if (thread_counter > 1){
     //se trocar pra -id > 1, ele ira ter todos os p@ang : fim, porem core dumped.
        _ready.insert(&_link); //inserindo a thread na fila de prontos
    }

    _joining = 0;
};

__END_API

#endif