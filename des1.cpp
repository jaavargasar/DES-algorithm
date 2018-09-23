#include<bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;
typedef pair< ull , ull>  uull;


const ull Rotations[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const int PC1[56] = {
   57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
   10,  2, 59, 51, 43, 35, 27,
   19, 11,  3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
   14,  6, 61, 53, 45, 37, 29,
   21, 13,  5, 28, 20, 12,  4
};

const int PC2[48] = {
   14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
   23, 19, 12,  4, 26,  8,
   16,  7, 27, 20, 13,  2,
   41, 52, 31, 37, 47, 55,
   30, 40, 51, 45, 33, 48,
   44, 49, 39, 56, 34, 53,
   46, 42, 50, 36, 29, 32
};

const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

const unsigned char iniKey[8] = {
    0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};

const unsigned char message[8] = {
    0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};

uull CnDnBlocks[17];

ull keysBlocks[16];

ull generateKeyPlus(){
    ull keyPlus=0L;
    int j=0;
    for(int i=56-1;i>=0;i--){
        if( iniKey[ PC1[i]/8 ] & (1 << ( ( 64-PC1[i]) % 8 ) ) ){
            keyPlus|=( 1L<< j*1L );
        }
        j++;
    }
    return keyPlus;
}

uull splitKeyPlus(ull keyPlus){
    ull c0=0L, d0=0L;

    for(int i=0;i<28;i++){
        if(keyPlus & (1L<<i*1L ) ) d0|=(1L<<i*1L);
        if(keyPlus & (1L<< i*1L +28L ) ) c0|=(1L<<i*1L);
    }
    return make_pair( c0, d0);
}


void generateCnDnBlocks( uull seedKey){
    CnDnBlocks[0] = seedKey;
    ull cn ,dn, newCn, newDn;
    ull getOnCn, getOnDn;

    for(int i=1;i<=16;i++){
        
        getOnCn = 0L;
        getOnDn = 0L;

        cn = CnDnBlocks[i-1].first ;
        dn = CnDnBlocks[i-1].second;

        for(ull j=0;j< Rotations[ i-1 ];j++){

            if(  cn & (1 << (27-j) )  ) getOnCn|= 1L << (Rotations[ i-1 ]==1 ? j: 1-j) ;
            if(  dn & (1 << (27-j) )  ) getOnDn|= 1L << (Rotations[ i-1 ]==1 ? j: 1-j);

        }

        newCn = cn << Rotations[ i-1 ];
        newDn = dn << Rotations[ i-1 ];

        for(ull j=0; j< Rotations[ i-1 ] ;j++){
            newCn &= ~(1<< (28+j) );
            newDn &= ~(1<< (28+j) );
        }
        newCn |= ( getOnCn );
        newDn |= ( getOnDn );
        CnDnBlocks[ i ] = make_pair( newCn, newDn);
    }

}

ull joinCnDn(ull cn, ull dn){ return (cn<<28) | dn; }

void generateKeysBlocks(){
    ull cnDn, keyn;
    ull k;

    for(int i=1;i<=16;i++){
        cnDn = joinCnDn( CnDnBlocks[i].first, CnDnBlocks[i].second );
        keyn = 0L; k=0L;

        for(int j=48-1;j>=0;j--){
            if( cnDn & ( 1L << (56-PC2[j])*1L ) ) {
                 keyn|= ( 1L<< k );
            }
            k++;
        }
        keysBlocks[i-1] = keyn;
    }

}


int main(){

    uull keyHalves = splitKeyPlus( generateKeyPlus() );
    generateCnDnBlocks( keyHalves );
    generateKeysBlocks();
    
    return 0;
}

