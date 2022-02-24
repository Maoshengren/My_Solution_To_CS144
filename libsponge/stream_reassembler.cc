#include "stream_reassembler.hh"

#include <iostream>
#include <algorithm>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

//template <typename... Targs>
//void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _buff(capacity, '\0'), _check(capacity, false), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t size = data.size();
    if(index >= _head_index + _output.remaining_capacity())
        return;
    if(index + size < _head_index){
        if(eof)
            _eof = true;
    }
    else {
        if (index + data.size() <= _head_index + _output.remaining_capacity()) 
            if(eof)
                _eof = true;
        size_t len = min(index + data.size(), _head_index + _output.remaining_capacity());
        for(size_t i = index; i < len; i++){ 
            if(i >= _head_index && !_check[i - _head_index]){
                _buff[i - _head_index] = data[i - index];
                _check[i - _head_index] = true;
                _unassembled_bytes++;
            }
        }
        string str = "";
        while (_check.front()) {
            str += _buff.front();
            _buff.pop_front();
            _buff.push_back('\0');
            _check.pop_front();
            _check.push_back(false);
        }

        len = str.size();
        if (len > 0) {
            _unassembled_bytes -= len;
            _head_index += len;
            _output.write(str);
        }
    }
    if (_eof && empty()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }
