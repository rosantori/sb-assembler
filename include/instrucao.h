#include <string>

class Instrucao
{
    public: 
        Instrucao(std::string, int, int, int);
        int GetOp();
        int GetCod();
        int GetTam();

    protected:
        std::string mnemonico;
        int op, cod, tam; //operando, codigo e tamanho, rescpectivamente

}