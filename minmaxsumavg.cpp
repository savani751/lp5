#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

//find min max using omp
int findMin(const int* data,int size){
	int minval=data[0];
	#pragma omp parallel for reduction(min: minval)
	{
		for(int i=1;i<size;i++){
			if (data[i]<minval){
				minval=data[i];
			}
			
		}
		return minval;
	}
}

int findMax(const int* data,int size){
	int maxval=data[0];
	#pragma omp parallel for reduction(max: maxval)
	{
		for(int i=1;i<size;i++){
			if (data[i]>maxval){
				maxval=data[i];
			}
			
		}
		return maxval;
	}
}

int calcSum(const int* data,int size){
	int sum=0;
	#pragma omp parallel for reduction(+: sum)
	{
		for (int i=0; i<size; i ++){
			sum+=data[i];
		}
		return sum;
	}
}

int calcAvg(const int* data, int size){

	#pragma omp parallel for reduction(avg)
	{
		int sum=calcSum(data,size);
		int avg=sum/size;
		return avg;
	}
	
}

int main(){
	int data[]={5,4,3,2,1,0};
	int size=sizeof(data)/sizeof(data[0]);
	
	cout<<"Min value from the array is: "<<findMin(data,size)<<endl;
	cout<<"Max value from the array is: "<<findMax(data,size)<<endl;
	cout<<"Sum of elem in array is: "<<calcSum(data,size)<<endl;
	cout<<"Avg of array is: "<<calcAvg(data,size)<<endl;
}
