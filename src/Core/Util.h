#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <optional>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bitset>
#include <thread>
#include <chrono>
#include <mutex>

#define BT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace MyWorld
{
    namespace Util
    {
        std::string readText(const std::string& filepath);

        void log(char* stir);
        void log(glm::vec2 vec, char* text = "");
        void log(glm::vec3 vec, char* text = "");
        void log(float num, char* text = "");
        void log(double num, char* text = "");
        void log(bool boo, char* text = "");
        void log(uint8_t type, char* text = "");

        float getPrecition(float& num, int&& pre = 5);
        float getPrecition(float&& num, int&& pre = 5);

        template <typename T>
        void merge(T arr[], T temp[], int begin, int middle, int end, const std::function<float(T item1, T item2)>& compare)
        {
            int pointer = begin;
            int pointer1 = begin;
            int pointer2 = middle + 1;

            while (pointer1 <= middle && pointer2 <= end)
            {
                if (compare(arr[pointer1], arr[pointer2]) > 0)
                {
                    temp[pointer] = arr[pointer1];
                    pointer1++;
                }
                else
                {
                    temp[pointer] = arr[pointer2];
                    pointer2++;
                }
                pointer++;
            }

            while (middle - pointer1 >= 0)
            {
                temp[pointer] = arr[pointer1];
                pointer1++;
                pointer++;
            }

            while (end - pointer2 >= 0)
            {
                temp[pointer] = arr[pointer2];
                pointer2++;
                pointer++;
            }

            for (int i = begin; i <= end; i++)
            {
                arr[i] = temp[i];
            }
        };

        template <typename T>
        void mergeSortFunc(T arr[], T temp[], int begin, int end, const std::function<float(T item1, T item2)>& compare)
        {
            if (begin >= end) return;
            int middle = (end + begin) / 2;
            mergeSortFunc<T>(arr, temp, begin, middle, compare);
            mergeSortFunc<T>(arr, temp, middle + 1, end, compare);
            merge(arr, temp, begin, middle, end, compare);
        };

        // TODO: improve this
        template <typename T>
        void mergeSort(T arr[], int size, const std::function<float(T item1, T item2)>& compare)
        {
            T temp[size];
            mergeSortFunc<T>(arr, temp, 0, size - 1, compare);
        };
    }
}
