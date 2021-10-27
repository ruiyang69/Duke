"""
Math 560
Project 1
Fall 2021

Partner 1: Rui Yang
Partner 2: Zhangzhang Yue
Date: 2021-10-22
"""
import random

"""
SelectionSort

Sort the input array using Selection Sort

Parameters
----------
listToSort : list
    an array that need to be sorted

Returns
-------
list
    sorted input array
"""
def SelectionSort(listToSort):
    # extract the length of the array
    n = len(listToSort)

    for i in range(n):
        minIdx = i
        # select the minimum element in the range(i+1,n)
        for j in range(i + 1, n):
            if listToSort[minIdx] > listToSort[j]:
                minIdx = j
        listToSort[i], listToSort[minIdx] = \
            listToSort[minIdx], listToSort[i]

    return listToSort


"""
InsertionSort

Sort the input array using Insertion Sort

Parameters
----------
listToSort : list
    an array that need to be sorted

Returns
-------
list
    sorted input array
"""
def InsertionSort(listToSort):
    # extract the length of the array
    n = len(listToSort)

    for i in range(1, n):
        j = i - 1
        val = listToSort[i]
        # find an appropriate index for `val` in the array
        while j >= 0 and listToSort[j] > val:
            listToSort[j + 1] = listToSort[j]
            j -= 1
        listToSort[j + 1] = val
    return listToSort


"""
BubbleSort

Sort the input array using Bubble Sort

Parameters
----------
listToSort : list
    an array that need to be sorted

Returns
-------
list
    sorted input array
"""
def BubbleSort(listToSort):
    # extract the length of the array
    n = len(listToSort)

    for i in range(n):
        # we add a flag to optimize the performance
        is_fully_sorted = True

        # the last i elements are already sorted
        for j in range(n - i - 1):
            if listToSort[j] > listToSort[j + 1]:
                listToSort[j + 1], listToSort[j] = \
                    listToSort[j], listToSort[j + 1]
                is_fully_sorted = False

        # if no swapping occurs in the current loop,
        # then the list is already sorted, and we simply exit
        if is_fully_sorted:
            break

    return listToSort


"""
MergeSort

Sort the input array using Merge Sort

Parameters
----------
listToSort : list
    an array that need to be sorted

Returns
-------
list
    sorted input array
"""
def MergeSort(listToSort):
    # extract the length of the array
    n = len(listToSort)

    # it the length is less or equal to 1
    # there is no need to sort
    if n <= 1:
        return listToSort

    # divide the original array
    # merge sort through recursion
    mid = n // 2
    left = MergeSort(listToSort[:mid])
    right = MergeSort(listToSort[mid:])

    # merge the left and right arrays by iteration
    index = i = j = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            listToSort[index] = left[i]
            i += 1
        else:
            listToSort[index] = right[j]
            j += 1
        index += 1
    # if there is element left in the left array
    while i < len(left):
        listToSort[index] = left[i]
        i += 1
        index += 1
    # if there is element left in the right array
    while j < len(right):
        listToSort[index] = right[j]
        j += 1
        index += 1

    return listToSort


"""
QuickSort

Sort a list with the call QuickSort(listToSort),
or additionally specify i and j.

Parameters
----------
listToSort : list
    an array that need to be sorted
i : int
    the left boundary of this array (i=0 by default)
j : int
    index that is one past the end of this array (None by default)

Returns
-------
list
    sorted input array
"""
def QuickSort(listToSort, i=0, j=None):
    # Set default value for j if None.
    if j is None:
        j = len(listToSort)
    if i < j-1:
        # divide and conquer based on partition method
        q = partition(listToSort, i, j)
        QuickSort(listToSort, i, q)
        QuickSort(listToSort, q+1, j)
    return listToSort


"""
partition

Divide the input array by a selected pivot value

Parameters
----------
listToSort : list
    an array that need to be sorted
i : int
    the left boundary of this array (i=0 by default)
j : int
    index that is one past the end of this array (None by default)

Returns
-------
int
    the index where the input array should be divided
"""
def partition(listToSort, left, right):
    # randomly select an element as the pivot
    p = random.randint(left, right-1)
    # place the pivot element in the end
    listToSort[p], listToSort[right-1] = listToSort[right-1], listToSort[p]
    pivotIdx = right - 1
    pivot = listToSort[pivotIdx]

    i = left - 1
    for j in range(left, pivotIdx):
        if listToSort[j] <= pivot:
            i += 1
            listToSort[i], listToSort[j] = \
                listToSort[j], listToSort[i]

    listToSort[i + 1], listToSort[pivotIdx] = \
        listToSort[pivotIdx], listToSort[i + 1]
    return i + 1


"""
Importing the testing code after function defs to ensure same references.
"""
from project1tests import *

"""
Main function.
"""
if __name__ == "__main__":
    print('Testing Selection Sort')
    print()
    testingSuite(SelectionSort)
    print()
    print('Testing Insertion Sort')
    print()
    testingSuite(InsertionSort)
    print()
    print('Testing Bubble Sort')
    print()
    testingSuite(BubbleSort)
    print()
    print('Testing Merge Sort')
    print()
    testingSuite(MergeSort)
    print()
    print('Testing Quick Sort')
    print()
    testingSuite(QuickSort)
    print()
    print('UNSORTED measureTime')
    print()
    measureTime()
    print()
    print('SORTED measureTime')
    print()
    measureTime(True)
