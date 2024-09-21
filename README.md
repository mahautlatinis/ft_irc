# ft_irc 📚

Checkout the rfc here: https://www.rfc-editor.org/rfc/rfc2812#section-1.3

IRC Client I recommend on a mac : `LimeChat`.

Use `Wireshark` to see the packets sent by a client. (filter `6667/TCP`)

## Connection registration

To run the server:

```sh
# define the password you want but make sure to add this argument
./ircserv 6667 PASSWORD
```

To run the server another port so the proxy can run on 6667:

```sh
./ircserv 6668 PASSWORD
```

To run the proxy (optional):

```sh
python3 proxy.py
```

To test a new client connection from the terminal (optional)

```sh
brew install telnet

# then
telnet 127.0.0.1 6667
PASS PASSWORD
NICK nickname
USER username 0 * :realname
```

# Use the port number of the server if there is no proxy

# Avoid crashing when a signal is killing a client connection.

```

## Commands to test

#### Find information about the administrator of the server

- `ADMIN`

#### Set away status (automatic reply)

- `AWAY :reason`
- `AWAY` to remove away status

#### Shutdown the server (operator permissions)

- `DIE`

#### Invite another user to a channel

- `INVITE nickname #channel`

#### Joining / Creating channels

- `JOIN #channel`

⚠️ Make sure you implement the possibility to join multiple channels at once (which is not the case in the current project)
Example:

- `JOIN #channel1,#channel2,#channel3`

⚠️ Channel names with & are supported but we did not implement the full features.
We prefixed with a `#` channel names that are missing it, but you'd rather not do it (not supported on every client).

⚠️ Make sure channels protected with keys are well implemented, topics should not be defined if key is not mentionned or incorrect.

- `TOPIC #channel key :newtopic`

#### Kick a user from a channel (part by force from a channel)

- `KICK #channel nickname`
- `KICK #channel nickname :reason`

⚠️ Avoid crashing when the user is kicking itself.

#### Kill a client (operator permissions) - disconnected from the server

- `KILL nickname :reason`

#### List channels and their topics

- `LIST`

#### List specific channels

- `LIST #channel`

#### Change mode of a user

The user mentionned must be the same as the one connected to the server.

- `MODE nickname +i` (to give invisible status)
- `MODE nickname -i` (to remove invisible status)

#### Change mode of a channel

- `MODE #channel +t` (to give topic protection)
- `MODE #channel -t` (to remove topic protection)
- `MODE #channel +k password` (to give a key to the channel)

Make sure you implement more options than we did (even though you are not supposed to do all of it).
⚠️ Make sure you implement properly the operator options.

#### Get message of the day

- `MOTD`

#### Listing users present in a channel

Make sure multiple users are present in the channel.
Checkout on the terminal what the server sends back, cause you might probably see nothing on the client software.

- `NAMES`

#### Listing all users in a channel

- `NAMES #channel`

#### Define nickname at registration or edit it

- `NICK nickname`

#### Sed a notice message (no automatic answers)

- `NOTICE nickname :Hello`

#### Become an operator

- `OPER username password`

⚠️ Make sure the expected password corresponds to the server's password (dynamic).
For some reason we chose to use a static password here.

#### Part a message from a channel

- `PART #channel`
- `PART #channel :this is a very nice message`

#### Define the password at connection registration

- `PASS password`

#### Send a PING to the server

- `PING ft_irc`

#### Sending private message to a user

- `PRIVMSG nickname :Hello`

#### Sending private message to a channel

- `PRIVMSG #channel :Hello`

#### Sending private message to a channel with a key

- `PRIVMSG #channel key :Hello`

#### Quit (stop client session) - Error message is sent by the server to the client

- `QUIT`

#### Query local time

- `TIME`

#### Change topic of a channel

- `TOPIC #channel :newtopic`

#### Define the username, realname and hostname at connection

- `USER username localhost ft_irc :realname`

#### Query the version of the server's implementation

- `VERSION "ft_irc"`

#### List all users (WHO) using wildcards

- `WHO user*`

#### List all users (WHO) using wildcards and operator

- `WHO user* o`

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

Disclaimer: while testing it afterwards, we noticed that some segmentation faults could be encountered.
Make sure to test it properly and to fix it if you encounter any.
```
