#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// 在 path 指定的位置寻找名字为 target 的文件。
// 先处理当前路径，再对其中的子目录递归调用自己。
void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // 打开 path；打开失败时打印错误并返回。
  fd = open(path, O_RDONLY);
  if(fd < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // 获取 path 的类型。
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // 如果 path 是普通文件或设备，比较最后的文件名。
  if(st.type == T_FILE || st.type == T_DEVICE){
    int start = 0;

    for(int i = 0; path[i] != '\0'; i++){
      if(path[i] == '/')
        start = i + 1;
    }

    if(strcmp(path + start, target) == 0)
      printf("%s\n", path);
  } else if(st.type == T_DIR){
    // 检查拼接子路径时是否会超出缓冲区。
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      fprintf(2, "find: path too long\n");
      close(fd);
      return;
    }

    // 先把当前路径复制到 buf，并在末尾添加 '/'.
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p = '/';
    p++;

    // 读取目录中的每一个目录项。
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;

      // 跳过当前目录和父目录，避免无限递归。
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = '\0';
      if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
        continue;

      // buf 现在是一个子路径，继续递归查找。
      find(buf, target);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: find <path> <target>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
