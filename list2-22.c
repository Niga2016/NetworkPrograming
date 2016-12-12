/*** ServerSide ***/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(){
  int sock0;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  struct addrinfo hints, *res;
  int err;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE; // AI_PASSIVE フラグを指定して getaddrinfo() を利用することで、bind() のための sockaddr 構造体を作成。AI_PASSIVE フラグを利用すると、INADDR_ANY, in6addr_any を切り分けたり、sockaddr_in 構造体と sockaddr_in6 構造体を個別に考える必要がなくなる。AI_PASSIVE で getaddrinfo() を利用する際は、getaddrinfo() の第1引数には NULL, 第2引数にはポート番号(文字列指定)を指定する。
  hints.ai_socktype = SOCK_STREAM;
  err = getaddrinfo(NULL, "12345", &hints, &res);
  if(err < 0){ printf("getaddrinfo: %s\n", gai_strerror(err)); return 1; }
  // getaddrinfo(): インターネットホストとサービスを識別するノードとサービスを渡し、1つ以上の addrinfo 構造体を返す(具体的には addrinfo 構造体のメモリ確保を行い、addrinfo 構造体のリンクリストを初期化し、第3引数にリストの先頭へのポインタを入れて返す)。このとき、各構造体のネットワークアドレスは node と service に一致し、hints で課されたすべての制限を満たすものとなる。リンクリストの要素は ai_next フィールドにより連結される。第3引数の addrinfo 構造体には、bind() または connect() を呼び出すときに指定できるインターネットアドレスが格納される。
  // 1st arg: [In]Hostname(node)
  // 2nd srg: [In]ポート番号に関連した Service
  // 3rd arg: [In]調べたいアドレスの制約に関するヒントとなる addrinfo 構造体
  // 4th arg: [Out]結果となる1つ以上の addrinfo 構造体
  
  /* ソケットの作成 */
  sock0 = socket(res->ai_family, res->ai_socktype, 0);
  if(sock0 < 0){ perror("socket"); return 1; }
   // 1st arg: アドレスファミリ(AF_INET:IPv4 socket, AF_INET6:IPv6 socket, AF_UNIX:the communication between local processes)
  // 2nd arg: ソケットタイプ(SOCK_STREAM:TCP, SOCK_DGRAM:UDP, SOCK_ROW: the connection using IP directly)
  // 3rd arg: プロトコル(0:AutoSetting, IPPROTO_TCP:TCP/IP, IPPROTO_UDP:UDP/IP, IPPROTO_RAW:ICMP)
  
  if(bind(sock0, res->ai_addr, res->ai_addrlen) != 0){ perror("bind"); return 1; }
  // 1st arg: ソケット記述子(Socket descropter)
  // 2nd arg: プロトコルに対応するアドレス構造体へのポインタ
  // 3rd arg: アドレス構造体のサイズ
  /* addrinfo 構造体の解放 */
  
  freeaddrinfo(res);

  /* TCPクライアントからの接続世級を待つ状態に */
  listen(sock0, 5);
  // 1st arg: ソケット記述子(Socket descripter)
  // 2nd arg: 待ち行列の長さ(クライアントからの未acceptの確立済み接続を待つことが出来る数) -> 5
  
  /* TCPクライアントからの接続要求を受け付け */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr*)&client, &len);
  // 1st arg: ソケット記述子(Socket descropter)
  // 2nd arg: クライアントのアドレスポインタ（sockaddr_in構造からsockaddr構造へのキャストする）接続されたクライアントのアドレスなどが格納される。sockaddr_in構造体として、その内容を取り出すことで、クライアント側のIPアドレスやポート番号などの情報を確認することができる。NULLを指定すると、クライアント側の情報を返さない。
  // 3rd arg: アドレス構造体の長さ
  
  /* クライアントに5文字送信 */
  write(sock, "HELLO", 5);
  // 1st arg: ファイルハンドラ -> ソケット記述子(Socket descripter)
  // 2nd arg: データのアドレス -> 送信する文字列
  // 3rd arg: データサイズ -> 文字列長
  
  /* TCPセッションの終了 */
  close(sock);

  /* listenするsocketの終了 */
  close(sock0);
  return 0;
}
