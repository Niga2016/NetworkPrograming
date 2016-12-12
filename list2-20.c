#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(){
  int err;

  /* 無効な引数による getaddrinfo 関数の利用 */
  if((err = getaddrinfo(NULL, NULL, NULL, NULL)) != 0){
    printf("error %d: %s\n", err, gai_strerror(err));
    // gai_strerror(): getaddrinfo 関数のエラー説明文字列を返す
    return 1;
  }

  return 0;
}
