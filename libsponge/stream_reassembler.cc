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

StreamReassembler::StreamReassembler(const size_t capacity) : buff(capacity, '\0'), check(capacity, false), output(capacity), myCapacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t size = data.size();
    if(index >= headIndex + output.remaining_capacity())
        return;
    if(index + size < headIndex){
        if(eof)
            isEof = true;
    }
    else {
        if (index + data.size() <= headIndex + output.remaining_capacity()) 
            if(eof)
                isEof = true;
        size_t len = min(index + data.size(), headIndex + output.remaining_capacity());
        for(size_t i = index; i < len; i++){ 
            if(i >= headIndex && !check[i - headIndex]){
                buff[i - headIndex] = data[i - index];
                check[i - headIndex] = true;
                unassembledBytes++;
            }
        }
        string str = "";
        while (check.front()) {
            str += buff.front();
            buff.pop_front();
            buff.push_back('\0');
            check.pop_front();
            check.push_back(false);
        }

        len = str.size();
        if (len > 0) {
            unassembledBytes -= len;
            headIndex += len;
            output.write(str);
        }
    }
    if (isEof && empty()) {
        output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return unassembledBytes; }

bool StreamReassembler::empty() const { return unassembledBytes == 0; }
