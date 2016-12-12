/*** 単純なHTTPクライアント ***/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[]){
  int err;
  int sock;
  char buf[32];
  char *deststr;
  struct addrinfo hints, *res0, *res;
  int n;

  /* コマンドライン引数の第1引数をWebサーバのドメイン名(FQDN)に設定 */
  if(argc != 2){ printf("Usage: %s dest\n", argv[0]); }
  deststr = argv[1];
  
  /* getaddrinfo() に渡すための addrindfo 構造体の設定 */
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = PF_UNSPEC;

  /* addrindfo 構造体と HTTP(TCP80番ポート) 用の sockaddr を返すように設定して実行 */
  if((err = getaddrinfo(deststr, "http", &hints, &res0)) != 0){
    printf("ERROR: %s\n", gai_strerror(errno));
  }

  /* getaddrinfo() の結果に対して、ひとつずつ connect() を実行 */
  for(res = res0; res != NULL; res = res->ai_next){
    printf("%d\n", res->ai_family);
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sock < 0){ continue; }

    if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
      close(sock); continue;
    }
    break;
  }

  /* getaddrinfo() によって確保されたメモリ領域の解放 */
  freeaddrinfo(res0);

  /* getaddrinfo() の結果をすべて試し、ひとつも connect() に成功しなかった場合 */
  if(res == NULL){ fprintf(stderr, "failed\n"); return 1; }

  /* HTTPで「/」をリクエストする文字列を生成 */
  snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");

  /* HTTPリクエストをWebサーバに対して送信 */
  n = write(sock, buf, (int)strlen(buf));
  if(n < 0){ perror("write"); return 1; }

  /* サーバ側がデータをすべて送信し終わり、サーバ側がTCPコネクションを切るまで処理 */
  while(n > 0){
    /* サーバからHTTPメッセージの受信 */
    n = read(sock, buf, sizeof(buf));
    if(n < 0){ perror("read"); return 1; }

    /* 受信結果を標準出力へ表示（ファイルディスクリプタ 1 は標準出力） */
    if(write(fileno(stdout), buf, n) < 0){ perror("write"); };
  }

  close(sock);
  return 0;
}

