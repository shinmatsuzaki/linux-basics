# Socketプログラミング学習メモ

### 主要な要学習事項
#### system call
|名称|説明|
----|---- 
|socket()|ソケットをopen|
|close()|ソケットをclose|
----|---- 
|bind()|自ホストのIPaddr/Port番号の設定|
|liseten()|コネクションの受付開始|
|connect()|コネクション確立要求|
|accept()|受け付けたコネクション用にソケットを作る|
----|---- 
|recv()|メッセージの受信(コネクション型)|
|send()|メッセージの送信(コネクション型)|
|recvfrom()|メッセージの受信(コネクションレス型)|
|sendto()|メッセージの送信(コネクションレス型)|
----|---- 
|select()|入出力の多重化|
|poll()|入出力の多重化|
|epoll()|入出力の多重化|
----|---- 
|setsockopt()|ソケットオプションの設定|
#### ソケットライブラリ関数
#### ソケット構造体
|名称|説明|
----|---- 
|s_addr|IPアドレスを格納|
|sockaddr|ソケットアドレスを格納する構造体の雛形|
|sockaddr_in|IPアドレスとポート番号を格納|
|servent|サービスの情報を格納|

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
