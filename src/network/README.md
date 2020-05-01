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

* Length - sizeof(size_t) 32b or 64b depending on respective 32/64 bit architecture
* Data - Representation of message as char array

## Sender 

1. Get the sizeof(struct), set to size_t bytes and prefix to beginning.
2. Convert message to char ptr, append after length.
3. Send formatted data over the network.

## Receiver

1. Read first size_t bytes to figure out the Length (struct).
2. Read length number of bytes to get the char representation of the message.
3. Convert char array to the correct message.

## Usage

### NetworkClient

NetworkClient has two main methods, send and read. Send is used to send
data over to the server, and read is used to read data coming in from the
server into storage (you can access this storage via the member variable
"messages")
    

### ServerNetwork

This is the underlying server that will handle incoming connections and data.
The class has 3 main methods, send, sendToAll, and readAllMessages(). send
simply sends to a specific client, sendToAll sends to all clients, and
readAllMessages reads all incoming messages from all the clients.

### Data

The client will send events to the server and the server will send states back.
The client and server will receive and accept qualified messages -- ServerMessage and ClientMessage.
The Server/Client message will be an umbrella for all valid messages that each can receive.

#### ServerMessage

The message that the server passes to the client. 

TODO: This is still being designed

#### ClientMessage

The message that the client passes to the server.

TODO: This is still being designed