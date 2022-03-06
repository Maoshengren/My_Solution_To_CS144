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

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
     if (index >= _head_index + _capacity) {  // capacity over
        return;
    }

    // handle extra substring prefix
    block_node seg_arrive;
    if (index + data.length() <= _head_index) {  // couldn't equal, because there have emtpy substring
        goto JUDGE_EOF;
    } else if (index < _head_index) {
        size_t offset = _head_index - index;
        seg_arrive.data.assign(data.begin() + offset, data.end());
        seg_arrive.begin = _head_index;
        seg_arrive.length = seg_arrive.data.length();
    } else {
        seg_arrive.begin = index;
        seg_arrive.length = data.length();
        seg_arrive.data = data;
    }
    _unassembled_bytes += seg_arrive.length;

    // merge substring
    do {
        long merged_bytes = 0;
        auto iter = _buffers.lower_bound(seg_arrive);
        while (iter != _buffers.end() && (merged_bytes = merge_block(seg_arrive, *iter)) >= 0) {
            _unassembled_bytes -= merged_bytes;
            _buffers.erase(iter);
            iter = _buffers.lower_bound(seg_arrive);
        }
        // merge prev
        if (iter == _buffers.begin()) {
            break;
        }
        iter--;
        while ((merged_bytes = merge_block(seg_arrive, *iter)) >= 0) {
            _unassembled_bytes -= merged_bytes;
            _buffers.erase(iter);
            iter = _buffers.lower_bound(seg_arrive);
            if (iter == _buffers.begin()) {
                break;
            }
            iter--;
        }
    } while (false);
    _buffers.insert(seg_arrive);

    // write to ByteStream
    if (!_buffers.empty() && _buffers.begin()->begin == _head_index) {
        const block_node head_block = *_buffers.begin();
        // modify _head_index and _unassembled_byte according to successful write to _output
        size_t write_bytes = _output.write(head_block.data);
        _head_index += write_bytes;
        _unassembled_bytes -= write_bytes;
        _buffers.erase(_buffers.begin());
    }

JUDGE_EOF:
    if (eof) {
        _eof = true;
    }
    if (_eof && empty()) {
        _output.end_input();
    }
}

long StreamReassembler::merge_block(block_node &block1, const block_node &block2) {
    block_node x, y;
    if (block1 < block2) {
        x = block1;
        y = block2;
    } else {
        x = block2;
        y = block1;
    }
    if (x.begin + x.length < y.begin) {
        return -1;  // no intersection, couldn't merge
    } else if (x.begin + x.length >= y.begin + y.length) {
        block1 = x;
        return y.length;
    } else {
        block1.begin = x.begin;
        block1.data = x.data + y.data.substr(x.begin + x.length - y.begin);
        block1.length = block1.data.length();
        return x.begin + x.length - y.begin;
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }
