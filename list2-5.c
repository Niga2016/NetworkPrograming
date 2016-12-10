/*** Server Side ***/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(){
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int len;
  int sock;
  
  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  // 1st arg: アドレスファミリ(AF_INET:IPv4 socket, AF_INET6:IPv6 socket, AF_UNIX:the communication between local processes)
  // 2nd arg: ソケットタイプ(SOCK_STREAM:TCP, SOCK_DGRAM:UDP, SOCK_ROW: the connection using IP directly)
  // 3rd arg: プロトコル(0:AutoSetting, IPPROTO_TCP:TCP/IP, IPPROTO_UDP:UDP/IP, IPPROTO_RAW:ICMP)
  if(sock0 < 0){ printf("%d\n", errno); perror("socket"); exit(1); }
  
  /* ソケットの設定 */
  addr.sin_family = AF_INET;         // 受信するIPタイプ -> IPv4 Socket
  addr.sin_port = htons(12345);      // 受信するPORT番号 -> 12345
  addr.sin_addr.s_addr = INADDR_ANY; // 受信するIPアドレス -> すべてのIPアドレス
  // htons(): u_short型をホストバイトオーダーからTCP/IPネットワークバイトオーダーに変換する
  
  if(bind(sock0, (struct sockaddr*)&addr, sizeof(addr)) < 0){ printf("%d\n", errno); perror("bind"); exit(1); };
  // 1st arg: ソケット記述子(Socket descropter)
  // 2nd arg: プロトコルに対応するアドレス構造体へのポインタ
  // 3rd arg: アドレス構造体のサイズ
  
  /* TCPクライアントからの接続要求待ち状態 */
  if(listen(sock0, 5) < 0){ printf("%d\n", errno); perror("listen"); exit(1); }
  // 1st arg: ソケット記述子(Socket descripter)
  // 2nd arg: 待ち行列の長さ(クライアントからの未acceptの確立済み接続を待つことが出来る数) -> 5
  
  /* TCPクライアントからの接続要求受領 */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr*)&client, (socklen_t*)&len);
  if(sock < 0){ printf("%d\n", errno); perror("accept"); exit(1); }
  // 1st arg: ソケット記述子(Socket descropter)
  // 2nd arg: クライアントのアドレスポインタ（sockaddr_in構造からsockaddr構造へのキャストする）接続されたクライアントのアドレスなどが格納される。sockaddr_in構造体として、その内容を取り出すことで、クライアント側のIPアドレスやポート番号などの情報を確認することができる。NULLを指定すると、クライアント側の情報を返さない。
  // 3rd arg: アドレス構造体の長さ

  printf("[Client Info]\n");
  printf("IPtype: %d\n", client.sin_family);
  printf("Port:   %d\n", ntohs(client.sin_port));
  printf("IPaddr: %s\n", inet_ntoa(client.sin_addr));
  // ntohs(): u_short型をTCP/IPネットワークバイトオーダーからホストバイトオーダーに変換する
  // inet_ntoa(): ネットワークバイトオーダで渡されたインターネットホストアドレスを、IPv4のドット区切りの10進数表記の文字列に変換する。文字列は静的に割当てられたバッファーに格納されて返されるので、この後でこの関数を再度呼び出すと文字列は上書きされる。

  /* 5文字の文字列送信 */
  if(write(sock, "Hello", 5) < 0){ printf("%d\n", errno); perror("write"); exit(1); }
  // 1st arg: ファイルハンドラ -> ソケット記述子(Socket descripter)
  // 2nd arg: データのアドレス -> 送信する文字列
  // 3rd arg: データサイズ -> 文字列長

  /* TCP接続終了 */
  if(close(sock) < 0){ printf("%d\n", errno); perror("close"); exit(1); }
  // 1st arg: ファイルハンドラ -> ソケット記述子(Socket descripter)

  /* listenするソケットの終了 */
  if(close(sock0) < 0){ printf("%d\n", errno); perror("close"); exit(1); }

  return 0;
}
