/* pi.cl */
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef unsigned long long ull;


ull generateKeyPlus(ull *d_iniKey, __global int *d_PC1){
    ull keyPlus=0L;
    for(int i=56-1;i>=0;i--){
        if( d_iniKey[ d_PC1[i]/8 ] & (1 << ( ( 64-d_PC1[i]) % 8 ) ) ){
            keyPlus|=( 1L<< (55-i)*1L );
        }
    }
    return keyPlus;
}

ull* splitKeyPlus(ull keyPlus){
    ull c0=0L, d0=0L;

    for(int i=0;i<28;i++){
        if(keyPlus & (1L<<i*1L ) ) d0|=(1L<<i*1L);
        if(keyPlus & (1L<< i*1L +28L ) ) c0|=(1L<<i*1L);
    }
    ull new_array[2]= {c0,d0};
    return new_array;
}

ull* splitIniPer(ull codeIniPer){
    ull l0=0L, r0=0L;

    for(int i=0;i<32;i++){
        if(codeIniPer & (1L<<i*1L ) ) r0|=(1L<<i*1L);
        if(codeIniPer & (1L<< i*1L +32L ) ) l0|=(1L<<i*1L);
    }
    ull new_array[2] ={ l0,r0};
    return new_array;
}

void generateCnDnBlocks( ull *seedKey,__global ull *d_CnDnBlocks, __global ull *d_Rotations){
    d_CnDnBlocks[0] = seedKey[0];
    d_CnDnBlocks[1] = seedKey[1];
    ull cn ,dn, newCn, newDn;
    ull getOnCn, getOnDn;

    for(int i=1;i<=16;i++){
        
        getOnCn = 0L;
        getOnDn = 0L;

        cn = d_CnDnBlocks[(i*2+0)-2];
        dn = d_CnDnBlocks[(i*2+1)-2];

        for(ull j=0;j< d_Rotations[ i-1 ];j++){

            if(  cn & (1 << (27-j) )  ) getOnCn|= 1L << (d_Rotations[ i-1 ]==1 ? j: 1-j) ;
            if(  dn & (1 << (27-j) )  ) getOnDn|= 1L << (d_Rotations[ i-1 ]==1 ? j: 1-j);

        }

        newCn = cn << d_Rotations[ i-1 ];
        newDn = dn << d_Rotations[ i-1 ];

        for(ull j=0; j< d_Rotations[ i-1 ] ;j++){
            newCn &= ~(1<< (28+j) );
            newDn &= ~(1<< (28+j) );
        }
        newCn |= ( getOnCn );
        newDn |= ( getOnDn );
        d_CnDnBlocks[ (i*2)+0 ] = newCn;
        d_CnDnBlocks[ (i*2)+1 ] = newDn;
    }

}

ull joinCnDn(ull cn, ull dn){ return (cn<<28) | dn; }

void generateKeysBlocks(ull *d_CnDnBlocks,__global int *d_PC2,__global ull *d_keysBlocks){
    ull cnDn, keyn;

    for(int i=1;i<=16;i++){
        cnDn = joinCnDn( d_CnDnBlocks[i*2+0], d_CnDnBlocks[i*2+1] );
        keyn = 0L;

        for(int j=48-1;j>=0;j--){
            if( cnDn & ( 1L << (56-d_PC2[j])*1L ) ) {
                 keyn|= ( 1L<< (47-j)*1L );
            }
        }
        d_keysBlocks[i-1] = keyn;
    }

}

ull generateIniPer(__global int *d_IniPer, __global ull *d_message){
    ull keyPlus=0L;

    for(int i=64-1;i>=0;i--){
        if( d_message[ (d_IniPer[i]/8) >=8 ? 7: (d_IniPer[i]/8) ]  & (1L << ( ( 64-d_IniPer[i]) % 8 ) ) ){
            keyPlus|=( 1L<< (63-i)*1L );
        }
    }
    
    return keyPlus;
}

ull expandRn(ull Rn,__global int *d_Expansion){
    //from a Rn 32 bit to a Kn 48 bit
    ull exRn=0L;
    for(int j=48-1;j>=0;j--){
            if( Rn & ( 1L << (32-d_Expansion[j])*1L ) ) {
                 exRn|= ( 1L<< (47-j)*1L );
            }
    }
    return exRn;
}

ull xorOperation(ull En, ull Kn){
    return (Kn ^ En);
}


ull getSboxNumber(int Bn, int k, __global int *d_Sbox){

    int row=0,col=0;
    if( Bn & 1<<0 ) row |= ( 1<<0);
    if( Bn & 1<<6-1) row |=( 1<<1 );

    for(int i=1;i<=4;i++){
        if( Bn & 1<<i ) col |=(1<<(i-1));
    }

    return d_Sbox[ (k*4*16)+(row*16)+col ];
}


ull generateSboxCombination(ull Bn,__global int *d_Sbox){

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
        if( Bn & (1L<<i*1L) ){
            number |= (1L<< ( (i%6)*1L ) );
        }
    }

    for(int i=0;i<8;i++){
        sbBox[i] = getSboxNumber( Bbox[i], i,d_Sbox);

       
    }

    
    for(int i=0;i<8;i++){
        snBnChain |= (sbBox[i]<< step);
        step-=4;
    }

    return snBnChain;

}

ull generateFalgorithm(ull snBn, __global int *d_Pbox){

    ull fn=0L;
    for(int j=32-1;j>=0;j--){
            if( snBn & ( 1L << (32-d_Pbox[j])*1L ) ) {
                 fn|= ( 1L<< (31-j)*1L );
            }
    }
    return fn;
}

void generateLnRnBlocks(ull *L0R0,__global ull *d_LnRnBlocks, __global ull *d_keysBlocks, __global int *d_Expansion,__global int *d_Sbox,__global int *d_Pbox){

    d_LnRnBlocks[0] = L0R0[0];
    d_LnRnBlocks[1] = L0R0[1];
    ull fn;

    for(int time=1; time<=16;time++){

        ull Ln = d_LnRnBlocks[ (time*2+0)-2 ];
        ull Rn = d_LnRnBlocks[ (time*2+1)-2 ];
       
        ull snBn = 
            generateSboxCombination( xorOperation( expandRn( Rn, d_Expansion ),d_keysBlocks[ time-1 ] ),d_Sbox );
        
        fn = generateFalgorithm(snBn,d_Pbox);

        d_LnRnBlocks[ (time*2+0) ] = Rn;
        d_LnRnBlocks[ (time*2+1) ] = (Ln ^ fn);

    }

}

ull reverseLnRn( ull *LnRn, ull *LnRn1){
    ull Ln = *LnRn;
    ull Rn = *LnRn1;

    return ( Rn<<32L) | Ln;
}

ull generateCipherMessage( ull RnLn, __global int *d_reverseIniPer ){

    ull cipher=0L;
    for(int j=64-1;j>=0;j--){
            if( RnLn & ( 1L << (64-d_reverseIniPer[j])*1L ) ) {
                 cipher|= ( 1L<< (63-j)*1L );
            }
    }
    return cipher;
}

__kernel void cipherDES(
    __global ull *d_LnRnBlocks,
    __global ull *d_CnDnBlocks,
    __global ull *d_keysBlocks,
    __global ull *d_Rotations,
    __global int *d_PC1,
    __global int *d_PC2,
    __global int *d_IniPer,
    __global int *d_reverseIniPer,
    __global int *d_Expansion,
    __global int *d_Pbox,
    __global int *d_Sbox,
    __global ull *d_iniKey,
    __global ull *d_message,
    __global ull *d_result)
{   

    int id = get_global_id(0);
    int numthreads = get_global_size(0);
    printf("id: %i - thtreads: %i - other: %i \n",id,numthreads,d_Pbox[0]);
}
