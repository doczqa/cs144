#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t c) { 
    this->capacity = c;
    this->buf = vector<char>(this->capacity,'1');
    this->l = this->r = 0;
    this->isfull = false;
    this->isInputEnd = false;
    this->readBytes = 0;
    this->writenBytes = 0;
    // std::cout<<"初始化，capacity="<<this->capacity<<std::endl;
}

size_t ByteStream::write(const string &data) {
    size_t cnt = 0;
    while(cnt < data.size()) {
        if(this->r == this->l && this->isfull) {
            return cnt;
        }
        buf[r] = data[cnt];
        r = (r+1)%this->capacity;
        this->writenBytes++;
        cnt++;
        if(r == l) {
            this->isfull = true;
            return cnt;
        }
    }
    return cnt;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string str="";
    size_t idx = l;
    for(size_t i=0;i<(len<capacity?len:capacity);i++) {//最多只能取出capacity个字节
        if(idx == r && isfull == false) { //空了
            return str;
        }
        str+=this->buf[idx];
        idx = (idx+1)%capacity;
    }
    return str;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {//这个和read的效果差不多
    for(size_t i=0;i<len;i++) {
        if(this->l == this->r) {
            if(this->isfull == true) {
                this->isfull = false;
            } else {
                return ;
            }
        }
        this->l = (this->l+1)%this->capacity;
        this->readBytes++;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string str="";
    for(size_t i=0;i<len;i++) {
        if(this->l == this->r) {
            if(this->isfull == true) {
                this->isfull = false;
            } else {
                return str;
            }
        }
        str+=this->buf[this->l];
        this->readBytes++;
        this->l = (this->l+1)%this->capacity;
    }
    return str;
}

void ByteStream::end_input() {
    this->isInputEnd = true;
}

bool ByteStream::input_ended() const { return this->isInputEnd; }

size_t ByteStream::buffer_size() const {
    if(this->r == this->l) {
        if(this->isfull) {
            return this->capacity;
        } else {
            return 0;
        }
    } else if(this->r > this->l) {
        return this->r-this->l;
    } else {
        return capacity - (this->l-this->r);
    }
}

bool ByteStream::buffer_empty() const { return this->l == this->r && this->isfull == false; }

bool ByteStream::eof() const { return this->l == this->r && this->isfull == false && this->isInputEnd; }

size_t ByteStream::bytes_written() const { return this->writenBytes; }

size_t ByteStream::bytes_read() const { return this->readBytes; }

size_t ByteStream::remaining_capacity() const { return this->capacity - this->buffer_size(); }
