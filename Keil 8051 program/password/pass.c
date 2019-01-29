#include<reg51.h>
#include<string.h>
char a;
char k[10]="bobby";
char b[10];
sbit m=P2^1;
sbit n=P2^2;

void x(char v)
{
	 SBUF=v;
			while(!TI);
			TI=0;
			
}
void write(char *p)
	{ while(*p)
		{x(*p);
			p++;
		}
		x(0x0d);
	}
void recieve()
{	
	int j=0;
	write("enter the password");
	while(SBUF!='\r')
	{	while(RI==0);
		RI=0;
		if(SBUF!='\r')
		{	b[j]=SBUF;
			j++;	
		}

	}
}
void password()
{while(1)
	{	
		recieve();
	if(strcmp(b,k)==0)
		{write("password correct");
			return;
		}
		write("password incorrect");
		write(b);
		write(k);

	
	}
		
}
void main()
{	volatile char c;
	
	TMOD=0x20;
	SCON=0x50;
	TH1=0xfd;
	TR1=1;
	write("begin");
	password();
		while(1)
	{
		while(RI==0);
		c=SBUF;
		RI=0;
		if(c=='A')
			m=1;
		else if(c=='B')
			m=0;
		else if(c=='C')
			n=1;
		else if(c=='D')
			n=0;
	}
}	
		