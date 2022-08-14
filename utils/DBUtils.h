#ifndef DBMS_DBUTILS_H
#define DBMS_DBUTILS_H

#include <string>
#include <unordered_map>
#include <vector>
#include "StringUtils.h"
#include "../headers/Record.h"

class DBUtils {
public:
    static void heapify(std::vector<Record>& arr, int n, int i, int columnId) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < n && arr[l].getValue(columnId) > arr[largest].getValue(columnId)) {
            largest = l;
        }

        if (r < n && arr[r].getValue(columnId) > arr[largest].getValue(columnId)) {
            largest = r;
        }
    }

    static void heapSort(std::vector<Record>& arr, int columnId) {
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i, columnId);
        }

        for (int i = n - 1; i > 0; i--) {
            std::swap(arr[0], arr[i]);
            heapify(arr, i, 0, columnId);
        }
    }
};
#endif //DBMS_DBUTILS_H
