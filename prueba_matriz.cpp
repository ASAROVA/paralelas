#include <iostream>
#include "matriz.h"

typedef int tipo;
typedef matriz<tipo>(matriz<tipo>::*funcion)(const matriz<tipo>&)const;

using namespace std;
#define SL cout<<endl

int main(void)
{
    cout << "\n\t\t >> ASAROVA << \n\n";

    /// Comparaciones //////////////////////////////

    funcion f;

    //vector<int> tamannos {128,256,512,1024,2048,4096};
    vector<int> tamannos {128,256,512};
    //vector<int> tamannos; interpolar(tamannos,100,100,1000);

    cout << "Tabla de Demoras (seg)"; SL;SL;
    cout << "Tamanno lado\t" << "Prod. Normal\t" << "Prod. Bloques\t"; SL;

    vector<float> lapsos_1, lapsos_2;
    lapsos_1.resize(tamannos.size());
    lapsos_2.resize(tamannos.size());

    cronometro<tipo,funcion> reloj;

    f = matriz<tipo>::operator*;
    reloj.est_funcion(f);
    transform(tamannos.begin(),tamannos.end(),lapsos_1.begin(),reloj);

    f = matriz<tipo>::mbloque;
    reloj.est_funcion(f);
    transform(tamannos.begin(),tamannos.end(),lapsos_2.begin(),reloj);

    tabulador(tamannos,lapsos_1,lapsos_2);

    SL;SL;
    /// ////////////////////////////////////////////*/

    /*// Prueba multibloque //////////////////////////

    matrint M (8,8);
    M.rellenar();
    //cout << M; SL;SL;

    matrint N (8,8);
    N.rellenar();
    //cout << N; SL;SL;

    matrint P = M.mbloque(N);

    cout << "Multibloque(M,N) = "; SL;
    cout << P; SL;SL;
    cout << "M * N = "; SL;
    cout << M*N; SL;SL;

    /// ////////////////////////////////////////////*/

    /*// Prueba M*N ///////////////////////////////////
    matrint M ( {{4,5,6},
                 {1,2,3},
                 {9,8,7},
                 {5,7,3}} );

    matrint N ( {{2,3,1,9},
                 {5,6,4,1},
                 {8,9,7,5}} );
    cout << M*N; SL;SL;

    /// ////////////////////////////////////////////*/

    /*// Prueba M+N ///////////////////////////////////
    matrint M ( {{4,5,6},
                 {1,2,3},
                 {9,8,7}} );

    matrint N ( {{2,3,1},
                 {5,6,4},
                 {8,9,7}} );
    cout << M+N; SL;SL;

    /// ////////////////////////////////////////////*/

	return 0;
}
