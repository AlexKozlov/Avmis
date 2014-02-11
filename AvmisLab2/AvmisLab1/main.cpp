#include<iostream>
#include "conio.h"
#include <time.h>

using namespace std;

int M = 2;
int K = 2;
#define razmer 4

double* CreateMatrix() 
{
	int size = sizeof(double) * M * M * K * K;

	double* matrix = (double*)_aligned_malloc(size, 16 );

	int y = 6;

    for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for(int k = 0; k < K; k++)
			{
				for(int l = 0; l < K; l++)
				{
					matrix[i * M * K * K + j * K * K + k * K  + l] = y;//rand()%100;//y++
					//cout << "\n ms[" << i * M * K * K + j * K * K + k * K  + l << "] = " << y;
				}
			}			
		}
	}
	//std::cout << "\n";

	

	return matrix;


}

double* MulMatrix(double* matrixA, double* matrixB)
{
	int size = sizeof(double) * M * M * K * K;

	double* result = (double*)_aligned_malloc(size, 16 );

	for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for(int k = 0; k < K; k++)
			{
				for(int l = 0; l < K; l++)
				{
					result[i * M * K * K + j * K * K + k * K  + l] = 0;
				}
			}
		}
	}

   clock_t start, end;
   
	start = clock();
    
	for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for (int inner1 = 0; inner1 < M; inner1++)
			{
				for(int k = 0; k < K; k++)
				{
					for(int l = 0; l < K; l++)
					{	
						for (int inner2 = 0; inner2 < K; inner2++)
						{
						   
							int r = i * M * K * K + j * K * K + k * K + l;
							int a = i * M * K * K + (inner1) * K * K + k * K + (inner2);
							int b = (inner1) * M * K * K + j * K * K + (inner2) * K + l;

							result[r] += matrixA[a] * matrixB[b];
						
						}
					}
				}
			}
		}
	}

	end = clock();
 
    //printf("The above code block was executed in %.4f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));

	//std::cout << "\n";

	return result;
}

double* MulMatrixASM(double* matrixA, double* matrixB)
{
	int koofA = 0;
	int koofB = 0;
	int koofC = 0;

	int size = sizeof(double) * M * M * K * K;

	double* result = (double*)_aligned_malloc(size, 16 );

	for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for(int k = 0; k < K; k++)
			{
				for(int l = 0; l < K; l++)
				{
					result[i * M * K * K + j * K * K + k * K  + l] = 0;
				}
			}
		}
	}

   //clock_t start, end;
   
	//start = clock();
    


	for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for (int inner1 = 0; inner1 < M; inner1++)
			{
				//float mas[4] = {2,2,2,2};

				//float gag = *mas; 

				int gdg = 0;

				_asm
				{
					mov ecx,0

					mov eax,0
					push eax

					//for(int k = 0)
					loop1:
					pop eax
					cmp eax,K
					je loop1End
					push eax
					//for(int k = 0)
						
						mov eax,0
						push eax

						//for(int l = 0)
						loop2:
						pop eax
						cmp eax,K
						je loop2End
						push eax
						//for(int l = 0)					

							mov eax,0
							push eax

							//for(int INNER2 = 0)
							loop3:
							pop eax
							cmp eax,K
							je loop3End
							push eax
							//for(int INNER2 = 0)
							///////////////////////////////////////////////////////////

							// koofA ----------------------------------------------
							mov eax,i
							imul eax,M
							imul eax,K
							imul eax,K

							mov ebx,eax

							mov eax,j
							imul eax,K
							imul eax,K 

							add ebx,eax

							mov eax,K

							pop ecx
							imul eax,ecx
							pop edx

							add ebx,eax
							add ebx,edx

							push edx
							push ecx

							mov koofA,ebx
							/// koofA  -------------------------------------------


							/// koofB  -------------------------------------------
							mov eax,i
							imul eax,M
							imul eax,K
							imul eax,K

							mov ebx,eax

							mov eax,inner1
							imul eax,K
							imul eax,K 

							add ebx,eax

							mov eax,K

							pop ecx
							imul eax,ecx
							add ebx,eax
							
							pop edx
							pop eax
							
							add ebx,eax

							push eax
							push edx
							push ecx

							mov koofB,ebx
							/// koofB  -------------------------------------------

							/// koofC  -------------------------------------------
							mov eax,inner1
							imul eax,M
							imul eax,K
							imul eax,K

							mov ebx,eax

							mov eax,j
							imul eax,K
							imul eax,K 

							add ebx,eax

							pop eax
							pop ecx
							pop edx

							push ecx
							mov ecx,K
							imul ecx,edx
							add ebx,ecx

							pop ecx
							add ebx,ecx

							push edx
							push ecx
							push eax

							mov koofC,ebx
							/// koofC  -------------------------------------------

							//mov eax,dword ptr [m1] 

							mov esi,matrixA
							
							mov eax,dword ptr [esi] 
							mov gdg,eax
							//mov    edx,dword ptr [matrixB]  
							//mov    ebx,dword ptr [result] 

							//movupd	xmm0,mmword ptr [eax]
							//movupd	xmm1,mmword ptr [edx]
							//mulpd		xmm0,xmm1
							//movupd	xmm1,mmword ptr [ebx]
							//addpd		xmm0,xmm1
							//movupd	mmword ptr [ebx],xmm0


							///////////////////////////////////////////////////////////
							//for(int INNER2 = 0)
							pop eax
							add eax,1
							push eax
							jmp loop3
							loop3End:
							//for(int INNER2 = 0)

						//for(int l = 0)
						pop eax
						add eax,1
						push eax
						jmp loop2
						loop2End:
						//for(int l = 0)

					//for(int k = 0)
					pop eax
					add eax,1
					push eax
					jmp loop1
					loop1End:
					//for(int k = 0)

					 
					//mov dd,ecx
				}
				cout << "\n gdg: " << gdg;
				//getch();
				
				//for(int f = 0; f < K; f++)
				//{
				//	for(int l = 0; l < K; l++)
				//	{	
				//		for (int inner2 = 0; inner2 < K; inner2++)
				//		{
				//		   
				//			int r = i * M * K * K + j * K * K + f * K + l;
				//			int a = i * M * K * K + (inner1) * K * K + f * K + (inner2);
				//			int b = (inner1) * M * K * K + j * K * K + (inner2) * K + l;
				//			result[r] += matrixA[a] * matrixB[b];
				//		
				//		}
				//	}
				//}
			}
		}
	}
	cout << "\n KoofA: " << koofA;
	cout << "\n KoofB: " << koofB;
	cout << "\n KoofC: " << koofC;

	//end = clock();
 
    //printf("The above code block was executed in %.4f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));

	//std::cout << "\n";

	return result;
}




//void MulAsm(double m0[razmer],double m1[razmer],double result[razmer])
//{	
//	_asm
//	{
//		//mov eax,dword ptr [m1] 
//		mov    eax,dword ptr [m1]    
//		mov    edx,dword ptr [m0]  
//		mov    ebx,dword ptr [result] 
//
//		movupd	xmm0,mmword ptr [eax]
//		movupd	xmm1,mmword ptr [edx]
//		mulpd	xmm0,xmm1
//		//movupd	xmm1,mmword ptr [ebx]
//		//addpd	xmm0,xmm1
//		movupd	mmword ptr [ebx],xmm0
//	}
//}

int main()
{
	int size = sizeof(double) * M * M * K * K;

	double* matrixA;
	double* matrixB;

	int ggwp = 0;

	matrixA = (double*)_aligned_malloc(size, 16 );
	matrixB = (double*)_aligned_malloc(size, 16 );
	
	matrixA = CreateMatrix();	
	matrixB = CreateMatrix();

	//MulMatrix(matrixA,matrixB);
	//double a[razmer] = {2.0,1.0,2.0,1.0};
	//double b[razmer] = {2.0,1.0,2.0,1.0};
	//double c[razmer] = {0.0,0.0,0.0,0.0};
	//double a = 10;
	//MulAsm(a,b,c);

	MulMatrixASM(matrixA,matrixB);

//	int a = 0;
//
//	_asm
//	{
//		mov ax,10
//		mov ebx,a
//
//		loop1:
//		cmp ax,0
//		je loop1End
//
//		add ebx,2
//
//
//		sub ax,1
//		jmp loop1
//loop1End:
//
//		mov a,ebx
//					
//	}
//
//	cout << "\n lolka teper: " << a;

	//for(int i = 0;i<4;i++)
//	cout<<"\nHi: "<<c[i];

		
	//for(int i = 0;i<razmer;i++)
	//{
	//	cout<<"\nHi: "<<c[i];
	//}

	getch();
	return 0;
}

 
