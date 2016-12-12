/*** Client Side ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(){
  struct sockaddr_in server;
  // IPv4の場合は、sockaddr_in構造体
  // IPv6の場合は、sockaddr_in6構造体
  int sock;
  char buf[32];
  int n;

  /* ソケットの作成 */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  // 1st arg: アドレスファミリ(AF_INET:IPv4 socket, AF_INET6:IPv6 socket, AF_UNIX:the communication between local processes)
  // 2nd arg: ソケットタイプ(SOCK_STREAM:TCP, SOCK_DGRAM:UDP, SOCK_ROW: the connection using IP directly)
  // 3rd arg: プロトコル(0:AutoSetting, IPPROTO_TCP:TCP/IP, IPPROTO_UDP:UDP/IP, IPPROTO_RAW:ICMP)
  if(sock < 0){ printf("%d\n", errno); perror("socket"); exit(1); }
  
  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
  // inet_pton(): IPv4/IPv6アドレスをプレインテキスト形式からバイナリ形式に変換する
  // 127.0.0.1 is localhost
  
  /* クライアントからサーバへの接続 */
  if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){ printf("%d\n", errno); perror("connect"); exit(1); };
  // 1st arg: ソケット記述子(Socket descripter)
  // 2nd arg: sockaddr構造体へのポインタ
  // 3rd arg: アドレス構造体のサイズ
  
  /* サーバからのデータ受信 */
  memset(buf, 0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));
  if(n < 0){ printf("%d\n", errno); perror("read"); exit(1); }
  // 1st arg: ファイルハンドラ -> ソケット記述子(Socket descripter)
  // 2nd arg: データのアドレス -> 送信する文字列
  // 3rd arg: データサイズ -> 文字列長
  // ret: 受信データのバイト数
  
  printf("%d, %s\n", n, buf);

  /* ソケット終了 */
  if(close(sock) < 0){ printf("%d\n", errno); perror("close"); exit(1); }
  
  return 0;
}
