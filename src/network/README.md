# Game Protocol

## Synposis

TCP is a byte-stream and will not know the the difference between messages sent. 
As a passage of ritual, we have faced the TCP problem of multiple sends and a single receive. Packets, like memory, is an illusion. 

The issue is, we do not know how to interpret the bytes, because of an unknown length.

For example, all messages coming in to the server would be concatenated together,
meaning it would be impossible to break up the data
into parts without knowing the length of the individual parts on the server side. 

## Protocol

Header Format

[ Length | PB Data ]

Length - 2 bytes unsigned int
Data - Representation of message as char array

## Sender 

1. Get the sizeof(struct), set to 2 byte unsigned int, prefix to beginning.
2. Convert message to char ptr, append after length.
3. Send formatted data over the network.

## Receiver

1. Read first 2 bytes to figure out the Length (struct bytes).
2. Read length number of bytes to get the char representation of the message.
3. Convert char array to the correct message.

