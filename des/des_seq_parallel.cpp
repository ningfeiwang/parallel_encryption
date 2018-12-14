// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <time.h>
#include <chrono>
#include <map>
#include <omp.h>
#define M 0x7effffff
using namespace std;
int i,j,k,m;

typedef struct
{
    char Plain_let;
    int BIT_Plain[64];
    int Cipher[64];
	// vector <double> sum_b;
	// vector<int> cir;
}En;

typedef struct
{
    int Plain[64];
    int BIT_Cipher[64];
    int Cipher[64];
    char Word[8];
}De;

char D[M];
int Cipher_F[999999];

int IP[64]={                 
    58 , 50 , 42 , 34 , 26 , 18 , 10 ,  2 ,
    60 , 52 , 44 , 36 , 28 , 20 , 12 ,  4 ,
    62 , 54 , 46 , 38 , 30 , 22 , 14 ,  6 ,
    64 , 56 , 48 , 40 , 32 , 24 , 16 ,  8 ,
    57 , 49 , 41 , 33 , 25 , 17 ,  9 ,  1 ,
    59 , 51 , 43 , 35 , 27 , 19 , 11 ,  3 ,
    61 , 53 , 45 , 37 , 29 , 21 , 13 ,  5 ,
    63 , 55 , 47 , 39 , 31 , 23 , 15 ,  7};
const static char IPR[64] = {       
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25};
int PCC[48]={                      //置换选择2
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};
int PCK[56]={                    
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4};
int P[32]={                         
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25};
int Extension[48]={                   
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1};
int S_Box[8][4][16]={                
    //S1
    {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
    //S2
    {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
    //S3
    {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
    //S4
    {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
    //S5
    {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
    //S6
    {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
    //S7
    {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
    //S8
    {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};
void BIT_K(int BIT_Key[64],string key)
{
    int j = 0;
    int m = 0;
    char Key_let;
    for(auto k = 0;k != key.size() ;++ k)
    {
        Key_let = key[k];
        while(Key_let)
        {
            BIT_Key[7-j+8*m] = Key_let % 2;
            Key_let /= 2;
            j++;
        }
        // for(i=0;i<8;i++)
        // cout<<BIT_Key[i+8*m]<<" ";
        //cout<<endl;
        m++;
        j=0;
    }
    return;
}
void Tran_K(int Plain[64],char Word[8])
{
    int letter = 0;
    for(int j = 0 ; j < 8 ; j++)
    {
        for(int i=7;i!=0;i--)
            letter+=(int)(Plain[i+8*j]*pow(2,(7-i)));
        Word[j]=letter;
        //cout<<(char)(letter);
        letter=0;
    }
}
void Getkey(int BIT_Key[64],int C_Key[16][48])
{
    int mov,temp,PC1[56];
    for(int i=0;i<56;i++)
    {
        PC1[i]=BIT_Key[PCK[i]-1];
    }
    for(int i=0;i<16;i++)
    {
        if(i==0||i==1||i==8||i==15)
        {mov=1;}
        else
        {mov=2;}
        for(int k=0;k<mov;k++)
        {
            temp=PC1[0];
            for(int m=0;m<27;m++)
                PC1[m]=PC1[m+1];
            PC1[27]=temp;
            temp=PC1[28];
            for(int m=28;m<55;m++)
                PC1[m]=PC1[m+1];
            PC1[55]=temp;
        }
        for(int j=0;j<48;j++)
            C_Key[i][j]=PC1[PCC[j]-1];
    }
    //  for(j=0;j<6;j++)
    //{
    //for(i=0;i<8;i++)
    //	cout<<C_Key[15][i+j*7]<<" ";
    //cout<<endl;
    //}
    
}
void Decode(int BIT_Cipher[64],int C_Key[16][48],int Plain[64])
{
    int Cipher_Ini[64];
    int CR_Ext[48];
    int tmp1[32];
    int RC[32];
    int LC[32];
    int h,l;
    for(int i=0;i<64;i++)
    {
        Cipher_Ini[i]=BIT_Cipher[IP[i]-1];
    }
    for(int i=0;i<16;i++)
    {
        for(int k=0;k<48;k++)
            CR_Ext[k]=Cipher_Ini[32+Extension[k]-1];
        for(int k=0;k<48;k++)
            CR_Ext[k]^=C_Key[15-i][k];
        for(int j=0;j<8;j++)
        {
            h=CR_Ext[j*6]*2+CR_Ext[5+j*6];
            l=CR_Ext[1+j*6]*8+CR_Ext[2+j*6]*4+CR_Ext[3+j*6]*2+CR_Ext[4+j*6];
            tmp1[j*4+3]=S_Box[j][h][l]%2;
            tmp1[j*4+2]=(S_Box[j][h][l]/2)%2;
            tmp1[j*4+1]=(S_Box[j][h][l]/4)%2;
            tmp1[j*4]=(S_Box[j][h][l]/8)%2;
        }
        for(int k=0;k<32;k++)
            RC[k]=tmp1[P[k]-1];
        for(int k=0;k<32;k++)
            LC[k]=Cipher_Ini[k];
        for(int k=0;k<32;k++)
            Cipher_Ini[k]=Cipher_Ini[k+32];
        for(int k=32;k<64;k++)
            Cipher_Ini[k]=LC[k-32]^RC[k-32];
    }
    for(int k=0;k<32;k++)
        LC[k]=Cipher_Ini[k];
    for(int k=0;k<32;k++)
        Cipher_Ini[k]=Cipher_Ini[k+32];
    for(int k=32;k<64;k++)
        Cipher_Ini[k]=LC[k-32];
    for(int i=0;i<64;i++)
        Plain[i]=Cipher_Ini[IPR[i]-1];
}

void Code_File(int BIT_Plain[64],int C_Key[16][48],int Cipher[64])
{
    int Plain_Ini[64];
    int R_Ext[48];
    int tmp[32];
    int RP[32];
    int L[32];
    int h,l;
    for(int i=0;i<64;i++)
        Plain_Ini[i]=BIT_Plain[IP[i]-1];
    for(int i=0;i<16;i++)
    {
        for(int k=0;k<48;k++)
            R_Ext[k]=Plain_Ini[32+Extension[k]-1];
        for(int k=0;k<48;k++)
            R_Ext[k]^=C_Key[i][k];;
        for(int j=0;j<8;j++)
        {
            h=R_Ext[j*6]*2+R_Ext[5+j*6];
            l=R_Ext[1+j*6]*8+R_Ext[2+j*6]*4+R_Ext[3+j*6]*2+R_Ext[4+j*6];
            tmp[j*4+3]=S_Box[j][h][l]%2;
            tmp[j*4+2]=(S_Box[j][h][l]/2)%2;
            tmp[j*4+1]=(S_Box[j][h][l]/4)%2;
            tmp[j*4]=(S_Box[j][h][l]/8)%2;
        }
        for(int k=0;k<32;k++)
            RP[k]=tmp[P[k]-1];
        for(int k=0;k<32;k++)
            L[k]=Plain_Ini[k];
        for(int k=0;k<32;k++)
            Plain_Ini[k]=Plain_Ini[k+32];
        for(int k=32;k<64;k++)
            Plain_Ini[k]=L[k-32]^RP[k-32];
    }
    for(int k=0;k<32;k++)
        L[k]=Plain_Ini[k];
    for(int k=0;k<32;k++)
        Plain_Ini[k]=Plain_Ini[k+32];
    for(int k=32;k<64;k++)
        Plain_Ini[k]=L[k-32];
    for(int i=0;i<64;i++)
        Cipher[i]=Plain_Ini[IPR[i]-1];
}

int main()
{
    string key;
    string plain;
    string cipher;
    string filename;
    
    int Cipher[64];
    int BIT_Key[64]={0};
    int BIT_Plain[64]={0};
    int C_Key[16][48]={0};
    int Plain[64]={0};
    int BIT_Cipher[64]={0};
    char Word[8];
    
    cout << "input 8 digit keys:" << endl;
    cin >> key;
    long dd;
    int f;
    
    int Vain[64]={0};
    char Plain_let;
    auto start_key = chrono::high_resolution_clock::now();
    BIT_K(BIT_Key,key);
    
    Getkey(BIT_Key, C_Key);
    auto end_key = chrono::high_resolution_clock::now();
    cout << "\n\n";
    cout << "KEY Gen EXECUTION TIME = "<<std::chrono::duration_cast<std::chrono::microseconds>(end_key-start_key).count()<<"\n";
    //cout<<C_Key<<endl;
    
    ifstream in("./img/pixel_data.txt");
    string line;
    string C,E;
    long d;
    if(in) 
    {
        while (getline (in, line)) 
        {
            C=C+line;
            C=C+"\n";
        }
    }
    d=C.size();
    while (d%256)
    {
        C=C+" ";
        d=C.size();
    }
    int threads = 8;
    dd = d;
    int block = dd/threads;
    vector<string> seg;
    string tmp = "";
    for (int j = 0; j < threads ;++ j)
    {
        for (int i = 0 ; i < block ;++ i)
        {
            tmp += C[j*block + i];
        }
        seg.push_back(tmp);
	    tmp = "";
    }
    vector <int> cir (dd*8, 0);

    En blocks[threads];
    for (int i = 0 ; i < threads ;++ i)
    {
        // blocks[i].Plain_let = "";
        for (int j = 0 ; j < 64 ;++ j)
        {
            blocks[i].BIT_Plain[i] = 0;
            blocks[i].Cipher[i] = 0;
        }
        // for (int k = 0 ; k < dd ;++ k)
        // {
        //     blocks[i].cir.push_back(0);
        // }
        
    }



    // int i, m;
    int m;
    auto start_en = chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(4)
    for (int pp = 0 ; pp < threads ;++ pp)
    {
	    int q = 0;
        for (int i = 0 ; i < block ;)
        {
            // m = 0
            // q = 0;
            for (int m = 0 ; m < 8 ;++ m)
            {
                blocks[pp].Plain_let = seg[pp][i ++];
                for (int j = 0 ; j < 8 ;++ j)
                {
                    blocks[pp].BIT_Plain[7-j+8*m] = blocks[pp].Plain_let % 2;
                    blocks[pp].Plain_let /= 2;
                }
            }
            Code_File(blocks[pp].BIT_Plain,C_Key,blocks[pp].Cipher);
            for (int k = 0 ; k < 64 ;++ k)
            {
                cir[k+64*q+pp*block*8] = blocks[pp].Cipher[k];
                //  Cipher_F[k+64*q+pp*block*8] = Cipher[k];
            }
		    q++;
        }
    }
    ofstream outf("./img/test1_DES_Encrypt.txt",ios::app);
    for(int k=0 ; k < dd*8 ;++ k)
    {
	//cout<<cipher_o[k]<<endl;
        outf << cir[k];
        // outf<<Cipher_F[k];
    }
    outf.close();
    auto end_en = chrono::high_resolution_clock::now();
    cout << "\n\n";
    cout << "Encode EXECUTION TIME = "<<std::chrono::duration_cast<std::chrono::microseconds>(end_en-start_en).count()<<"\n";
   
    ifstream readf("./img/test1_DES_Encrypt.txt",ios::out);
    
    if(readf) 
    {
        while (getline (readf, line))
        {
            E=E+line;
        }
    }
    
    readf.close();
    // int d_threads = 64;
    // dd = d;
    // int d_block = dd/d_threads;
    // vector<string> d_seg;
    // tmp = "";
    // for (int j = 0; j < threads ;++ j)
    // {
    //     for (int i = 0 ; i < block ;++ i)
    //     {
    //         tmp += E[i];
    //     }
    //     seg.push_back(tmp);
    //     tmp = "";
    // }
    int d_threads = 8;
    int d_block = dd * 8 / d_threads;
    De d_blocks[d_threads];
    for (int j = 0 ; j < d_threads ;++ j)
    {
        for (int i = 0 ; i < 64 ;++ i)
        {
            d_blocks[j].Plain[i] = 0;
            d_blocks[j].BIT_Cipher[i] = 0;
            // d_blocks[j].BIT_Cipher[i] = "";
        }
    }
    vector <string> d_seg;
    string d_tmp = "";
    for (int i = 0 ; i < d_threads ;++ i)
    {
        for (int j = 0 ; j < d_block ;++ j)
        {
            d_tmp += E[i*d_block + j];
        }
        d_seg.push_back(d_tmp);
        d_tmp = "";
    }
    vector <string> res (dd, "");
    auto start_de = chrono::high_resolution_clock::now();
    
// typedef struct
// {
//     int Plain[64];
//     string BIT_Cipher[64];
//     int Cipher[64];
//     char Word[8];
// }De;
    
    #pragma omp parallel for num_threads(4)
    for ( int i = 0 ; i < d_threads ;++ i)
    {
        int qq = 0;
        for ( int j = 0 ; j < d_block ; )
        {
            for (int k = 0 ; k < 64 ;++ k)
            {
                d_blocks[i].BIT_Cipher[k] = d_seg[i][j++] - '0';
            }
            Decode(d_blocks[i].BIT_Cipher,C_Key,d_blocks[i].Plain);
            Tran_K(d_blocks[i].Plain,d_blocks[i].Word);
            for ( int k = 0 ; k < 8 ;++ k)
            {
                res[k+qq*8+i*d_block/8] = d_blocks[i].Word[k];
            }
            qq ++;
            // D[k+f*8]=Word[k];
        } 

    }
    // for(f=0;f<dd/8;f++)
    // {
    //     for ( int k = 0 ; k < 64 ;++ k)
    //     {
    //         BIT_Cipher[k]=E[k+f*64]-'0';
    //     }
    //     for(k=0;k<64;k++)
    //         BIT_Cipher[k]=E[k+f*64]-'0';
    //     Decode(BIT_Cipher,C_Key,Plain);
    //     Tran_K(Plain,Word);
    //     for(k=0;k<8;k++)
    //         D[k+f*8]=Word[k];
    // }
    ofstream newf("./img/test1_DES_Final.txt",ios::out);
    for(k=0;k<dd;k++)
    {
        // newf<<D[k];
        newf<<res[k];
    }
    newf.close();
    auto end_de = chrono::high_resolution_clock::now();
    cout << "Decode EXECUTION TIME = "<<std::chrono::duration_cast<std::chrono::microseconds>(end_de-start_de).count()<<"\n";
}
