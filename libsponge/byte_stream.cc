#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <iostream>
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : myCapacity(capacity){}

size_t ByteStream::write(const string &data) {
    size_t len = data.size();
    size_t size = buf.size();
    if(len + size > myCapacity){
        len = myCapacity - size;
    }
    buf.append(data, 0, len);
    bytesWritten += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t size = buf.size();
    if(size == 0)
        return "";
    size_t readNum = min(len, size);
    string readString = buf.substr(0, readNum);
    return readString;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t eraseNum = min(len, buf.size());
    buf.erase(0, eraseNum);
    bytesRead += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \returns a vector of bytes read
std::string ByteStream::read(const size_t len) { 
    const auto ret = peek_output(len);
    pop_output(len);
    // cout << "bytes read: " << buf.substr(0, len) << endl;
    return ret;
}


bool ByteStream::error() const { return errorState; }

void ByteStream::set_error() { errorState = true; }

void ByteStream::end_input() { endInput = true; }

bool ByteStream::input_ended() const { return endInput; }

size_t ByteStream::buffer_size() const { return buf.size(); }

bool ByteStream::buffer_empty() const { return buf.size() == 0; }

bool ByteStream::eof() const { return endInput && (buffer_size() == 0); }

size_t ByteStream::bytes_written() const { return bytesWritten; }

size_t ByteStream::bytes_read() const { return bytesRead; }

size_t ByteStream::remaining_capacity() const { return myCapacity - buf.size(); }

