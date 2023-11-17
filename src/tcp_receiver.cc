#include "tcp_receiver.hh"
#include "wrapping_integers.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  // Your code here.
  (void)message;
  (void)reassembler;
  (void)inbound_stream;
  if(!isn_.has_value())
  {
    if(!message.SYN)
    {
      return;
    }
    isn_ = message.seqno;
  }
  auto checkpoint = inbound_stream.bytes_pushed() + 1;
  auto abs_seqno = message.seqno.unwrap(isn_.value(),checkpoint);

  auto first_index = message.SYN ? 0 : abs_seqno - 1;
  reassembler.insert(first_index, message.payload, message.FIN, inbound_stream);
}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
  // Your code here.
  (void)inbound_stream;
  TCPReceiverMessage msg;
  auto win_s = inbound_stream.available_capacity();
  msg.window_size = win_s > UINT16_MAX ? UINT16_MAX : win_s;
  if(isn_.has_value())
  {
    auto abs_seqno = inbound_stream.bytes_pushed() + 1 + inbound_stream.is_closed();
    msg.ackno = Wrap32::wrap(abs_seqno,isn_.value());
  }
  
  return msg;
}
