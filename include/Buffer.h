#pragma once

#include <vector>

class Buffer
{
public:
    Buffer();
    char *read_ptr_();
    void push_data(char *data_ptr, int size);
    char *get_ptr_(int index);
    int read_fd(int fd);
    int get_one_line(char *&begin);
    void clear();

    std::vector<char> data_vec_;
    int read_index_;  //[0,read_index) has been read
    int write_index_; //[read , write) can be   read
                      //[write,  size) can be   write
private:
};