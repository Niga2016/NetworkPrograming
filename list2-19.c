#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(){

  char *hostname = "localhost";
  struct addrinfo hints, *res;
  struct in_addr addr;
  int err;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  if((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0){ printf("error %d\n", err); return 1; }
  // getaddrinfo(): インターネットホストとサービスを識別するノードとサービスを渡し、1つ以上の addrinfo 構造体を返す(具体的には addrinfo 構造体のメモリ確保を行い、addrinfo 構造体のリンクリストを初期化し、第3引数にリストの先頭へのポインタを入れて返す)。このとき、各構造体のネットワークアドレスは node と service に一致し、hints で課されたすべての制限を満たすものとなる。リンクリストの要素は ai_next フィールドにより連結される。第3引数の addrinfo 構造体には、bind() または connect() を呼び出すときに指定できるインターネットアドレスが格納される。
  // 1st arg: [In]Hostname(node)
  // 2nd srg: [In]ポート番号に関連した Service
  // 3rd arg: [In]調べたいアドレスの制約に関するヒントとなる addrinfo 構造体
  // 4th arg: [Out]結果となる1つ以上の addrinfo 構造体

  /* ローカルホストのIPアドレス取得 */
  addr.s_addr = ((struct sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;
  printf("IP Address: %s\n", inet_ntoa(addr));

  /* getaddrinfo 関数で確保したメモリの解放 */
  freeaddrinfo(res);
  // getaddrinfo 関数はスレッドセーフになっている。これは、getaddrinfo 関数の結果が毎回新しいメモリを確保して作成されるためである。したがって、getaddrinfo 関数で確保されているメモリは、不要になった場合に解放する必要がある。
  
  return 0;
}
