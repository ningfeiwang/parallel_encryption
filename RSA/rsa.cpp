#include <string>
#include <iostream>
#include "time.h"


using namespace std;

unsigned char msg[8] ={ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
unsigned int C_uint[10];
unsigned int Dec[10];
unsigned int PP = 7;
unsigned int QQ = 17;

bool JudgePrimeNum(unsigned int num)
{
    
    for(unsigned int devider=2; devider < num/2; devider++)
    {
       if(num%devider==0)
           return false;
    }
    return true;
}


unsigned int RandomlyGenerateE(unsigned int t)
{
    unsigned int e=0;
    srand((unsigned int)time(0));
    e=2+rand()%(t-3);
    return(e);
}

void Gcd(unsigned int BigNum,unsigned int SmallNum,unsigned int &MaxGcd )
{
    int tmp=0;
   while(BigNum%SmallNum)
   {

       tmp=SmallNum;
       SmallNum=BigNum%SmallNum; 
       BigNum=tmp;
   }
   MaxGcd=SmallNum;
}

bool JudgeGcd_1(unsigned int BigNum,unsigned int SmallNum)
{
    unsigned int M=0;
    Gcd( BigNum,SmallNum,M);
    if(M==1)
        return true;
    else
        return false;
}



int Moni(unsigned int e, unsigned int model, unsigned int* d)
{
    unsigned int i;
    unsigned int over = e;
    for (i = 1; i<model;)
    {
        over = over % model;
        if (over == 1)
        {
            *d = i;
            return 1;
        }
        else
        {
            if (over + e <= model)
            {
                do
                {
                    i++;
                    over += e;
                } while (over + e <= model);
            }
            else

            {
                i++;
                over += e;
            }
        }
    }
    return 0;
}

void ProduceKey(unsigned int p,unsigned int q,unsigned int &e,unsigned int &d,unsigned int &n)
{
    unsigned int t=0;
    n=p*q;
    t=(p-1)*(q-1);
    e=RandomlyGenerateE(t);
    while(!JudgeGcd_1(t,e))
    {
        e=RandomlyGenerateE(t);
    }
    Moni(e,t,&d);

}
unsigned int Modular_Ex(unsigned int e1,int b,const unsigned int m)
{
    unsigned int i;
    unsigned int tmp=b;
     for(i=0;i<e1;i++)
      {
        b=b%m;
        b=b*tmp;
      }
     return  b/tmp;
}

int BianaryTransform(int num, int bin_num[])
{

    int i = 0, mod = 0;

    while(num != 0)
    {
        mod = num%2;
        bin_num[i] = mod;
        num = num/2;
        i++;
    }

    return i;
}

unsigned int Modular_Exonentiation(unsigned int a, int b, int n)
{
    int c = 0, bin_num[1000];
    long long d = 1;
    int k = BianaryTransform(b, bin_num)-1;

    for(int i = k; i >= 0; i--)
    {
        c = 2*c;
        d = (d*d)%n;
        if(bin_num[i] == 1)
        {
            c = c + 1;
            d = (d*a)%n;
        }
    }
    return d;
}

void RSA_Encrytion(unsigned int e1,const unsigned int n1)
{
    unsigned int i;
    unsigned int tmp;
    int j;
    for(j=0;j<sizeof(msg);j++)
    {

        C_uint[j]=Modular_Exonentiation(msg[j],e1,n1);

   }

}

void RSA_Decrytion(unsigned int d2,const unsigned int n2)
{
    unsigned int i;
    unsigned int tmp;
    int j;
    for(j=0;j<sizeof(msg);j++)
    {

        Dec[j]=Modular_Exonentiation(C_uint[j],d2,n2);


    }
}

void RunRSA()
{
   unsigned int e=0;
   unsigned int d=0;
   unsigned int n=0;//n=p*q
   if (e == d)
        ProduceKey(PP,QQ,e,d,n);
   cout<<"e:"<<e<<endl;
   cout<<"d:"<<d<<endl;
   RSA_Encrytion( e, n);
   RSA_Decrytion( d,n);
}
int main()
{
    RunRSA();
    cout<<sizeof(unsigned int);
    cout<<"plain:"<<msg<<endl;
     cout<<"cifer:"<<endl;
    for(int i=0;i<sizeof(msg);i++)
    {
      cout<<char(C_uint[i]);
    }
    cout<<endl;
     cout<<"after decryption:"<<endl;
    for(int i=0;i<sizeof(msg);i++)
    { 
       cout<<char(Dec[i]);
    }
    cout << endl;
    return 0;
}
