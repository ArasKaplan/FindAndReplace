#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct element{//Diðer dillerde bulunan dictionary mantýðýný kullanarak +++
	int slide;//+++char ve int deðerine sahip olan structure'lar yarattým
	char ch;
};

struct element* createTable(char* find){
	int i;
	int j;
	
	struct element *Table;
	Table=(struct element*)malloc(sizeof(struct element)*(strlen(find)-1));
	
	for(i=0;i<strlen(find)-1;i++){//256 karakterlik bir dizi açmak yerine 5 tane structure kullandým;
		Table[i].ch=find[i];
		Table[i].slide=strlen(find)-i-1;
		for(j=0;j<i;j++){
			if(find[i]==Table[j].ch){//harf önceden kullanýldýysa slide deðeri güncelleniyor
				Table[j].slide=strlen(find)-i-1;
			}
		}
	}
	return Table;
}

int getSlide(struct element* Table,char cur,char* find){//Table'dan slide miktarýný gönderiyor
	if(strlen(find)==1){
		return 1;
	}
	int i=0;
	for(i=0;i<strlen(find)-1;i++){
		if(cur==Table[i].ch){
			return Table[i].slide;
		}
	}
	return strlen(find);
}

int sensitive(char* text,char* find,char* replace,struct element* Table){
	int initial=strlen(text);
	int i;
	int j;
	int counter=0;
	int z=0;
	int k;
	int flag=0;
	int shifter=0;
	for(i=strlen(find)-1;i<strlen(text);i){
		flag=0;
		z=0;
		for(j=0;j<strlen(find) && flag==0;j++){
			if(text[i-j]!=find[strlen(find)-1-j]){//bulunmasý gereken stringe kadar gidiliyor
				flag=1;//eðer farklý bir harf bulunursa for döngüsüne ve aþaðýdaki if statementlarýn içine girmiyor ve i'yi slide deðeri kadar ilerletiyor
				i+=getSlide(Table,text[i],find);
				}
			}
			if(flag==0){
				counter++;
				if(strlen(replace)>strlen(find)){//eklenecek sözcük bulunacak olandan fazlaysa bulunan sözcüðün son harfinden itibaren aradaki fark kadar kaydýrýlýyor
					shifter=strlen(replace)-strlen(find);
					for(k=initial+shifter;k>i+shifter;k--){
						text[k]=text[k-shifter];
					}
					for(k=0;k<strlen(replace);k++){
						text[k+i-j+1]=replace[k];
					}
					initial=initial+shifter;
					i=i+shifter;
				}
				else if(strlen(replace)==strlen(find)){//eklenecek sözcük bulunacak sözcük direkt deðiþim yapýlýyor
					for(k=0;k<strlen(find);k++){
						text[k+i-j+1]=replace[k];
					}
					i=i+shifter;
				}
				else{//eklenecek sözcük bulunan sözcükten kýsaysa her þey aradaki fark kadar geri kaydýrýlýyor
					shifter=strlen(find)-strlen(replace);//her þeyi shifter kadar geri al

					for(k=0;k<strlen(replace);k++){
						text[k+i-j+1]=replace[k];
					}
					for(k=i-shifter+1;k<initial;k++){
						text[k]=text[k+shifter];
					}
					initial=initial+shifter;
					i=i-1;
				}
			}
		}
	return counter;	
}


int notSensitive(char* text,char* find,char* replace,struct element* Table){
	int initial=strlen(text);
	int i;
	int j;
	int counter=0;
	int z=0;
	int k;
	int flag=0;
	int shifter=0;
	for(i=strlen(find)-1;i<strlen(text);i){
		flag=0;
		z=0;
		for(j=0;j<strlen(find) && flag==0;j++){
			if(text[i-j]!=find[strlen(find)-1-j] && text[i-j]+32!=find[strlen(find)-1-j]){//Not sensitive kýsmýnda, bulunacak sözcük önceden küçültüldüðü için+++
				flag=1;                                                                   //+++ sadece text'teki sözcüðün harflerinin büyük veya ayný olmasýna bakýlýyor+++ 
				if(text[i]>64 && text[i]<91){											  //+++ text'in harfinin küçük find'ýn harfinin büyük olmasýna bakmaya gerek kalmýyor
				i+=getSlide(Table,text[i]+32,find);
			}
				else{
					i+=getSlide(Table,text[i],find);
				}
				}
			}
			if(flag==0){
				counter++;
				if(strlen(replace)>strlen(find)){//eklenecek sözcük bulunacak olandan fazlaysa bulunan sözcüðün son harfinden itibaren aradaki fark kadar kaydýrýlýyor
					shifter=strlen(replace)-strlen(find);
					for(k=initial+shifter;k>i+shifter;k--){
						text[k]=text[k-shifter];
					}
					for(k=0;k<strlen(replace);k++){
						text[k+i-j+1]=replace[k];
					}
					initial=initial+shifter;
					i=i+shifter;
				}
				else if(strlen(replace)==strlen(find)){//eklenecek sözcük bulunacak sözcük direkt deðiþim yapýlýyor
					for(k=0;k<strlen(find);k++){
						text[k+i-j+1]=replace[k];
					}
					i++;
				}
				else{//eklenecek sözcük bulunan sözcükten kýsaysa her þey aradaki fark kadar geri kaydýrýlýyor
					shifter=strlen(find)-strlen(replace);

					for(k=0;k<strlen(replace);k++){
						text[k+i-j+1]=replace[k];
					}
					for(k=i-shifter+1;k<initial;k++){
						text[k]=text[k+shifter];
					}
					initial=initial+shifter;//yeni sözcük eskisinden büyük olduðunda i daha önce traverse edilmemiþ bir yeri gösteriyor+++
					i=i-1;//+++Bu durum kelime veya harf kaçýrmaya neden olabilir.
				}
			}
		}
	return counter;	
}

int main(){
	char find[100];
	char replace[100];
	char text[2000];	//Eðer text 1500den büyükse text array inin büyüklüðünü arttýrýn.
	FILE *fp;
	char filename[100];
	clock_t start,end;
	
	printf("Please enter the filename:");
	gets(filename);
	fp=fopen(filename,"rb+");
	if(fp==NULL){
		printf("Error! File not found");
		return 1;
	}
	
	int i=0;

	while(!feof(fp) ){
		text[i]=fgetc(fp);
		i++;
	}
	text[i-1]='\0';
	
	printf("\nOriginal text:%s",text);
	
	printf("\nPlease enter the string to search for:");
	gets(find);
	
	printf("\nPlease enter the replacement string:");
	gets(replace);
	
	struct element *Table1;
	Table1=createTable(find);
	
	struct element *Table2;
	
	
	printf("If you would like a case sensitive search enter '1', \nIf you dont want a case sensitive search enter anything else:\n");
	char state;
	scanf("%c",&state);
	int counter;
	
	 start = clock();//Zaman baþlangýcý inputlar alýndýktan sonra yapýlýyor
	
	switch(state){
		case '1':
			counter=sensitive(text,find,replace,Table1);
			fclose(fp);
			
			fp=fopen(filename,"w");
			for(i=0;i<strlen(text);i++){//Carriage return  ('\r') fazladan 1 satýr daha atlamasýna neden oluyor.+++
				if(text[i]=='\r')//+++bunun olmamasý için yerine boþluk ekliyorum
				text[i]=' ';//Kod bu olaydan dolayý satýr atlamalý caselerde de çalýþýyor
			}
			fputs(text,fp);
			printf("%s",text);
			fclose(fp);
			
			printf("\n Found and Replaced:%d",counter);
			printf("\nCheck the file to see the differences");
			break;
			
		default:
			for(i=0;i<strlen(find);i++){//Zaten Not case sensitive seçildiði için find sözcüðünün harflerinin büyük ya da küçük olmasýný deðiþtirmek+++
				if(find[i]<91 && find[i]>64){//+++ bir sorun yaratmayacaktýr
					find[i]=find[i]+32;
				}
			}
			
			Table2=createTable(find);			
			counter=notSensitive(text,find,replace,Table2);
			
			fclose(fp);
			fp=fopen(filename,"w");
			for(i=0;i<strlen(text);i++){//Carriage return -> '\r' fazladan 1 satýr daha atlamasýna neden oluyor.+++
				if(text[i]=='\r')//+++bunun olmamasý için yerine boþluk ekliyorum
				text[i]=' ';//Kod bu olaydan dolayý satýr atlamalý caselerde de çalýþýyor
			}
			fputs(text,fp);
			printf("%s",text);
			fclose(fp);
			
			printf("\n Found and Replaced:%d",counter);
			printf("\nCheck the file to see the differences");			
	}
	end=clock();
	 printf("\nRunning Time: %f ms",(double)((double)end-(double)start)/CLOCKS_PER_SEC);
	return 0;
}
