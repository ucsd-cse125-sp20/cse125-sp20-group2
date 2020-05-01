# Server Game 

1. ServerGame receives a ClientMessage
2. GameProcessor processes the ClientMessage and update GameState
3. ServerGame gets the Game::ServerMessage from MessageBuilder and sends to all clients

# Client Game