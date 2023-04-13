#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* read shift value k, m, n and m rows of n numbers */
/* subtract k from all numbers except first one in each row */

void main()
{
long m,n,i,j,k,x;

scanf("%ld %ld %ld",&k,&m,&n);
printf("\n%ld %ld %ld",k,m,n);

for(i=1;i<=m;i++)
 for (j=1;j<=n;j++)
   {
     scanf("%ld ",&x);
     if(j==1)
       printf("\n%ld ",x);
     else
       printf(" %ld ",x-k);
   }
       printf("\n");
}
