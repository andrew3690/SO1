#include "thread.h"
#include "main_class.h"

__BEGIN_API

//Thread* Thread::_joining;
Thread* Thread::_running;
int Thread::thread_counter = 0;
int Thread::_exit_code = 0; // codigo de saída dado pelo método join e repassado para o método thread_exit
Thread Thread::_dispatcher;
Thread Thread::_main;
Ordered_List<Thread> Thread::_sus;
Ordered_List<Thread> Thread::_ready;
CPU::Context Thread::_main_context;
CPU::Context * Thread::context() { return _context;}


int Thread::id(){
    return _id; // implmentação sem erros
}

void Thread::thread_exit(int _exit_code){
    printf("Teste\n");
    db<Thread>(TRC) << "Thread:thread_exit() chamado\n";
    // CPU::switch_context(&prev ->_context,&next ->_context);
    this -> _state = FINISHING; // Alteração de estados da Thread para FINISHING indicando a finalização da Thread
    // Finalização de thread
    Thread::resume();
    if(_joining == this){
        printf("Teste 1\n");
        _joining = 0;
    }

    Thread::yield();

}

void Thread::init(void (*main)(void *)) {

    db<Thread>(TRC) << "Thread::init() chamado\n";
    
    new (&_main) Thread (main, (void *) "main"); // Criação da Thread Main

    new (&_dispatcher) Thread((void (*) (void*)) &Thread::dispatcher, (void*) NULL); // Criação da Thread Dispatcher
    
    new (&_main_context) CPU::Context();
    //eu chamo isso pra nao alocar memoria novamente quando eu criar, pq ele é estatico e ja tem memoria alocada
    
    _running = &_main; 
    _main._state = RUNNING; // Troca de estado da Thread Main para Running
    
    
    CPU::switch_context(&_main_context, _main.context());

}

void Thread::dispatcher(){

// Dispacher da Thread (ainda a ser desenvolvido)
    db<Thread>(TRC) << "Thread::dispacher() chamado\n";
    while (_ready.size() > 1) {

        Thread * next = _ready.remove()->object(); // escolha da próxima thread a ser executada
        
        _dispatcher._state = READY; //descobrimos novo jeito de usar
        _ready.insert(&_dispatcher._link);
        // reinserir em ready
        
        _running =  next;// atualiza o ponteiro _running para apontar para a próxima thread a ser executada
        
        _running -> _state = RUNNING; // Atualiza o status da thread dispatacher
        Thread::switch_context(&_dispatcher,next);// troca de contexto entre a thread atual e a proxima

        if (next -> _state == FINISHING) {
            _ready.remove(&(next->_link)); // remover next da lista de prontos, como ? não sei se é assim que se faz

        }
        
    }
    _dispatcher._state = FINISHING; // muda o estado da thread dispatcher para FINISHING

    _ready.remove(&_dispatcher._link); //eu entendi isso, mas tu remove o contexto dela ?? remova a thread dispatcher da fila de prontos
        
    Thread::switch_context(&_dispatcher,&_main);// troque o contexto da thread dispatcher para main
}

void Thread::yield(){
    
    db<Thread>(TRC) << "Thread::yield() chamado\n"; // imprima informação usando o debug em nível TRC
    
    Thread * exec = running();
    db<Thread>(TRC) << "Tamanho fila "<< _ready.size()<<"\n";

    Thread * next = _ready.remove()->object(); // escolha da próxima thread a ser executada

    if (exec-> _state != FINISHING && exec != &_main) {
        
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
        exec -> _link.rank(now); //atualiza o timestamp
        exec -> _state = READY;
        _ready.insert(&exec->_link); // reinserindo a thread que estava executando na fila de prontos
    }
    _running = next;
    next -> _state = RUNNING;   // atualiza o estado da próxima thread a ser executada

    db<Thread>(TRC) << "Thread::yield(): Trocando contexto de " << exec->_id << " para " << next->_id << "\n";
    switch_context(exec, next);// troque o contexto entre as threads
}

int Thread::switch_context(Thread *prev, Thread *next)
{
    return CPU::switch_context(prev->_context, next->_context);
}

int Thread::join(){
    db<Thread>(TRC) << "Thread::join() chamado\n";
    _joining = running();
    
    if(_joining->_state != FINISHING){
        printf("entrei no suspend\n");
        printf("%d\n",_joining->_id);
        _joining->suspend(); // suspende a execução da Thread
    }
    return _exit_code++; // Thread::thread_exit(_exit_code);
}

void Thread::resume(){
    db<Thread>(TRC) << "Thread::resume() chamado\n";

    Thread * finish = _sus.remove()->object();
    finish->_state = READY; // ..troca de estado para pronto...
    db<Thread>(TRC) << "troca de estado feita \n";
    _ready.insert(&finish->_link);// ... insere-a na fila de prontos.
}

void Thread::suspend(){
    db<Thread>(TRC) << "Thread::suspend() chamado\n";

    this->_state = SUSPENDED; // troca de estado para suspenso
    db<Thread>(TRC) << "tirando da lista de prontos \n";
    //_ready.remove(&this->_link); // retira thread suspensa da fila de prontos
    db<Thread>(TRC) << "inserindo na lista de suspensos\n";
    _sus.insert(&this->_link);// thread em execução está sendo inserida na fila de suspensos
    
    //this->thread_exit(_exit_code);
    // enquanto resume não está sendo chamado
    db<Thread>(TRC) << "esperando término da thread alvo\n";
    Thread::yield();
    
}

Thread::~Thread(){

    db<Thread>(TRC) << "Thread::~Thread() chamado"<< Thread::_id<<"\n"; // imprima informação usando o debug em nível TRC
    _exit_code = 0;
    _ready.remove(this);
    delete _context;
}

__END_API