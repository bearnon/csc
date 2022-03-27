#include <iostream>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

using namespace std;

int main(){
    char aa[] = {0, 0, 0};
    cout << sizeof(aa) << endl;
    cout << sizeof(struct udphdr) << endl;
    return 0;
}