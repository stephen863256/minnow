#include <stdexcept>

#include "byte_stream.hh"

using namespace std;
#define min(x,y) (x)<(y)?(x):(y)

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) ,buffer(),hasClose (false) ,error(false),numOfPop(0) ,numOfPush(0) {}

void Writer::push( string data )
{
  /*
  if (data.length()+buffer.size()>capacity_){
    set_error();  // 写入的字节大于空闲字节，这是一种处理方式，设置错误码，然后能写多少写多少
  }
  //(void)data; // data 此时是字符串，先将data转为字节流
    for (uint64_t ptr=0;buffer.size()<capacity_&&ptr<data.length();++ptr){
      buffer.push(data.at(ptr));
      ++numOfPush;
    }
  // 有代码量更简洁,性能更高的优化写法： 采用min函数比较写入字符串和还可以写的哪个更短，取更短的写入即可
*/
  uint64_t dataLen=data.length();
  uint64_t toBePushed=min(available_capacity(),dataLen);
  if (toBePushed<dataLen) set_error();
  for (uint64_t i=0; i< toBePushed; ++i){
    buffer.push(data.at(i));
    ++numOfPush;
  }
}

void Writer::close()
{
  hasClose=true;
}

void Writer::set_error()
{
  error=true;
}

bool Writer::is_closed() const
{
  return hasClose;
}

uint64_t Writer::available_capacity() const
{
  return capacity_-buffer.size();
}

uint64_t Writer::bytes_pushed() const
{
  return numOfPush;
}

string_view Reader::peek() const
{
  return string_view(&buffer.front(),1);
}

bool Reader::is_finished() const
{
  return buffer.empty()&& hasClose; //仅仅当我读完了，并且对面writer close了，我的reader才算结束
}

bool Reader::has_error() const
{
  return error;
}

void Reader::pop( uint64_t len )
{
  uint64_t toBepoped = min(len,bytes_buffered());
  for (uint64_t i=0;i<toBepoped;++i){
    buffer.pop();
    ++numOfPop;
  } 
}

uint64_t Reader::bytes_buffered() const
{
  return buffer.size();
}

uint64_t Reader::bytes_popped() const
{
  return numOfPop;
}
