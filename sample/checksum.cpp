#include <bits/stdc++.h>
 
using namespace std;
 
void getChecksum (char a[8], char b[8], char* checksum){
    char sum[10],complement[10];
    int i;

    if(strlen(a)==strlen(b))
    {
        char carry='0';
        int length=strlen(a);
        
        for(i=length-1;i>=0;i--)
        {
            if(a[i]=='0' && b[i]=='0' && carry=='0')
            {
                sum[i]='0';
                carry='0';
            }
            else if(a[i]=='0' && b[i]=='0' && carry=='1')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='0' && b[i]=='1' && carry=='0')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='0' && b[i]=='1' && carry=='1')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='0' && carry=='0')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='1' && b[i]=='0' && carry=='1')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='1' && carry=='0')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='1' && carry=='1')
            {
                sum[i]='1';
                carry='1';
 
            }
            else
                break;
        }

        // cout<<"\nSum="<<carry<<sum;
 
        for(i=0;i<length;i++){
            if(sum[i]=='0')
                complement[i]='1';
            else
                complement[i]='0';
        }
        
        if(carry=='1')
            carry='0';
        else
            carry='1';

        checksum[0] = carry;
        
        for (int x = 1; x < strlen(complement)+1; ++x)
        {
            checksum[x]=complement[x-1];
        }
        checksum[strlen(complement)+1]='\0';
        // cout<<"\nChecksum="<<carry<<complement<<endl;
        // return checksum;
    }
}

bool checkChecksum (char a[8], char b[8], char c[10]){
    char sum[10],complement[10];
    int i;
    bool same = true;

    if(strlen(a)==strlen(b))
    {
        char carry='0';
        int length=strlen(a);
        
        for(i=length-1;i>=0;i--)
        {
            if(a[i]=='0' && b[i]=='0' && carry=='0')
            {
                sum[i]='0';
                carry='0';
            }
            else if(a[i]=='0' && b[i]=='0' && carry=='1')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='0' && b[i]=='1' && carry=='0')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='0' && b[i]=='1' && carry=='1')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='0' && carry=='0')
            {
                sum[i]='1';
                carry='0';
 
            }
            else if(a[i]=='1' && b[i]=='0' && carry=='1')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='1' && carry=='0')
            {
                sum[i]='0';
                carry='1';
 
            }
            else if(a[i]=='1' && b[i]=='1' && carry=='1')
            {
                sum[i]='1';
                carry='1';
 
            }
            else
                break;
        }

        for(i=0;i<length;i++){
            if(sum[i]=='0')
                complement[i]='1';
            else
                complement[i]='0';
        }
        
        if(carry=='1')
            carry='0';
        else
            carry='1';
 
        char checksum[10];
        checksum[0] = carry;
        
        for (int x = 1; x < strlen(complement)+1; ++x)
        {
            checksum[x]=complement[x-1];
        }
        checksum[strlen(complement)+1]='\0';
        
        for (int y = 0; y < strlen(checksum); ++y)
        {
            if(checksum[y] != c[y]){
                same = false;
            }
        }
        return same;
    }
}

int main(){
    char header[8],data[8],checksum[10],cek[10];
    cin>>header;
    cin>>data;
    getChecksum(header,data,checksum);
    cout<<checksum<<endl;
    cin>>cek;
    cout<<checkChecksum(header,data,cek)<<endl;
    return 0;
}
