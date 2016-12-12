/*** Server that doesn't use bind() ***/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ポート番号とbindされたアドレスを表示する関数 */
void print_my_port_num(int sock){
  char buf[16];
  struct sockaddr_in s;
  socklen_t sz;

  sz = sizeof(s);

  /* ソケットの名前の取得 */
  if(getsockname(sock, (struct sockaddr*)&s, &sz) != 0){
    perror("getsockname");
    return;
  }

  /* bindされているIPアドレスを文字列に変換 */
  inet_ntop(AF_INET, &s.sin_addr, buf, sizeof(buf));

  /* 結果の表示 */
  printf("%s : %d\n", buf, ntohs(s.sin_port));
}

int main(){
  int s0, sock;
  struct sockaddr_in peer;
  socklen_t peerlen;

  /* ソケットの作成 */
  s0 = socket(AF_INET, SOCK_STREAM, 0);
  if(s0 < 0){ perror("socket"); return 1; }
  // 1st arg: アドレスファミリ(AF_INET:IPv4 socket, AF_INET6:IPv6 socket, AF_UNIX:the communication between local processes)
  // 2nd arg: ソケットタイプ(SOCK_STREAM:TCP, SOCK_DGRAM:UDP, SOCK_ROW: the connection using IP directly)
  // 3rd arg: プロトコル(0:AutoSetting, IPPROTO_TCP:TCP/IP, IPPROTO_UDP:UDP/IP, IPPROTO_RAW:ICMP)

  /* bind なしでの listen */
  if(listen(s0, 5) != 0){ perror("listen"); return 1; }
  // 1st arg: ソケット記述子(Socket descripter)
  // 2nd arg: 待ち行列の長さ(クライアントからの未acceptの確立済み接続を待つことが出来る数) -> 5  // 1st arg: Socket descripter
  // 2nd arg: Length of queue(the number of clients that can be made to wait at connection request) -> 5

  /* listen後に自動的に未使用ポートが割り当てられていることを確認 */
  print_my_port_num(s0);

  /* TCP接続の受領 */
  peerlen = sizeof(peer);
  sock = accept(s0, (struct sockaddr*)&peer, &peerlen);
  if(sock < 0){ perror("accept"); return 1; }
  // 1st arg: ソケット記述子(Socket descropter)
  // 2nd arg: クライアントのアドレスポインタ（sockaddr_in構造からsockaddr構造へのキャストする）接続されたクライアントのアドレスなどが格納される。sockaddr_in構造体として、その内容を取り出すことで、クライアント側のIPアドレスやポート番号などの情報を確認することができる。NULLを指定すると、クライアント側の情報を返さない。
  // 3rd arg: アドレス構造体の長さ

  printf("[Client Info]\n");
  printf("IPtype: %d\n", peer.sin_family);
  printf("Port:   %d\n", ntohs(peer.sin_port));
  printf("IPaddr: %s\n", inet_ntoa(peer.sin_addr));

  /* クライアントへ5文字の文字列を送信 */
  write(sock, "HOGE\n", 5);
  // 1st arg: ファイルハンドラ -> ソケット記述子(Socket descripter)
  // 2nd arg: データのアドレス -> 送信する文字列
  // 3rd arg: データサイズ -> 文字列長// 1st arg: File handle -> Socket descripter

  close(sock);
  close(s0);
  return 0;
}
