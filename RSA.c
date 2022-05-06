#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define e 7 
int  ExtendEuclide(int m,int n)
{
    int a,a1,b,b1,c,d,q,r,t;
    a1=b=1,a=b1=0,c=m,d=n;
    while(1)
    {
        q=c/d,r=c%d;
        if(r==0)
        {
            // printf("(%d)*%d+(%d)*%d=%d\n",a,m,b,n,d);
            return a;   //a为逆元
        }
        c=d,d=r,t=a1,a1=a,a=t-q*a,t=b1,b1=b,b=t-q*b;
    }
}
//求最大公约数 
int gcd(int a,int b)
{
	if(b==0) 
	return a;
	else 
	return gcd(b,a%b);
}
//快速模幂
int pow_mod(int a,int b,int c) //y=a^b mod c,都平方,从后向前为1乘a,
{
    int y=1;
    while(b>0)
    {
        if(b%2==1)
            y=(y*a)%c;
        a=(a*a)%c;
        b=(b-b%2)/2;    //b>>=1
    }
    return y;
}
//快速模乘
int multiply_mod(int a,int b,int c)
{
    int y=0;
    while(b>0)
    {
        if(b%2==1)
            y=(y+a)%c;
        a=(a+a)%c;
        b=b=(b-b%2)/2;
    }
    return y;
}
//素数检验
int Miller_Ranbin_Primity_Test(int n,int t) //奇数n>=3,安全参数t
{
    int a,b,j,i;
    int r=n-1,s=0;
    while(!(r&1))    //x分解为（2^s）*r
    {
        s++;
        r>>=1;
    }
    //printf("%d %d",s,r);
    for(i=0;i<t;i++)   //重复t次
    {
        a=rand()%(n-2)+2;  //生成[2,n]的随机底数
        if(gcd(a,n)>1)    //测试不通过
        return 0;
        else
        b=(a^r)%n;     
        if(b==1 || b==n-1) //通过测试，进行下一轮
        for(j=1;j<s;j++)   //然后进行s-1次平方
        {
            b=(b*b)%n;
            if(b==n-1)     //测试通过
            break;
        }
        if(j==s)      //不通过
        return 0;
    }
    return 1;    //通过
}
//随机数
int GetPrime(int k)   //表示素数的范围为0---k-1
{
    int a,p;
    a=rand()%k;
    for(int i=0;;i++)
    {
        if(Miller_Ranbin_Primity_Test(a,10)==1 && gcd(e,a-1)==1)  //判断生成的a,是否与e互质 
        {
            p=a;
            return p;
            break;
        }
        a=rand()%k;
    }
}
//加密
int encrypt(int m,int n)
{
    int c;
    printf("--------加密过程------\n");
    c=pow_mod(m,7,n);
    return c;
}
//解密
int decrypt(int c,int n,int d,int p,int q)
{   
    int dp,dq,inv_p,inv_q;
    int x1,x2,c1,c2,m;
    printf("--------解密过程------\n");
    dp=d%(p-1);
    dq=d%(q-1);
    if(dp<0)   //负数处理
    dp=dp+(p-1);
    if(dq<0)   //负数处理
    dq=dq+(q-1);
    printf("dp=%d dq=%d\n",dp,dq);
    inv_p=ExtendEuclide(p,q);
    if(inv_p<0)    //负数处理
    inv_p=inv_p+q;
    printf("inv_p=%d\n",inv_p);
    inv_q=(1-p*inv_p)/q;
    printf("inv_q=%d\n",inv_q);
    c1=c%p;
    if(c1<0)  //负数处理
    c1=c1+p;
    c2=c%q;
    if(c2<0)
    c2=c2+p;
    x1=pow_mod(c1,dp,p);
    x2=pow_mod(c2,dq,q);
    m=(x1*q*inv_q+x2*p*inv_p)%n;
    if(m<0)
    m=m+n;
    printf("解密明文m=%d\n",m);

}
    
int main()
{
    while(1)
    {
        int flag=0,s;
        printf("请选择加密-0，解密-1 退出-2\n");
        scanf("%d",&s);
        switch(s)
        {
            int p,q,n,d,k,m,c;
            case 0:
                printf("------生成密钥-----\n");
                srand(time(NULL));
                p=GetPrime(101);
                q=GetPrime(101);
                k=(q-1)*(p-1);
                n=q*p;
                printf("素数p=%d 素数q=%d n=p*q=%d\n",p,q,n);
                d=ExtendEuclide(e,k);
                if(d<0)
                d=d+k;
                printf("密钥d=%d\n",d);
                m=rand()%n;
                printf("随机生成的明文m=%d\n",m);
                c=encrypt(m,n);
                printf("密文c=%d\n",c);
                break;
            case 1:
                printf("密文c,n,d,p,q\n");
                scanf("%d%d%d%d%d",&c,&n,&d,&p,&q);
                decrypt(c,n,d,p,q);
                break;
            case 2:
                printf("退出\n");
                flag=1;
                break;
            default:
                printf("输入错误！\n");
                break;
        }
        if(flag==1)
        break;
    }
    return 0;
}
