# Socketプログラミング学習メモ

### Socket API

#### socket(2)

C言語にてソケットを使用する際は、`socket`関数にてソケットをオープンする。
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

`socket`関数は、成功するとソケットディスクリプタ(*socket descriptor*)あるいは*sockfd*と呼ばれる  
非負整数を返す.  

#### bind(2)とソケットアドレス構造体

```C
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *myaddr, socklent_t addrlen
```

### 参考文献
 - UNIXネットワークプログラミング, 1999, ピアソン・エデュケーション
 - 基礎からわかるTCP/IP ネットワーク実験プログラミング
