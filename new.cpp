//
// Copyright (c) 2015 by Xin Wang.
// All Rights Reserved.
//



#include <iostream>
#include <stdio.h>
#include <stdlib.h> /*srand, rand*/
#include <time.h>   /*time*/
#include <assert.h>
#include "mkl.h"

using namespace std;

/**
    matrix class

    This is a matrix class, mainly supporting fast matrix multiplication by call the
    cblas_dgemm function in mkl.
 */

class matrix{
public:
    /**
        member variables
        @m: number of row in the matrix
        @n: number of column in the matrix
        @p: A pointer to a 1-D double array to represent 2-D matrix, with lenth m*n*sizeof(double)
     */
    int m,n;
    double *p;
    //matrix(){}
    /**
        Constructor with default args.
        init a matrix with shape(row,col), elements of it are randomized.
        @row: row number of matrix
        @col: col number of matrix
     */
    matrix(int row = 2, int col = 2)
    {
        m= row;
        n = col;
        p = (double*)mkl_malloc(m*n*sizeof(double),64);
        srand(time(NULL));
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++){
                /* An integer value between 0 and RAND_MAX.
                  RAND_MAX is a constant defined in <cstdlib>*/
                p[i*n+j] = rand()%(1<<12);
            }
        }
    }
    /**
        Copy Constructor
     */
    matrix(const matrix& a) //reference must
    {
        //matrix(a.m,a.n);
        /*First initialize "this" object(memory allocation),
        or it will lead to undefined behavior(bus error:10)*/
        m= a.m;
        n = a.n;
        p = (double*)mkl_malloc(m*n*sizeof(double),64);

        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                p[i*n+j] = a.p[i*n+j];
    }
    /**
        Identity matrix Constructor
        @d: dimension of the Identity
        @c: char should be set to 'i', tell the Constructor to generate identity matrix
     */
    matrix(int d, char c)
    {
        assert(c == 'i');
        m = n = d;
        p = (double*)mkl_malloc(m*n*sizeof(double),64);
        memset(p,0,m*n*sizeof(double));
        for(int i=0;i<m;i++)
            p[i*m+i] = 1.0;
    }
    /**
        Destructor
        Call mkl_free to free array pointer p.
     */
    ~matrix()
    {
        mkl_free(p);
    }
    /**
        Transpose a matrix
     */
    friend matrix transpose (const matrix&);
    /**
        Overloading operator * between matries
        @args: two matries as args, number of cols of the first matrix equals to number
        rows of the second matrix.
     */
    friend matrix operator * (const matrix&, const matrix&);
    /*Overloading operator * between matrix and double
     */
    friend matrix operator * (const matrix&, const double);
    /*Overloading operator * between double and matrix
     */
    friend matrix operator * (const double , const matrix&);
    /*Overloading operator /
     */
    const matrix& operator / (const double );
    friend matrix operator + (const matrix&, const matrix&);
    friend matrix operator - (const matrix&, const matrix&);

    friend matrix A_on_B (const matrix&, const matrix&);
    friend matrix A_concatnate_B (const matrix&, const matrix&);
    friend bool operator == (const matrix& A, const matrix& B);
    /*display
      Display the matrix
    */
    void display()
    {
        cout<<"The matrix is";
        for(int i=0;i<m;i++){
            cout<<endl;
            for(int j=0;j<n;j++){
                cout<<long (p[i*n+j])<<" ";
            }
        }
        cout<<endl;
    }

};

void shape(matrix & A)
{
    cout<<"shape is: ("<<A.m<<" "<<A.n<<")"<<endl;
}

matrix transpose(const matrix& A)
{
    matrix res(A.n,A.m);
    for(int i=0;i<A.m;i++)
        for(int j=0;j<A.n;j++)
            res.p[j*A.m+i] = A.p[i*A.n+j];
    return res;
}

matrix A_on_B (const matrix& A, const matrix& B)
{
    assert(A.n == B.n);
    int m = A.m+B.m, n = A.n;
    matrix res(m,n);
    for(int i=0;i<A.m;i++)
        for(int j=0;j<n;j++)
            res.p[i*n+j] = A.p[i*n+j];
    for(int i=0;i<B.m;i++)
        for(int j=0;j<n;j++)
            res.p[(i+A.m)*n+j] = B.p[i*n+j];
    return res;
}

matrix A_concatnate_B (const matrix& A, const matrix& B)
{
    assert(A.m == B.m);
    int m = A.m,n=A.n+B.n;
    matrix *res = new matrix(m,n);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++){
            if(j<A.n)  res->p[i*n+j] = A.p[i*A.n+j];
            else  res->p[i*n+j] = B.p[i*B.n+j-A.n];
        }
    return *res;
}

bool operator == (const matrix& A, const matrix& B)
{
    assert(A.m==B.m && A.n == B.n);
    for(int i=0;i<A.m;i++)
        for(int j=0;j<A.n;j++)
            if(A.p[i*A.n+j] != B.p[i*A.n+j]) return false;
    return true;
}

matrix operator + (const matrix& A , const matrix& B)
{
    assert(A.m==B.m && A.n==B.n);
    int m=A.m, n=A.n;
    matrix res(m,n);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            res.p[i*n+j] = A.p[i*n+j] + B.p[i*n+j];
    return res;
}

matrix operator - (const matrix& A , const matrix& B)
{
    int m=A.m,n=A.n;
    matrix res = matrix(m,n);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            res.p[i*n+j] = A.p[i*n+j] - B.p[i*n+j];
    return res;
}

matrix operator * (const matrix & A , const matrix & B)
{
    assert(A.n == B.m);
    matrix * res= new matrix (A.m,B.n);
    int m,n,p;
    m = A.m;
    p = A.n;
    n = B.n;
    double alpha = 1.0, beta = 0.0;
    //int a = clock();
    double s_initial,s_elapsed;
    s_initial = dsecnd();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,m,n,p,alpha,A.p,p,B.p,n,beta,(*res).p,n);
    return *res;
}


matrix operator *(const matrix& A,const double a)
{
    matrix res(A);
    for(int i=0;i<A.m;i++)
        for(int j=0;j<A.n;j++)
            res.p[i*A.n+j] =A.p[i*A.n+j]*a;
    return res;
}

matrix operator *(const double a,const matrix& A)
{
    return operator*(A,a);
}

const matrix& matrix::operator / (double div)
{
    int m = this->m, n = this->n;
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            this->p[i*n+j]/=div;
    return *this;
}

int l = 60;
int extension = 10;

double max(const matrix& A)
{
    double tmp = -(1<<20);
    for(int i=0;i<A.m*A.n;i++)
        if(A.p[i]>tmp) tmp=A.p[i];
    return tmp;
}

/**
    get c*

    This method gets the binary represented matrix of c by representing every element
    in c with l bit( 2^l>|c|,where |c| is the max abs value of c, so that no loss
    *happens ). For every element in c*, possible values are 1, 0, -1.

    Examples:

    Represent 7 and -7 with l=4 bits:
    7  --> [1,1,1,0]
    -7 --> [-1,-1,-1,0]

    @c: ciphertext
 */
matrix getBitVector(const matrix& c)
{
    int n = c.m;
    assert(c.n==1);
    matrix *res = new matrix(n*l,1);
    memset(res->p,0,n*l*sizeof(double));
    for(int i=0;i<n;++i){  //1 element to l-bit
        int sign = 1;
        if(c.p[i]<0) sign = -1;
        for(int j=0;j<l;++j){
            res->p[i*l+j] = sign * (int(c.p[i])&1);
            if(c.p[i])
            c.p[i]/=2;
        }
    }
    return *res;
}

/**
    get S*

    This method gets the binary represented matrix of secret_key by representing every
    elementin secret_key with l bit( 2^l>|secret_key|,where |secret_key| is the max
    abs value of secret_key, so that no loss happens. ). For every element in c*,
    possible values are 1, 0, -1.

    Examples:

    Represent 7 and -7 with l=4 bits:
    7  --> [1,1,1,0]
    -7 --> [-1,-1,-1,0]

    @secret_key: of its literal meaning
 */
matrix getBitMatrix(const matrix& secret_key)
{
    int m = secret_key.m , n = secret_key.n;
    matrix * res = new matrix(m,n*l);
    memset(res->p,0,m*n*l*sizeof(double));             //memset to 0
    for(int i=0;i<m;++i)
        for(int j=0;j<n;++j)
        {
            long tmp = secret_key.p[i*n+j];         //tmp should be long
            for(int k=0;k<l;++k)
                {
                    if(tmp==0) break;
                    res->p[i*n*l+j*l+k] = tmp<<k;
                }
        }
    return *res;
}

/**
    get M
    S: original secret_key
    T: T in new secret_key = [I,T]
 */
const matrix keySwitchMatrix(const matrix& S,const matrix& T)
{
    // bit string of secret_key
    matrix bitwise_secret_key = getBitMatrix(S);
    //cout<<"S*"; shape(bitwise_secret_key);
    // get random matrix A
    matrix A(T.n,bitwise_secret_key.n);
    return A_on_B(bitwise_secret_key-(T*A),A);
}
/**
    return c = M * c*;
 */
matrix* keySwitch(const matrix & M, const matrix & c)
{
    return &(M * getBitVector(c));
}
/**
    S = [I,T]
 */
matrix* encrypt(const matrix& T,const matrix& x,const double w)
{
    matrix *c = keySwitch(keySwitchMatrix(matrix(T.m,'i'),T),w * x);
    return c;
}
/**
    return the nearest integer in the form of double.
*/
double nearestInt(double a)
{
    if((a-int(a))>=0.5000) return 1.0+int(a);
    else return double(int(a));
}
/**/
const matrix decrypt(const matrix & S, const matrix & c, const double w)
{
    matrix Sc = S*c;
    for(int i=0;i<Sc.m;i++)
        for(int j=0;j<Sc.n;j++)
            Sc.p[i*Sc.n+j] = nearestInt(Sc.p[i*Sc.n+j]/w);
    return Sc;
}

const matrix getSecretKey(const matrix& T)
{
    return A_concatnate_B(matrix(T.m,'i'),T);
}

/*  return M corresponding to Transform G
    send M to server to transform
    S: current secret_key
    G: Transform matrix
    T: T in new secret_key(generated by client)
*/
const matrix linearTransformClient(const matrix &T, const matrix& G,const matrix S)
{
    return keySwitchMatrix(G*S,T);
}

/*
    server-side transform
*/
const matrix linearTransform(const matrix& M, const matrix& c)
{
    return M*getBitVector(c);
}
/*
*/
matrix vectorize(const matrix& H)
{
    int m=H.m,n=H.n;
    matrix res(m*n,1);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            res.p[j*m+i]= H.p[i*n+j];
    return res;
}
/*  return M
    S1: secret_key1
    S2: secret_key2
    H: weighted matrix
    T: T in new secret_key
*/
const matrix innerProductClient(const matrix& S1, const matrix& S2,const matrix& H, matrix& T)
{
    matrix old_key = transpose(vectorize(transpose(S1)*H*S2));
    //cout<<"old_key shape: ("<<old_key.m<<" "<<old_key.n<<")"<<endl;
    //cout<<"T "; shape(T);
    //cout<<"max of new S: ";
    //cout<<max(T)<<"*****************"<<endl;
    return keySwitchMatrix(old_key,T);
}

/*return c:
This is for sever side to compute the inner product on ciphers c1 and c2, M is the
transform matrix to calculate the inner product.

@M: Transform matrix generated by client to
*/
const matrix innerProduct(const matrix& M, const matrix& c1, const matrix& c2,double w)
{
    cout<<"M shape: ("<<M.m<<" "<<M.n<<")"<<endl;
    matrix tmp = (vectorize(c1*transpose(c2))/w);
    cout<<"c1*c2/w shape: ("<<tmp.m<<" "<<tmp.n<<")"<<endl;
    return M*getBitVector(tmp);
}

int main(int argc, char * argv[]){

    if(argc<2){cout<<"args < 2 ..."<<endl; return 0;}

    int n = atoi(argv[1]);
    /*
    matrix a(n,n+2);
    a.display();
    matrix b = vectorize(a);
    b.display();
    matrix c = transpose(a);
    c.display();
    */
    /*Weighted Inner Products*/
    /* Setting w >> |S|,where S is any secret_key (after encryption or keySwitch)
        to make sure decryption can be successful.
    */
    double w = (1<<30)*(1.0)*(1<<2);
    matrix a(n,1);
    a.p[0]=100;
    a.p[1]=200;
    a.p[2]=33;
    a.p[3]=47;
    //cout<<"a is:"<<endl;
    //a.display();

    matrix b(n,1);
    b.p[0]=13;
    b.p[1]=250;
    b.p[2]=3700;
    b.p[3]=49;
    //cout<<"b is:"<<endl;
    //b.display();
    
    matrix T(n,extension);
    int s = clock();
    matrix *c = encrypt(T,a,w);
    int t = clock();
    cout<<double(t-s)/CLOCKS_PER_SEC<<endl;

    
	/*
    matrix T1(n,extension),T2(n,extension);
    matrix c1 = encrypt(T1,a,w),c2=encrypt(T2,b,w);
    //encrypt(T,plain,w).display();
    //cout<<"b is:"<<endl;
    //b.display();
    matrix S1= getSecretKey(T1), S2=getSecretKey(T2);

//  for inner product , row number of T should equals to S*, which is always 1
    matrix T(1,extension);
    matrix S = getSecretKey(T);
    //cout<<"new S:"; shape(S);
    matrix M = innerProductClient(S1,S2,matrix(n,'i'),T);

    matrix H(n,'i');

    matrix old_key = transpose(vectorize(transpose(S1)*H*S2));
    matrix tmp = (vectorize(c1*transpose(c2))/w);
    /*
    matrix res = old_key*tmp;
    cout<<"new S*c: "<<endl;
    res.display();
    cout<<"transpose a:"<<endl;

    matrix plain_product = transpose(a)*b;
    cout<<"plain_product"<<endl;
    plain_product.display();


    //cout<<"hehe"<<endl;
    cout<<"SM=S*?" << (S*M == getBitMatrix(old_key))<<endl;
    //(S*M).display();
    //getBitMatrix(old_key).display();

    matrix c = innerProduct(M,c1,c2,w);
    matrix product = S*c/w;
    cout<<"inner product is :"<<endl;
    cout<<product.p[0]<<endl;
    /*
    /*
    matrix cipher = encrypt(T,plain,w);
    cout<<"enced: "<<endl;
    cipher.display();

    matrix dec = decrypt(getSecretKey(T),cipher,w);
    cout<<"deced: "<<endl;
    dec.display();
    */
    return 0;
}
