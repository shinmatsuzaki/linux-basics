# Socketプログラミング学習メモ

### Socket API

#### socket システムコール

C言語にてソケットを使用する際は、`socket`システムコールにてソケットをオープンする。
```C
#include <sys/types.h>
#include <sys/socket.h>

int socket(int family, int type, int protocol)

```
`family`では以下のいずれかのプロトコルファミリを指定する.  
- AF_INET  : IPv4プロトコル 
- AF_INET6 : IPv6プロトコル
- AF_LOCAL : Unixドメインプロトコル
- AF_ROUTE : 経路制御ソケット
- AF_KEY   : キーソケット

`type`では以下のいずれかのソケット型を指定する.  
 - SOCKET_STREAM : ストリームソケット
 - SOCKET_DGRAM  : データグラムソケット
 - SOCK_RAW      : rawソケット
