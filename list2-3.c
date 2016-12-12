#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(){

  int sock;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  printf("sock=%d\n", sock);
  
  return 0;
}
