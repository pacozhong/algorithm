#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void HeapAdjust(int array[], int i, int nLength)
{
	int nChild;
	int nTemp;
	for (nTemp = array[i]; 2 * i + 1 < nLength; i = nChild)
	{
		// 子结点的位置=2*（父结点位置）+ 1
		nChild = 2 * i + 1;
		// 得到子结点中较大的结点
		if ( nChild < nLength-1 && array[nChild + 1] > array[nChild])
			++nChild;
		// 如果较大的子结点大于父结点那么把较大的子结点往上移动，替换它的父结点
		if (nTemp < array[nChild])
		{
			array[i] = array[nChild];
			array[nChild]= nTemp;
		}
		else
			// 否则退出循环
			break;
	}
}

// 堆排序算法
void HeapSort(int array[],int length)
{  
	int tmp;
	// 调整序列的前半部分元素，调整完之后第一个元素是序列的最大的元素
	//length/2-1是第一个非叶节点，此处"/"为整除
	int i = 0, j = 0;
	for (i = (length -1)/ 2 ; i >= 0; --i)
		HeapAdjust(array, i, length);
	// 从最后一个元素开始对序列进行调整，不断的缩小调整的范围直到第一个元素
	for(i = 0; i < length; i ++){printf("%d\t", array[i]);}
	printf("\n");
	for (i = length - 1; i > 0; --i)
	{
	for(j = 0; j < length; j ++){printf("%d\t", array[j]);}
	printf("\n");
		// 把第一个元素和当前的最后一个元素交换，
		// 保证当前的最后一个位置的元素都是在现在的这个序列之中最大的
		///  Swap(&array[0], &array[i]);
		tmp = array[i];
		array[i] = array[0];
		array[0] = tmp;
	for(j = 0; j < length; j ++){printf("%d\t", array[j]);}
	printf("\n");
		// 不断缩小调整heap的范围，每一次调整完毕保证第一个元素是当前序列的最大值
		HeapAdjust(array, 0, i);
	}
}

int
main(int argc, char * argv[]){

	int n = 0;
	scanf("%d", &n);
	int * data = (int*)malloc(sizeof(int) * n);
	int i = 0;
	int tmp = 0;
	while(i < n && scanf("%d", &tmp)){
		*(data + i) = tmp;
		i ++;
	}
	for(i = 0; i < n; i ++){
		printf("%d\t", *(data + i));
	}
	printf("\n");
	
	HeapSort(data, n);
	

	for(i = 0; i < n; i ++){
		printf("%d\t", *(data + i));
	}
	printf("\n");

}
