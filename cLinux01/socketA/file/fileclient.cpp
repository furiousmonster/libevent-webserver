#include "../_freecplus.h"

char strfilename[301];
int isize;
char strmtime[21];

int main(int argc, char *argv[])
{
  // 判断是否传入文件参数
  if (argc != 2)
  {
    printf("Using:./fileclient filename\n");
    printf("Sample:./fileclient /test.cpp\n");
    return -1;
  }

  // 文件属性读取
  strcpy(strfilename, argv[1]);

  if (FileMTime(strfilename, strmtime, "yyyymmddhh24miss") == false)
  {
    printf("文件%s不存在或没有访问权限\n", strfilename);
    return -1;
  }

  isize = FileSize(strfilename);

  // 测试文件代码
  // printf("filename=%s, mtime=%s, size=%d\n", strfilename, strmtime, isize);
  // return 0;


  CTcpClient TcpClient;
  if (TcpClient.ConnectToServer("127.0.0.1", 5858) == false)
  {
    printf("TcpClinet.ConnectToServer(\"127.0.0.1\", 5005) failed\n");
    return -1;
  }

  char strbuffer[1024];

  snprintf(strbuffer, 500, "<filename>%s</filename><size>%d</size><mtime>%s</mtime>", strfilename, isize, strmtime);
  printf("发送：%s\n", strbuffer);
  if (TcpClient.Write(strbuffer) == false) printf("client write error\n");

  // 发送文件内容
  CFile File;
  if (File.Open(strfilename, "r") == false) return -1;

  int readn = 0;
  while(true)
  {
    memset(strbuffer, 0, sizeof(strbuffer));

    if ((readn = File.Fread(strbuffer, 1000)) < 0) break;

    if (TcpClient.Write(strbuffer, readn) == false) return -1;
  }
  
  memset(strbuffer,0,sizeof(strbuffer));
  if (TcpClient.Read(strbuffer,20)==false) printf("client read error\n");  // 接收服务端的回应报文。
  printf("接收：%s\n",strbuffer);

}