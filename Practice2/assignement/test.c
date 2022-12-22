#include <stdio.h>
int main(){
char opmsg[1024];
opmsg[1]='+';
for(int i=0;i<1024;i++)
opmsg[i]=NULL;

for(int i=0;i<1024;i++)
printf("%c", opmsg[i]);

}
