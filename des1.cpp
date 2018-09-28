#include<bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;
typedef pair< ull , ull>  uull;


uull LnRnBlocks[17]; // from l0r0 to l16r16

uull CnDnBlocks[17]; //from c0d0 to c16d16

ull keysBlocks[16];  //from key[1] = k0 to key[16] = k15

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

const int IniPer[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

const int reverseIniPer[64] ={
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

const int Expansion[48] ={
    32, 1,  2,   3, 4,  5,
    4,  5,  6,   7, 8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

const int Pbox[32] ={
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

const int Sbox[8][4][16] = {
   {
   {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
   { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
   { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
   {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
   },
 
   {
   {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
   { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
   { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
   {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
   },
 
   {
   {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
   {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
   {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
   { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
   },
 
   {
   { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
   {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
   {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
   { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
   },
 
   {
   { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
   {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
   { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
   {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
   },
 
   {
   {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
   {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
   { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
   { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
   },
 
   {
   { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
   {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
   { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
   { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
   },
   {
   {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
   { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
   { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
   { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
   },
};

const ull iniKey[8] = {
    0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};

const ull message[8] = {    
    0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};



ull generateKeyPlus(){
    ull keyPlus=0L;
    int j=0;
    for(int i=56-1;i>=0;i--){
        if( iniKey[ PC1[i]/8 ] & (1 << ( ( 64-PC1[i]) % 8 ) ) ){
            keyPlus|=( 1LL<< j*1L );
        }
        j++;
    }
    return keyPlus;
}

uull splitKeyPlus(ull keyPlus){
    ull c0=0L, d0=0L;

    for(int i=0;i<28;i++){
        if(keyPlus & (1LL<<i*1L ) ) d0|=(1LL<<i*1L);
        if(keyPlus & (1LL<< i*1L +28L ) ) c0|=(1LL<<i*1L);
    }
    return make_pair( c0, d0);
}

uull splitIniPer(ull codeIniPer){
    ull l0=0L, r0=0L;

    for(int i=0;i<32;i++){
        if(codeIniPer & (1LL<<i*1L ) ) r0|=(1LL<<i*1L);
        if(codeIniPer & (1LL<< i*1L +32L ) ) l0|=(1LL<<i*1L);
    }
    return make_pair( l0, r0);
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

            if(  cn & (1 << (27-j) )  ) getOnCn|= 1LL << (Rotations[ i-1 ]==1 ? j: 1-j) ;
            if(  dn & (1 << (27-j) )  ) getOnDn|= 1LL << (Rotations[ i-1 ]==1 ? j: 1-j);

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
            if( cnDn & ( 1LL << (56-PC2[j])*1L ) ) {
                 keyn|= ( 1LL<< k );
            }
            k++;
        }
        keysBlocks[i-1] = keyn;
    }

}

ull generateIniPer(){
    ull keyPlus=0L;
    int j=0;
    for(int i=64-1;i>=0;i--){
        if( message[ (IniPer[i]/8) >=8 ? 7: (IniPer[i]/8) ]  & (1LL << ( ( 64-IniPer[i]) % 8 ) ) ){
            keyPlus|=( 1LL<< j*1L );
        }
        j++;
    }
    
    return keyPlus;
}

ull expandRn(ull Rn){
    //from a Rn 32 bit to a Kn 48 bit
    ull k=0L, exRn=0L;
    for(int j=48-1;j>=0;j--){
            if( Rn & ( 1LL << (32-Expansion[j])*1L ) ) {
                 exRn|= ( 1LL<< k );
            }
            k++;
    }
    return exRn;
}

ull xorOperation(ull En, ull Kn){
    return (Kn ^ En);
}

ull getSboxNumber(int Bn, int k){

    int row=0,col=0;
    if( Bn & 1<<0 ) row |= ( 1<<0);
    if( Bn & 1<<6-1) row |=( 1<<1 );

    for(int i=1;i<=4;i++){
        if( Bn & 1<<i ) col |=(1<<(i-1));
    }

    return ( Sbox[k][row][col]*1LL);
}


ull generateSboxCombination(ull Bn){

    int Bbox[8];
    ull sbBox[8];
    int number=0, k=7;
    ull snBnChain=0L;
    ull step=28L;

    for(int i=0;i<=48;i++ ){

        if( i%6==0 && i>=6){
            

            Bbox[ k-- ] = number;
            number = 0;
        }
        if( Bn & (1LL<<i*1L) ){
            number |= (1LL<< ( (i%6)*1L ) );
        }
    }

    for(int i=0;i<8;i++){
        sbBox[i] = getSboxNumber( Bbox[i], i);

       
    }

    
    for(int i=0;i<8;i++){
        snBnChain |= (sbBox[i]<< step);
        step-=4;
    }

    return snBnChain;

}

ull generateFalgorithm(ull snBn){

    ull k=0L, fn=0L;
    for(int j=32-1;j>=0;j--){
            if( snBn & ( 1LL << (32-Pbox[j])*1L ) ) {
                 fn|= ( 1LL<< k );
            }
            k++;
    }
    return fn;
}

void generateLnRnBlocks(uull L0R0){

    LnRnBlocks[0] = L0R0;
    ull fn;

    for(int time=1; time<=16;time++){

        ull Ln = LnRnBlocks[ time-1 ].first;
        ull Rn = LnRnBlocks[ time-1 ].second;
       
        ull snBn = 
            generateSboxCombination( xorOperation( expandRn( Rn ),keysBlocks[ time-1 ] ) );
        
        fn = generateFalgorithm(snBn);

        uull LnRn = make_pair( Rn, (Ln ^ fn) ); 
        LnRnBlocks[ time ] = LnRn;

    }

}

ull reverseLnRn( uull LnRn){
    ull Ln = LnRn.first;
    ull Rn = LnRn.second;

    return ( Rn<<32L) | Ln;
}

ull generateCipherMessage( ull RnLn ){

    ull k=0L, cipher=0L;
    for(int j=64-1;j>=0;j--){
            if( RnLn & ( 1LL << (64-reverseIniPer[j])*1L ) ) {
                 cipher|= ( 1LL<< k );
            }
            k++;
    }
    return cipher;
}


int main(){

    uull keyHalves = splitKeyPlus( generateKeyPlus() );
    generateCnDnBlocks( keyHalves );
    generateKeysBlocks();
    uull iniPerHalves = splitIniPer(generateIniPer() ); //got L0 and R0
    

    generateLnRnBlocks( iniPerHalves );

    ull revLnRn = reverseLnRn( LnRnBlocks[16] );
   
    ull cipherMessage = generateCipherMessage( revLnRn );
    printf("cipher: %llu\n",cipherMessage);
    fflush(stdout);

    printf("Hex Cipher: %llX\n", cipherMessage);
    fflush(stdout);

    return 0;
}


// L16 01000011010000100011001000110100    1128411700
// R16 00001010010011001101100110010101    172808597

// revL16R16 0000101001001100110110011001010101000011010000100011001000110100  742207273711055412

// cipher 1000010111101000000100110101010000001111000010101011010000000101     9648983453391827973