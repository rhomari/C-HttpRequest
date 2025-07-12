#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <http.h>
#include <winhttp.h>
#include <time.h>
#include <ctype.h>
HINTERNET CreateSession();

int main(){
    DWORD dwSize = 0;
    DWORD TotalSize = 0;
    DWORD dwDownloaded = 0;
    clock_t start, end;
    start = clock();
    HINTERNET hSession = CreateSession();
    if (hSession == NULL) return 1;

   
    
    HINTERNET hConnection = WinHttpConnect(hSession, L"restcountries.com",INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (hConnection == NULL) return 1;
  
    HINTERNET hRequest = WinHttpOpenRequest(hConnection, L"GET", L"/v3.1/all", L"HTTP/1.1", WINHTTP_NO_REFERER,  WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (hRequest == NULL) return 1;
  
    BOOL result = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (!result) return 1;
    
    BOOL onData=WinHttpReceiveResponse(hRequest, NULL);
    if (!onData) return 1;
    FILE *file = fopen("countries.json", "wb");
    char *buffer ;
    do{
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
        {
            printf("WinHttpQueryDataAvailable failed\n");
            break;
        }
        if (!dwSize)
             break;
        buffer = (char*)malloc(dwSize + 1);
        if (!buffer)
           break;
        if (!WinHttpReadData(hRequest, (LPVOID)buffer, dwSize, &dwDownloaded))
           break;
        
        fwrite(buffer, 1, dwDownloaded, file);
        buffer = NULL;
        free(buffer);
    }while(dwSize > 0);
    end = clock();
    fclose(file);
    
    long total_time = (end - start) *1000/ CLOCKS_PER_SEC;
    
    printf("finished in %ld ms\n", total_time);
   

    if (hRequest)
    {
        WinHttpCloseHandle(hRequest);
    }
    if (hConnection)
    {
        WinHttpCloseHandle(hConnection);
    }
    if (hSession)
    {
        WinHttpCloseHandle(hSession);
    }
}
HINTERNET CreateSession(){
   return WinHttpOpen(L"WinHTTP", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
}
