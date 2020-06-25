/***************************************************************************
 *   Copyright (C) 2008 by pilar   *
 *   pilarb@unex.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/***************************************************************************
 * Implementacion de los metodos de imageprocess.h
 *
 * @version 1.3
 * @author
 * Samuel Moreno Vincent
 *
 * Asignatura Administración Y Organización De Computadores
 * Curso 14/15
 ***************************************************************************/
#include "imageprocess.h"

void imageprocess::copiar(uchar * imgO, uchar * imgD)
{
    asm volatile(
        "movl %0, %%esi \n\t"           //esi = img0
        "movl %1, %%edi \n\t"           //edi = imgD
        "movl $320*240, %%ecx \n\t"
    "bcopia: \n\t"
        "movb (%%esi), %%al \n\t"       //El char ocupa un bite
                                        //asi que no necesitamos poner el 4
        "movb %%al, (%%edi) \n\t"
        "incl %%esi \n\t"
        "incl %%edi \n\t"
        "loop bcopia \n\t"

//
        :
        : "m" (imgO), "m" (imgD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"

    );


}

void imageprocess::borrar(uchar * imgD)
{
    asm volatile(
        "movl %0, %%esi         \n\t"   //esi = img0
        "movl $320*240, %%ecx   \n\t"
    "bborrar:                   \n\t"

        "movb $0, (%%esi)       \n\t"

        "incl %%esi             \n\t"
        "loop bborrar           \n\t"

        :
        : "m" (imgD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );


}

void imageprocess::cambiarContrasteFCuadrada(uchar * imgO, uchar * imgD)
{

    asm volatile(
        "movl %0, %%esi         \n\t"   //dirOrig = imgO;
        "movl %1, %%edi         \n\t"   //dirDest = imgD;
        "movl $320*240, %%ecx   \n\t"   //ecx = contador bucle p

    "BucleCCFC:                 \n\t"   //Para(p=0, p<320*240; p++){

        "movb (%%esi), %%al     \n\t"   //g(eax) = [dirOrig];
                                        //[dirDest] = (g*g)/255;
        "mul %%al               \n\t"   //eax = eax * eax ((imul XY -> eax = eax * XY))
                                        //>>>>> multiplica lo que le das por %%al y mete el resultado en ax
        "movb $255, %%bl        \n\t"
        "div %%bl               \n\t"   //queda en al el cosciente y en ah el resto

        "movb %%al, (%%edi)    \n\t"



        "inc %%esi              \n\t"   //dirOrig++;
        "inc %%edi              \n\t"   //dirDest++;

        "loop BucleCCFC         \n\t"
        :
        : "m" (imgO),   "m" (imgD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::cambiarContrasteFInversa(uchar * imgO, uchar * imgD)
{
    asm volatile(
        "movl %0, %%esi         \n\t"   //dirOrig = imgO;
        "movl %1, %%edi         \n\t"   //dirDest = imgD;
        "movl $320*240, %%ecx   \n\t"   //ecx = contador bucle p

    "BucleCCFI:                 \n\t"   //Para(p=0, p<320*240; p++){

        //"movl (%%esi), %%eax    \n\t"   //g(eax) = [dirOrig];

                                        //[dirDest] = 255-g;
        //"movl $255, %%ebx       \n\t"
        //"subl %%eax, %%ebx      \n\t"
        //"movl %%ebx, (%%edi)    \n\t"



        "movb (%%esi), %%al    \n\t"   //g(eax) = [dirOrig];

                                        //[dirDest] = 255-g;
        "movb $255, %%bl       \n\t"
        "subb %%al, %%bl      \n\t"
        "movb %%bl, (%%edi)    \n\t"




        "inc %%esi              \n\t"   //dirOrig++;
        "inc %%edi              \n\t"   //dirDest++;

        "loop BucleCCFI         \n\t"

        :
        : "m" (imgO),   "m" (imgD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::cambiarEscalaGrises(uchar * imgO, uchar * imgD, uchar minO, uchar maxO, uchar minD, uchar maxD)
{    //                                        %0            %1          %2          %3          %4          %5
    uchar rangoO; //%6
    uchar rangoD; //%7
    asm volatile(
        "movl %0, %%esi         \n\t"   //dirOrig = imgO;
        "movl %1, %%edi         \n\t"   //dirDest = imgD;
        "movl $320*240, %%ecx   \n\t"   //ecx = contador bucle p

        //subl Fuente, Destino // Destino ← Destino - Fuente
        //rangoO = maxO – minO;
        "movb %3, %%al         \n\t"
        "sub %2, %%al         \n\t"
        "movb %%al, %6         \n\t"

         //rangoD = maxD – minD;
        "movb %5, %%al         \n\t"
        "sub %4, %%al         \n\t"
        "movb %%al, %7         \n\t"

    "BucleCEG:                  \n\t"   //Para(p=0, p<320*240; p++){

        "movb (%%esi), %%al     \n\t"   //g(eax) = [dirOrig];

                                        //[dirDest] = ((g – minO)*rangoD)/rangoO + minD;

        "sub %2, %%al           \n\t"
        "movb %7, %%bl          \n\t"
        "mul %%bl               \n\t"
        "movb %6, %%bl          \n\t"
        "div %%bl               \n\t"
        "addb %4, %%al          \n\t"


        "movb %%al, (%%edi)    \n\t"


        "inc %%esi              \n\t"   //dirOrig++;
        "inc %%edi              \n\t"   //dirDest++;

        "loop BucleCEG        \n\t"

        :
        : "m" (imgO), "m" (imgD), "m" (minO), "m" (maxO), "m" (minD), "m" (maxD), "m" (rangoO), "m" (rangoD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::umbralizar(uchar * imgO, uchar * imgD, uchar uMax, uchar uMin)
{                                   //  %0            %1          %2          %3
    asm volatile(
        "movl %0, %%esi         \n\t"   //dirOrig = imgO;
        "movl %1, %%edi         \n\t"   //dirDest = imgD;
        "movl $320*240, %%ecx   \n\t"   //ecx = contador bucle p

    "BucleForU:                 \n\t"   //Para(p=0, p<320*240; p++){

                                        //Si([dirOrig]>uMax)[dirDest]=255;
        "movb (%%esi), %%al     \n\t"   //al = [dirOrig]
        "movb %2, %%bl          \n\t"   //bl = uMax
        "cmpb %%bl, %%al        \n\t"
        "jb Sino1U              \n\t"
        "movb $255, (%%edi)     \n\t"
        "jmp FinU               \n\t"

    "Sino1U:                    \n\t"

                                        //Si([dirOrig]<uMin)[dirDest]=0;
        "movb %3, %%bl          \n\t"
        "cmpb %%bl, %%al        \n\t"
        "ja Sino2U              \n\t"
        "movb $0, (%%edi)       \n\t"
        "jmp FinU               \n\t"

    "Sino2U:                    \n\t"   //[dirDest]=127;
        "movb $127, (%%edi)     \n\t"


    "FinU:                      \n\t"


        "inc %%esi              \n\t"   //dirOrig++;
        "inc %%edi              \n\t"   //dirDest++;

        "loop BucleForU         \n\t"

        :
        : "m" (imgO), "m" (imgD), "m" (uMax), "m" (uMin)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"

);


}

void imageprocess::filtroLineal(uchar * imgO, int * kernel, int norm, uchar * imgD)
{//                                       %0            %1        %2            %3
    int f;          //%4
    int c;          //%5
    asm volatile(

        "movl %0, %%esi         \n\t"   //dirOrig = imgO;
        "movl %3, %%edi         \n\t"   //dirDest = imgD;
        "movl %1, %%ebx         \n\t"   //ebx = kernel
        "movl $0, %4            \n\t"   //filas f
        "movl $0, %5            \n\t"   //columnas c

    "ForFL1:                    \n\t"   //f < 40
        "movl $0, %5            \n\t"
    "ForFL2:                    \n\t"   //c < 320
        "movl %0, %%eax         \n\t"
        "movl %0, %%edx         \n\t"
//________________________________________________________________________
        "movl $0, %%ecx         \n\t"   //acumulador = 0
    "SiFL0:                     \n\t"   //si0(f>0)
        "movl %4, %%edx         \n\t"
        "cmpl $0, %%edx         \n\t"
        "jle FinSiFL0           \n\t"

    "SiFL1:                     \n\t"   //si1(c>0)
        "movl %5, %%edx         \n\t"
        "cmpl $0, %%edx         \n\t"
        "jle FinSiFL1           \n\t"

                                        //acum = acum + [dirOrig – 321]*[dirKernel];
        "movb -321(%%esi), %%al \n\t"   //al = [dirOrig – 321]
        "movl (%%ebx), %%edx    \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"

    "FinSiFL1:                  \n\t"
                                        //acum = acum + [dirOrig – 320]*[dirKernel + 4];
        "movb -320(%%esi), %%al \n\t"
        "movl 4(%%ebx), %%edx   \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"

    "SiFL2:                     \n\t"   //Si2(c<319)
        "movl %5, %%edx         \n\t"
        "cmpl $319, %%edx       \n\t"
        "jge FinSiFL2           \n\t"

        "movb -319(%%esi), %%al \n\t"
        "movl 8(%%ebx), %%edx   \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t" // no se si esto es asi
    "FinSiFL2:                  \n\t"
    "FinSiFL0:                  \n\t"

    "SiFL3:                     \n\t"   //Si3(c>0)
        "movl %5, %%edx         \n\t"
        "cmpl $0, %%edx         \n\t"
        "jle FinSiFL3           \n\t"
                                        //acum = acum + [dirOrig – 1]*[dirKernel + 12];
        "movb -1(%%esi), %%al   \n\t"
        "movl 12(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"

    "FinSiFL3:                  \n\t"
                                        //acum = acum + [dirOrig]*[dirKernel + 16];
        "movb (%%esi), %%al     \n\t"
        "movl 16(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"
    "SiFL4:                     \n\t"   //Si3(c<319)
        "movl %5, %%edx         \n\t"
        "cmpl $319, %%edx       \n\t"
        "jge FinSiFL4           \n\t"
                                        //acum = acum + [dirOrig + 1]*[dirKernel + 20];
        "movb 1(%%esi), %%al    \n\t"
        "movl 20(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"
    "FinSiFL4:                  \n\t"
    "SiFL5:                     \n\t"   //Si5(f<239)
        "movl %4, %%edx         \n\t"
        "cmpl $239, %%edx       \n\t"
        "jge FinSiFL5           \n\t"
    "SiFL6:                     \n\t"   //Si6(c>0)
        "movl %5, %%edx         \n\t"
        "cmpl $0, %%edx         \n\t"
        "jle FinSiFL6           \n\t"
                                        //acum = acum + [dirOrig + 319]*[dirKernel + 24];
        "movb 319(%%esi), %%al  \n\t"
        "movl 24(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"

    "FinSiFL6:                  \n\t"
                                        //acum = acum + [dirOrig + 320]*[ dirKernel + 28];
        "movb 320(%%esi), %%al  \n\t"
        "movl 28(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"
    "SiFL7:                     \n\t"   //Si7(c<319)
        "movl %5, %%edx         \n\t"
        "cmpl $319, %%edx       \n\t"
        "jge FinSiFL7           \n\t"
                                        //acum = acum + [dirOrig + 321]*[dirKernel + 32];
        "movb 321(%%esi), %%al  \n\t"
        "movl 32(%%ebx), %%edx  \n\t"
        "movzbl %%al, %%eax     \n\t"
        "imull %%edx, %%eax     \n\t"
        "addl %%eax, %%ecx      \n\t"

    "FinSiFL7:                  \n\t"
    "FinSiFL5:                  \n\t"
//________________________________________________________________________
                                        //acum=acum/norm
        "movl $0, %%eax         \n\t"
        "movl %%ecx, %%eax      \n\t"
        "cdq                    \n\t"
        "idivl %2               \n\t"
        "movl %%eax, %%ecx      \n\t"
    "SiFL8:                     \n\t"   //Si8(acum<0)
        "cmpl $0, %%ecx         \n\t"
        "jge FinSiFL8           \n\t"
        "movl $0, %%ecx         \n\t"   //acum = 0;
    "FinSiFL8:                  \n\t"

    "SiFL9:                     \n\t"   //Si9(acum>255)
        "cmpl $255, %%ecx       \n\t"
        "jle FinSiFL9           \n\t"
        "movl $255, %%ecx       \n\t"   //acum = 255;
    "FinSiFL9:                  \n\t"

        "movl %%ecx, (%%edi)    \n\t"   //[dirDest] = acum;

        "inc %%esi              \n\t"   //dirOrig++;
        "inc %%edi              \n\t"   //dirDest++;
//________________________________________________________________________
        "incl %5                \n\t"
        "cmpl $320, %5          \n\t"
        "jl ForFL2              \n\t"

        "incl %4                \n\t"
        "cmpl $240, %4          \n\t"
        "jl ForFL1              \n\t"

        :
        : "m" (imgO), "m" (kernel), "m"(norm), "m" (imgD), "m" (f), "m" (c)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::filtroMediana(uchar * imgO, uchar * imgD)
{//                                       %0            %1
    uchar e[10];          //%2
    int f;              //%3
    int c;              //%4
    asm volatile(
                "\n\t"

        :
        : "m" (imgO),   "m" (imgD), "m" (e),"m" (f), "m" (c)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::ecualizarHistograma(int * histoOrig, uchar * tablaLUT)
{
    asm volatile(
        "\n\t"

        :
        : "m" (histoOrig), "m" (tablaLUT)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}

void imageprocess::aplicarTablaLUT(uchar * imgO, uchar * tablaLUT, uchar * imgD)
{
    asm volatile(
        "\n\t"

        :
        : "m" (imgO), "m" (tablaLUT), "m" (imgD)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
    );

}
