*  How to use wbc379
1. Launch client
   - Launch the client with the following params wbc379 hostname portnumber [keyfile]
     i. The keyfile is optional
   - If a keyfile is not passed into the client, messages will not be able to be encrypted
   - If client launches successfully it will let you know with a response message from the server
     that looks like...
     CMPUT379 Whiteboard Server v0\n38\n
   - The 38 lets you know how many entries are on the server
   - All messages passed by the client are base64 encrypted (this is not to spec, but not against it either)
2. Once the client has done the handshake with the server it will then ask if you want to Get info from the Server or Update.
   - Press g for get(?)
   - Press u for update(@)
   - If you chose get, proceed to step 6
3. Next the server will ask you which row you're trying to access
   - Please input only numbers in this section and press enter when done.
   - Failure to include only numbers will cause this section to end up in an infinite loop
4. If you chose update the client will then ask you if you want to encrypt the message
   - Press y for yes
   - Press n for no
5. You may press q in either steps 2, 3, or 4 to quit the server
6. Then then client will prompt you to enter you're message. 
   - This message can be any ascii compatible string except for new line
   - Hit enter once you're message is done
   - If you're looking to exit the server here, type exit and hit enter and the server will close 
     out of the loop
7. The client will then try to talk to the server
   - If there is an error in from the server the client will inform you and quit out
   - If you're updating an entry your response will look like '!3e0\n' letting you know the third 
     row was successfully updated
8. Repeat from step 2

