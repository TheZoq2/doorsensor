
class DynamicString {
    public:
        AtStatusListener();
        ~AtStatusListener();

        void add_char(char c);
    private:
        char* current_message;
        size_t length;
        size_t current_length;
};
