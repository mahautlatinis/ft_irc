# ft_irc 📚

Rewriting an IRC server in C++ in team (@qdam and @bahaas)

- Status: validated (13/01/2022)

IRC Client I recommend on a mac : `LimeChat`
Use `Wireshark` to see the packets sent by a client. (filter `6667/TCP`)

To run the server:

```sh
./ircserv 6667 PASS
```

```sh
# test new client connection (using proxy)
nc -C 127.0.0.1 3333
CAP LS 302
PASS pass
NICK nickname
USER nickname username 127.0.0.1 :realname
```

<img width="1434" alt="accesing our irc server" src="https://github.com/malatinipro/ft_irc/assets/77189438/575bdaf9-8390-4800-9d51-ded87f2aaf10">
<img width="1440" alt="server sees new connection" src="https://github.com/malatinipro/ft_irc/assets/77189438/2c491a83-67b8-403c-ab86-8bbf420fbc42">
<img width="1424" alt="away" src="https://github.com/malatinipro/ft_irc/assets/77189438/9bd63a25-4ae8-4d51-a02b-e9112e81a2a3">
<img width="1429" alt="ourproxy" src="https://github.com/malatinipro/ft_irc/assets/77189438/eb77546c-4cc1-4d0d-b953-561f6a477cc8">
<img width="1429" alt="motd" src="https://github.com/malatinipro/ft_irc/assets/77189438/cd37c5ca-e454-4adf-946f-f66201573a1b">
<img width="1434" alt="response" src="https://github.com/malatinipro/ft_irc/assets/77189438/34824f5b-aeeb-4d29-bd64-5b2b87e52e0f">
<img width="1424" alt="response and request" src="https://github.com/malatinipro/ft_irc/assets/77189438/ed95f1ce-bd18-465b-abcd-49bf17e39f0e">
<img width="1439" alt="bonus bot" src="https://github.com/malatinipro/ft_irc/assets/77189438/796308a9-524d-4cf9-b6cb-79ccf4e32e9b">

![Alt text](/irc.png?raw=true 'ft_irc')

<img width="1397" alt="Capture d’écran 2023-10-03 à 19 55 55" src="https://github.com/malatinipro/ft_irc/assets/77189438/c94a0a30-bc1f-4831-8d93-ad31e774b02d">
