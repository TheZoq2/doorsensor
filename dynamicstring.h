#ifndef H_DYNAMIC_STRING
#define H_DYNAMIC_STRING

#include <cstddef>

class DynamicString {
    public:
        DynamicString();
        DynamicString(const char* data);
        DynamicString(const DynamicString& other);
        ~DynamicString();

        DynamicString& operator=(const DynamicString& other);

        void add_char(char c);

        const char * const get() const;

        size_t size() const;
    private:
        char* data;
        std::size_t length;
        std::size_t current_length;
};

#endif
