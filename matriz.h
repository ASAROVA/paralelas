#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;
#define SL cout<<endl
#define MIN 0
#define MAX 10

/// Generador de Numero Aleatorio /////////////////
#include <ctime>
int aleatorio (int minimo=0, int maximo=RAND_MAX )
{
    static bool inicio = true;
    if (inicio) { srand(time(NULL));
                  inicio = false;    }
    return minimo + rand()%(maximo-minimo);
}
/// ////////////////////////////////////////////*/


/// Matriz ///////////////////////////////////////
template<class T>
class matriz;

template<class T>
matriz<T> multibloque (const matriz<T>* A, const matriz<T>* B, int n=3)
{
    if (!n) return (*A)*(*B);

    /*
    cout << "A = " << "\t\t\t\t\t\tn = " << n; SL;
    cout << *A; SL;SL;
    cout << "B = " << "\t\t\t\t\t\tn = " << n; SL;
    cout << *B; SL;SL;
    */

    int mm = B->nfilas;
    int m  = B->nfilas/2;

    matriz<T> A1 ( A->nfilas,m);
    matriz<T> A2 ( A->nfilas,mm-m);

    for (int i=0;i<A->nfilas;i++)
    {
        int j=0;
        for (;j<m;j++)  A1[i][j] = (*A)[i][j];
        for (;j<mm;j++) A2[i][j-m] = (*A)[i][j];
    }

    matriz<T> B1 ( m, B->ncolum);
    matriz<T> B2 ( mm-m, B->ncolum);

    for (int i=0;i<B->ncolum;i++)
    {
        int j=0;
        for (;j<m;j++)  B1[j][i] = (*B)[j][i];
        for (;j<mm;j++) B2[j-m][i] = (*B)[j][i];
    }

    matriz<T> P = multibloque(&A1,&B1,n-1) + multibloque(&A2,&B2,n-1);

    /*
    cout << "P = " << "\t\t\t\t\t\tn = " << n; SL;
    cout << P; SL;SL;
    */

    return P;
}

template<class T>
class matriz : private vector<vector<T>>
{
protected:
    int nfilas, ncolum;
    typedef vector<vector<T>> vvt;

public:

    using vvt::push_back;
    using vvt::operator[];
    using vvt::begin;
    using vvt::end;

    matriz(): nfilas(0), ncolum(0), vvt() {}
    matriz(initializer_list<vector<T>> L): vvt(L)
    {
        nfilas = L.size();
        ncolum = L.begin()->size();
    }
    //virtual ~matriz() { vvt::~vvt(); }

    matriz(int num_filas, int num_columnas, T val = T())
    {
        nfilas = num_filas;
        ncolum = num_columnas;
        this->resize(nfilas);
        for (auto& i: *this)
        {
            i.resize(ncolum);
            for(auto& ij: i) ij = val;
        }
    }

    //matriz(vector<vector<T>>::iterator i,vector<vector<T>>::iterator j): vvt(i,j) {}

    matriz<T> operator* (const matriz<T>& M) const
    {
        if(M.nfilas!=ncolum)
            throw invalid_argument( "Dimensiones incompatibles" );

        matriz<T> P(nfilas,M.ncolum,T());

        for(int i=0; i<nfilas; i++)
        {
            for(int j=0; j<M.ncolum; j++)
                for(int k=0; k<ncolum; k++)
                    P[i][j] += (*this)[i][k]*M[k][j];
        }
        return P;
    }

    matriz<T> mbloque (const matriz<T>& M) const
    {
        if(M.nfilas!=ncolum)
            throw invalid_argument( "Dimensiones incompatibles" );

        return multibloque(this, &M);
    }

    matriz<T> operator+ (const matriz<T>& M) const
    {
        if(M.nfilas!=nfilas or M.ncolum!=ncolum)
            throw invalid_argument( "Dimensiones incompatibles" );

        matriz<T> S(nfilas,ncolum);

        for(int i=0; i<nfilas; i++)
            for(int j=0; j<ncolum; j++)
                S[i][j] += (*this)[i][j]+M[i][j];
    return S;
    }

    void rellenar ( int minimo=MIN, int maximo=MAX   )
    {
        for (auto& i: *this)
            for(auto& ij: i) ij = aleatorio(minimo,maximo);
    }

    void imprimir()
    {
        for(int i=0; i<nfilas; i++)
        {
            for(int j=0; j<ncolum; j++)
                cout << (*this)[i][j] << "\t";
            SL;
        }
    }

    template<class TT>
    friend ostream& operator<<(ostream &, const matriz<TT> &);

    template<class TT>
    friend matriz<TT> multibloque (const matriz<TT>*, const matriz<TT>*, int);
};

template<class T>
ostream &operator<<(ostream &os, matriz<T> const &M)
{
    for(int i=0; i<M.nfilas; i++)
    {
        for(int j=0; j<M.ncolum; j++)
            os << M[i][j] << "\t";
        os << "\n";
    }
    return os;
}

typedef matriz<int>   matrint;
typedef matriz<float> matflot;
/// ////////////////////////////////////////////*/


/// Functor que mide la demora de una función ////
template<typename T, typename TF>
class cronometro
{
    TF f;
public:
    cronometro() {}
    void est_funcion(TF F) {f=F;}
    double operator()(int alto, int ancho=0)
    {
        if(!ancho) ancho = alto;

        matriz<T> M(alto,ancho);
        M.rellenar();
        matriz<T> N(alto,ancho);
        N.rellenar();

        clock_t t;
        t = clock();
        (M.*f)(N);
        t = clock()-t;
        return double(t)/double(CLOCKS_PER_SEC);
        //return double(t);
    }
};
/// ////////////////////////////////////////////*/


/// Rutina de Tabulacion de vectores /////////////
template <class T, class... TY>
void tabulador(vector<T> X, TY... Ys)
{
    int n = X.size();
    for(int i=0;i<n;i++)
    {
        cout << X[i] << "\t\t";
        for(auto& Y: {Ys...})
            cout << Y[i] << "\t\t";
        SL;
    }
}
/// ////////////////////////////////////////////*/


/// Rellenador de vectores por interpolacion /////
template <class T>
void interpolar(vector<T>& v,T inicio,T incremento,T fin)
{
    v.resize( 1 + (int)(fin-inicio)/(int)incremento );
    T x = inicio;
    for(auto& i:v)
    {
        i = x;
        x += incremento;
    }
}
/// ////////////////////////////////////////////*/


#endif // MATRIZ_H_INCLUDED
