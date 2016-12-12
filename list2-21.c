/* 名前から得られるIPアドレスを IPv4, IPv6 に限らず、すべて取得する */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(){
  char *hostname = "localhost";
  char *service = "http";
  struct addrinfo hints, *res0, *res;
  int err;
  int sock;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = PF_UNSPEC; // PF_UNSPEC: 不特定

  if((err = getaddrinfo(hostname, service, &hints, &res0)) != 0){
    printf("error %d\n", err); return 1;
  }
  // getaddrinfo(): インターネットホストとサービスを識別するノードとサービスを渡し、1つ以上の addrinfo 構造体を返す(具体的には addrinfo 構造体のメモリ確保を行い、addrinfo 構造体のリンクリストを初期化し、第3引数にリストの先頭へのポインタを入れて返す)。このとき、各構造体のネットワークアドレスは node と service に一致し、hints で課されたすべての制限を満たすものとなる。リンクリストの要素は ai_next フィールドにより連結される。第3引数の addrinfo 構造体には、bind() または connect() を呼び出すときに指定できるインターネットアドレスが格納される
  // 1st arg: [In]Hostname(node)
  // 2nd srg: [In]ポート番号に関連した Service
  // 3rd arg: [In]調べたいアドレスの制約に関するヒントとなる addrinfo 構造体
  // 4th arg: [Out]結果となる1つ以上の addrinfo 構造体

  for(res = res0; res != NULL; res = res->ai_next){
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sock < 0){ continue; }
    /* getaddrinfo関数によって得られた結果に応じて順次接続 */
    if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
      close(sock);
      continue;
    }
    /* 接続が成功したら通信を行うコードへと移行 */
    break;
  }

  freeaddrinfo(res0);

  /* 有効な接続ができなかった場合 */
  if(res == NULL){
    printf("failed\n");
    return 1;
  }

  /* ここ以降にsockを用いたプログラムを記述 */
  
  return 0;
}
