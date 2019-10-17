# cMail (Chat App)
I've created this project at the first year of my university (2017). This project is coded with C programming language and uses socket technology. Without using graphics library I've managed to create a userfriendly GUI with text and background colors. This project is created in Turkish language. But report file and these parts are written in english. Rest of this file is from my old report for this project. 

## Nature of My Program 
I wanted to create a program which is user friendly, easy to understand and easy to use. My program checks incoming messages every moment by itself. User can add/delete contacts, send/receive messages or create accounts.
All of the operations are screened at the server console. Also there is an predefined account: "Admin". But there is nothing special for this account.
To put it in a nutshell you can add and chat with other users, delete contacts. Also messages list is listed from newest to the oldest message.
Chatting section is designed like modern chat apps, for example whatsapp.
You can send and receive messages at the same time, you don't need to run a command to check messages. 
If a user sends a message to you, program makes a sound to notify you.

## Used Functions List
 - **setlocale(LC_ALL,"Turkish");**
 *Sets program's language to Turkish. *
 - **SetConsoleTitle("c-Posta v1.0 (Hazirlayan Bilal Guvenc)");**
*Sets console name with string.*
 - **KonsolBoyutu(int width, length 50);**
*Sets console width and length with parameters.*
 - **GolgeCiz(int x, int y, int width, int length);**
 *Draws shadow under my boxes. (Actually it prints space character. Nothing special.)*
 - **KutuCiz(int x, int y, int width, int length);**
 *Draws box to the screen. Color must be set before function.*
 - **strcmp(s1, s2); **
 *Compares two strings.*
 - **strcpy(s1, s2);**
 *Copys string to another.*
 - **FD_ZERO(&readfds);**
 *Sets socket fd set’s to zero.*
 - **FD_SET(socket, &readfds);**
 *Adds socket’s to fd set.*
 - **FD_ISSET(socket, &readfds);**
 *returns TRUE or FALSE If socket’s FD is set it returns TRUE. With this I can get to other step if there aren’t any messages.*
 - **send(socket, "MESSAGE", sizeof("MESSAGE"), 0);**
 *Sends “MESSAGE” to socket.*
 - **recv(socket, STRING, sizeof(STRING), 0); **
 *Receives STRING form socket.*
 - **TarihBul();**
 *returns TARIH Finds current date and time and returns as a struct that I’ve created.*
 - **TarihKarsilastir(TARIH t1, TARIH t2);**
 *returns char ‘<’ or ‘>’ or ‘=’ Compares two different date structures that I’ve created and returns ‘<’, ‘>’ or ‘=’.*
 - **\_kbhit();**
 *returns TRUE or FALSE If there is keyboard hit it returns TRUE, if not returns FALSE. With this function I can type/send messages and receive messages from server at the same time.*
 - **\_getch();**
 *This function helps me to get 2 byte length of keys. For example arrow keys are two byte length.*
 - **TusOku(char \*ch);**
 *returns int I’ve created this function to detect two byte keys and gave them numbers to find which one is it.*
 - **WSAGetLastError();**
 *returns int (ERROR CODE) This function returns WSA's last error.*
 - And other basic functions such as printf(), scanf(), getch(), for(), if(), while() etc.

## Strengths and Limitations of My Program 
There are too many bugs. If a client doesn’t sends a message that it should send, my server waits it to send it. If someone wants to crash my program he can do it easily. I didn’t add code to prevent attacks. My server has a limited user space. I could've created it as a reallocable but there were some other problems took my time so it’s now stable. To set received messages in time order I’ve used pointer array which are set to every user and then I’ve used nested arryays to set them new to old. I’ve created structures for messages(stores source, date and message), contacts(stores contact name, phone number, total messages, new messages, all of the messages), users(stores ID, password, phone number, name), tarih(stores year, month, day, hour, minute, second) and similar structures like those.

## Screenshots
- **Image 1 - Login Screen**
![Login Screen](https://raw.githubusercontent.com/bilalguvenc/cMail-ChatApp/master/screenshots/SS1.png "Login Screen")

- **Image 2 - Signup Screen**
![Signup Screen](https://raw.githubusercontent.com/bilalguvenc/cMail-ChatApp/master/screenshots/SS2.png "Signup Screen")

- **Image 3 - Inbox Screen**
![Inbox Screen](https://raw.githubusercontent.com/bilalguvenc/cMail-ChatApp/master/screenshots/SS3.png "Inbox Screen")

- **Image 4 - Chat Screen**
![Chat Screen](https://raw.githubusercontent.com/bilalguvenc/cMail-ChatApp/master/screenshots/SS4.png "Chat Screen")

- **Image 5 - Server Console**
![Server Console](https://raw.githubusercontent.com/bilalguvenc/cMail-ChatApp/master/screenshots/SS5.png "Server Console")



## Sources
- https://www.udemy.com/course/learn-socket-programming-in-c-from-scratch/
- https://www.binarytides.com/code-tcp-socket-server-winsock/
- https://gist.github.com/DnaBoss/68c0f663132792c2176d
- https://stackoverflow.com/questions/9203362/c-color-text-in-terminal-applications-in-windows
- https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
