#ifndef STREAM_H
#define STREAM_H

#include <cstring>
#include <fstream>

namespace Parser {

template <class charT>
class BaseStream {
  protected:
    int pos = 0;
    BaseStream() {}
  public:
    virtual void setPos(int p) {this->pos = p;}
    virtual int getPos() {return this->pos;}
    virtual charT next() {
      this->pos += 1;
      return this->get(this->pos-1);
    }
    virtual charT nextIf() {
      if (hasNext()) {
        return next();
      }
      return 0;
    }
    virtual ~BaseStream() { }

    virtual charT get(int pos=-1) = 0;
    virtual bool hasNext() = 0;

    virtual int lineNumber() {
      int p = 0;
      int line = 1;
      while (p < pos) {
        if (get(p) == '\n') {
          line += 1;
        }
        p += 1;
      }
      return line;
    }
    virtual int colNumber() {
      int p = 0;
      int col = 1;
      while (p < pos) {
        if (get(p) == '\n') {
          col = 1;
        }
        else {
          col += 1;
        }
        p += 1;
      }
      return col;
    }
};

template <class charT>
class BaseStringStream : public BaseStream<charT> {
  private:
    std::basic_string<charT> string;
  public:
    BaseStringStream(std::basic_string<charT> string) {
      this->string = string+" \n";
    }
    charT get(int pos = -1) {
      if (pos < 0) {
        pos = this->pos;
      }
      if (pos < string.size()) {
        return string.at(pos);
      }
      return (charT)0;
    }
    bool hasNext() {
      return string.size() > this->pos;
    }
};

template <class charT>
class BaseFileStream : public BaseStream<char> {
  private:
    std::basic_ifstream<charT>* file;
    bool ownsFile = false;
  public:
    BaseFileStream(std::string filePath) {
      this->file = new std::basic_ifstream<charT>();
      this->file->open(filePath);
      ownsFile = true;
    }
    BaseFileStream(std::basic_ifstream<charT>* file) {
      this->file = file;
    }
    void setPos(int p) {
      this->pos = p;
      file->clear(); // clear eof flag
      get();
    }
    charT get(int pos = -1) {
      if (pos < 0) { pos = this->pos; }
      file->seekg(pos);
      charT c;
      file->get(c);
      return c;
    }
    bool hasNext() {
      return file->is_open() && !file->eof();
    }
    ~BaseFileStream() {
      if (ownsFile) {
        if (this->file != NULL && this->file->is_open()) {
          this->file->close();
        }
        delete this->file;
        ownsFile = false;
        this->file = NULL;
      }
    }
};

typedef BaseStream<char> Stream;
typedef BaseStringStream<char> StringStream;
typedef BaseFileStream<char> FileStream;

}

#endif