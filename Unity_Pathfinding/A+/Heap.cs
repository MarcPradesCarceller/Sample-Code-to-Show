using System;
using System.Collections;
using UnityEngine;

public class Heap<T> where T : IHeapItem<T> {

    T[] items;
    int currentItemCount;
    public Heap(int maxHeapSize)
    {
        items = new T[maxHeapSize];
    }

    public void Add(T item)
    {
        item.HeapIndex = currentItemCount;
        items[currentItemCount] = item;
        PercolateUp(item);
        currentItemCount++;
    }

    public T RemoveFirst()
    {
        T firstItem = items[0];
        currentItemCount--;
        items[0] = items[currentItemCount];
        items[0].HeapIndex = 0;
        PercolateDown(items[0]);
        return firstItem;
    }

    public bool Contains(T item)
    {
        return Equals(items[item.HeapIndex], item);
    }

    public void UpdateItem(T item)
    {
        PercolateUp(item);
        //PercolateDown(item);
    }

    public int Count
    {
        get { return currentItemCount; }
    }


    void PercolateUp(T item)
    {
        int parentIndex = (item.HeapIndex - 1) / 2;
        while (true)
        {
            T parentItem = items[parentIndex];
            if (item.CompareTo(parentItem) > 0)
            {
                Swap(item, parentItem);
            }
            else break;
            parentIndex = (item.HeapIndex - 1) / 2;

        }
    }

    void PercolateDown(T item)
    {
        while (true)
        {
            int childIndexL = item.HeapIndex * 2 + 1;
            int childIndexR = item.HeapIndex * 2 + 2;
            int temp = 0;

            if (childIndexL < currentItemCount)
            {
                temp = childIndexL;
                if (childIndexR < currentItemCount)
                {
                    if (items[childIndexL].CompareTo(items[childIndexR]) < 0)
                    {
                        temp = childIndexR;
                    }
                }

                if (item.CompareTo(items[temp]) < 0)
                {
                    Swap(item, items[temp]);
                }
                else return;
            }
            else return;
        }
    }

    void Swap(T itemA, T itemB)
    {
        items[itemA.HeapIndex] = itemB;
        items[itemB.HeapIndex] = itemA;
        int temp = itemA.HeapIndex;
        itemA.HeapIndex = itemB.HeapIndex;
        itemB.HeapIndex = temp;
    }

}

public interface IHeapItem<T> : IComparable<T>
{
    int HeapIndex
    {
        get;
        set;
    }
}
