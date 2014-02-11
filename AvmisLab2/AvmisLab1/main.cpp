#include<iostream>
#include "conio.h"
#include <time.h>

using namespace std;

int M = 110;
int K = 10;
#define razmer 2
float* CreateMatrix() 
{
	int size = sizeof(float) * M * M * K * K;

	float* matrix = (float*)_aligned_malloc(size, 16 );

	int y = 1;

    for (int i = 0; i < M; i++) 
	{
        for (int j = 0; j < M; j++) 
		{
			for(int k = 0; k < K; k++)
			{
				for(int l = 0; l < K; l++)
				{
					matrix[i * M * K * K + j * K * K + k * K  + l] = y++;//rand()%100;//y++
					//cout << "\n ms[" << i * M * K * K + j * K * K + k * K  + l << "] = " << y;
				}
			}			
		}
	}
	//std::cout << "\n";

	

	return matrix;


}
float* MulMatrix(float* matrixA, float* matrixB)
{
	int size = sizeof(float) * M * M * K * K;

	float* result = (float*)_aligned_malloc(size, 16 );

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
 
    printf("The above code block was executed in %.4f second(s)\n", ((double) end - start) / ((double) CLOCKS_PER_SEC));

	std::cout << "\n";

	return result;
}

void MulAsm(float m0[razmer],float m1[razmer],float result[razmer])
{
	
	_asm
	{
		//mov eax,dword ptr [m1] 
		mov    eax,dword ptr [m1]    
		mov    edx,dword ptr [m0]  
		mov    ebx,dword ptr [result] 

		movupd	xmm0,mmword ptr [eax]
		movupd	xmm1,mmword ptr [edx]
		mulpd	xmm0,xmm1
		//movupd	xmm1,mmword ptr [ebx]
		//addpd	xmm0,xmm1
		movupd	mmword ptr [ebx],xmm0
	}
	
	for(int i = 0;i<razmer;i++)
	cout<<"\nHi: "<<result[i];
	

}

int main()
{
	int size = sizeof(float) * M * M * K * K;

	float* matrixA;
	float* matrixB;

	matrixA = (float*)_aligned_malloc(size, 16 );

	
	matrixA = CreateMatrix();
	matrixB = (float*)_aligned_malloc(size, 16 );
	
	matrixB = CreateMatrix();
	
	
	//MulMatrix(matrixA,matrixB);
	float a[razmer] = {2.0f,1.0f};
	float b[razmer] = {2.0f,1.0f};
	float c[razmer] = {0.0f,0.0f};
	//float a = 10;
	MulAsm(a,b,c);
	//for(int i = 0;i<4;i++)
//	cout<<"\nHi: "<<c[i];



	getch();
	return 0;
}

 
