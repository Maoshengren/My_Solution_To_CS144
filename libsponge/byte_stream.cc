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

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity){}

size_t ByteStream::write(const string &data) {
    size_t len = data.size();
    size_t size = _buff.size();
    if(len + size > _capacity){
        len = _capacity - size;
    }
    _buff.append(data, 0, len);
    _bytes_written += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t size = _buff.size();
    if(size == 0)
        return "";
    size_t _read_num = min(len, size);
    // _offset += _read_num;
    // return _buff.substr(_offset, _read_num);
    return _buff.substr(_offset, _read_num);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t _erase_num = min(len, _buff.size());
    _buff = _buff.substr(_erase_num);
    // _buff.erase(0, _erase_num);
    _bytes_read += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \returns a vector of bytes read
std::string ByteStream::read(const size_t len) { 
    const auto ret = peek_output(len);
    pop_output(len);
    // cout << "bytes read: " << buf.substr(0, len) << endl;
    return ret;
}


bool ByteStream::error() const { return _error; }

void ByteStream::set_error() { _error = true; }

void ByteStream::end_input() { _end_input = true; }

bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _buff.size(); }

bool ByteStream::buffer_empty() const { return _buff.size() == 0; }

bool ByteStream::eof() const { return _end_input && (buffer_size() == 0); }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buff.size(); }

