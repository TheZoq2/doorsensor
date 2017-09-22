#include "dynamicstring.h"

#include <cstring>

DynamicString::DynamicString() : DynamicString(""){
}

DynamicString::DynamicString(const DynamicString& other) {
    *this = other;
}

DynamicString::DynamicString(const char* data) {
    std::size_t data_length = strlen(data);
    // To fit the final nullpointer
    data_length += 1;

    this->data = new char[data_length];

    for (size_t i = 0; i < data_length; ++i) {
        this->data[i] = data[i];
    }

    this->length = data_length;
    this->current_length = data_length;
}

DynamicString::~DynamicString() {
    delete[] this->data;
}

DynamicString& DynamicString::operator=(const DynamicString& other) {
    this->length = other.length;
    this->current_length = other.current_length;

    this->data = new char[current_length];

    for (size_t i = 0; i < current_length; ++i) {
        data[i] = other.data[i];
    }

    return *this;
}

void DynamicString::add_char(char c) {
    // If we've run out of space
    if(current_length >= length) {
        auto old_length = length;
        length = length*2;

        auto new_ptr = new char[length];

        for(size_t i = 0; i < old_length; ++i) {
            new_ptr[i] = data[i];
        }
        delete[] data;
        data = new_ptr;
    }

    this->data[current_length-1] = c;
    this->data[current_length] = '\0';
    current_length++;
}

const char * const DynamicString::get() const {
    return this->data;
}

size_t DynamicString::size() const {
    // Because we have a null character at the end
    return this->current_length-1;
}

