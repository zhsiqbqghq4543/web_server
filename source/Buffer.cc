#include "Buffer.h"
#include <sys/uio.h>
#include <assert.h>

Buffer::Buffer() : read_index_(0), write_index_(0)
{
    data_vec_.resize(1024);
}

char *Buffer::get_ptr_(int index)
{
    return &(*data_vec_.begin()) + index;
}

void Buffer::push_data(char *data_ptr, int size)
{
    int writeable_size_back = data_vec_.size() - write_index_;
    int writeable_size_all = writeable_size_back + read_index_;

    if (size > writeable_size_back) // cant easy copy back
    {

        if (size <= writeable_size_all) // all move to begin
        {
            std::copy(get_ptr_(read_index_), get_ptr_(write_index_), get_ptr_(0));
        }
        else if (size > writeable_size_all) // must double
        {
            std::vector<char> new_vec(std::max(write_index_ - read_index_ + size, size * 2));
            std::copy(get_ptr_(read_index_), get_ptr_(write_index_), &(*new_vec.begin()));
            data_vec_.swap(new_vec);
        }
        read_index_ = 0;
        write_index_ -= read_index_;
    }

    std::copy(data_ptr, data_ptr + size, get_ptr_(write_index_));
    write_index_ += size;
}

int Buffer::read_fd(int fd)
{
    char extra_buf[65536];
    struct iovec vec[2];
    int writeable_size_back = data_vec_.size() - write_index_;
    vec[0].iov_base = get_ptr_(write_index_);
    vec[0].iov_len = write_index_;
    vec[1].iov_base = extra_buf;
    vec[1].iov_len = sizeof(extra_buf);

    int iovcnt = (writeable_size_back < sizeof(extra_buf)) ? 2 : 1;
    int n = readv(fd, vec, iovcnt);

    assert(n >= 0);
    if (n <= writeable_size_back)
    {
        write_index_ += n;
    }
    else
    {
        write_index_ = data_vec_.size();
        push_data(extra_buf, sizeof(extra_buf));
    }
    return n;
}
