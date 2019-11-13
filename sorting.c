#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE ( sizeof(list)/sizeof(*list))

int list[] = {7,12,19,3,18,4,2,-5,6,15,8};      // unsorted array
int result[SIZE] = {0};                         // same content but sorted

typedef struct{
  int* subArray;                                // SortingThreadParameters has subArray and size
  unsigned int size;
} SortingThreadParameters;

typedef struct{
  SortingThreadParameters left;                 // left array and right array
  SortingThreadParameters right;
} MergingThreadParameters;

// used bubleSort to sort the first two arrays
void bubbleSort(int list[], int size);
// mergeSort will hold the first array and size, second array and size and full array with all of them
void mergeSort(int firstArray[], int firstSize, int secondArray[], int secondSize, int mergeArray[]);

// creating the helper function was a must inorder to pass void function
void merge(MergingThreadParameters* pass);
void firstTwoSort(SortingThreadParameters* pass);

// display the array
void displayArray(int list[], int size);

int main(int argc, char* argv[]){
  pthread_t tid[3]; // thread called tid and we have three thread together

  SortingThreadParameters * paramsLeft = malloc( sizeof(SortingThreadParameters));
  paramsLeft -> subArray = list;
  paramsLeft -> size = SIZE/2;

  // create first sorting thread passing it paramsLeft as a parameter
  pthread_create(&tid[0], 0, firstTwoSort, paramsLeft);

  SortingThreadParameters * paramsRight = malloc( sizeof(SortingThreadParameters));
  paramsRight -> subArray = list + paramsLeft -> size;
  paramsRight -> size = SIZE - paramsLeft-> size;

  // create second sorting thread passing it paramsRight as a parameter
  pthread_create(&tid[1], 0, firstTwoSort, paramsRight);

  MergingThreadParameters * paramsMerge = malloc( sizeof(MergingThreadParameters));
  paramsMerge -> left = *paramsLeft;
  paramsMerge -> right = *paramsRight;

  // since we have odd number of array we need to makesure that onethread finishes
  // and wait for the second thread to finish to make sure all thread gets the values
  // in order to perform this we used forloop

  for(int i =0; i <2; i++){
    pthread_join(tid[i],0);
  }
  // create merging thread passing it paramMerge as a parameter
  pthread_create(&tid[2], 0, merge, paramsMerge);

  for(int i =0; i <3; i++){
    pthread_join(tid[i],0);
  }
  displayArray(result, SIZE);
  return 0;
}
void bubbleSort(int list[], int size){
  for(int i=0; i <size; i++){ // iterate through the whole array
    for(int j=0; j <size-i-1; j++){
      if(list[j] > list[j+1]){ // compare first and second element if first great then swap
        int hold = list[j];
        list[j] = list[j+1]; // hold will be used to hold the other value so it does not disappear
        list[j+1] = hold;
      }
    }
  }
}
// This sorting takes subarray of left and right according to the parameters passed
void firstTwoSort(SortingThreadParameters* pass){
  bubbleSort(pass->subArray, pass->size);
  pthread_exit(0);
}
// merge helping function takes the variables from all array left and right
void merge(MergingThreadParameters * pass){
  mergeSort(pass-> left.subArray, pass->left.size, pass->right.subArray, pass->right.size, result);
  pthread_exit(0);

}
// merge Sort will compare the first array and second array one by one and takes the most leftmost variables
void mergeSort(int firstArray[], int firstSize, int secondArray[], int secondSize, int mergeArray[]){
  int i =0; // index for firstArray
  int j =0; // index for secondArray
  int k =0; // index for mergeArray
  while( (i < firstSize) && (j < secondSize))
  {
    if(firstArray[i] < secondArray[j])
      mergeArray[k++] = firstArray[i++];
    else
        mergeArray[k++] = secondArray[j++];
  }
  while(i < firstSize) mergeArray[k++] = firstArray[i++];
  while(j < secondSize) mergeArray[k++] = secondArray[j++];
}
void displayArray(int list[], int size){
  for(int i =0; i <size; i++){
    printf("%d ", list[i]);
  }
}
